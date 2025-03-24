#include "TaskManager.h"

// Task handles
TaskHandle_t sensorTaskHandle = NULL;
TaskHandle_t processingTaskHandle = NULL;
TaskHandle_t displayTaskHandle = NULL;

// Synchronization and communication
QueueHandle_t rawDataQueue = NULL;
QueueHandle_t processedDataQueue = NULL;
SemaphoreHandle_t serialMutex = NULL;  // Mutex for serial output protection

// Module instances
UltrasonicSensor sensor(TRIGGER_PIN, ECHO_PIN);
SignalProcessor signalProcessor;
LCDDisplay display(0x27, 16, 2);  // I2C address 0x27, 16 columns, 2 rows

void initTasks() {
    // Initialize modules
    sensor.init();
    display.init();
    
    // Create synchronization primitives
    serialMutex = xSemaphoreCreateMutex();
    if (serialMutex == NULL) {
        Serial.println("Failed to create mutex");
        while(1); // Stop if mutex creation fails
    }
    
    // Create communication queues
    rawDataQueue = xQueueCreate(5, sizeof(int));
    processedDataQueue = xQueueCreate(5, sizeof(int));
    
    if (rawDataQueue == NULL || processedDataQueue == NULL) {
        Serial.println("Failed to create queues");
        while(1); // Stop if queue creation fails
    }
    
    // Create sensor task
    xTaskCreate(
        sensorTask,             // Task function
        "SensorTask",           // Task name
        128,                    // Stack size
        NULL,                   // Parameters
        3,                      // Priority (highest)
        &sensorTaskHandle       // Task handle
    );
    
    // Create processing task
    xTaskCreate(
        processingTask,         // Task function
        "ProcessingTask",       // Task name
        256,                    // Stack size (larger due to filtering)
        NULL,                   // Parameters
        2,                      // Priority (medium)
        &processingTaskHandle   // Task handle
    );
    
    // Create display task
    xTaskCreate(
        displayTask,            // Task function
        "DisplayTask",          // Task name
        128,                    // Stack size
        NULL,                   // Parameters
        1,                      // Priority (lowest)
        &displayTaskHandle      // Task handle
    );
    
    // Add a small delay to offset tasks
    vTaskDelay(pdMS_TO_TICKS(50));
    
    // Log task creation
    SerialIO::printMessage("Tasks initialized");
}

void sensorTask(void* parameters) {
    // Initialize task timing for precise intervals
    TickType_t lastWakeTime = xTaskGetTickCount();
    
    while (1) {
        // Read raw sensor data
        int rawDistance = sensor.readDistance();
        
        // Send data to the processing queue
        xQueueSend(rawDataQueue, &rawDistance, 0);
        
        // Take mutex for printing (optional, for debugging)
        if (xSemaphoreTake(serialMutex, pdMS_TO_TICKS(10)) == pdTRUE) {
            SerialIO::printRawValue(rawDistance);
            xSemaphoreGive(serialMutex);
        }
        
        // Schedule next execution with precise timing
        vTaskDelayUntil(&lastWakeTime, pdMS_TO_TICKS(SENSOR_READING_PERIOD));
    }
}

void processingTask(void* parameters) {
    // Initialize task timing for precise intervals
    TickType_t lastWakeTime = xTaskGetTickCount();
    int rawDistance = 0;
    
    // Offset from the sensor task
    vTaskDelay(pdMS_TO_TICKS(20));
    
    while (1) {
        // Receive data from sensor queue
        if (xQueueReceive(rawDataQueue, &rawDistance, 0) == pdTRUE) {
            // Process the raw reading
            int processedDistance = signalProcessor.processReading(rawDistance);
            
            // Calculate noise reduction percentage
            float noiseReduction = signalProcessor.getNoiseReductionPercentage();
            
            // Send processed data to display queue
            xQueueSend(processedDataQueue, &processedDistance, 0);
            
            // Take mutex for printing (optional, for debugging)
            if (xSemaphoreTake(serialMutex, pdMS_TO_TICKS(10)) == pdTRUE) {
                SerialIO::printProcessedValue(processedDistance);
                SerialIO::printFilterInfo(noiseReduction);
                xSemaphoreGive(serialMutex);
            }
        }
        
        // Schedule next execution with precise timing
        vTaskDelayUntil(&lastWakeTime, pdMS_TO_TICKS(PROCESSING_PERIOD));
    }
}

void displayTask(void* parameters) {
    // Initialize task timing for precise intervals
    TickType_t lastWakeTime = xTaskGetTickCount();
    int processedDistance = 0;
    int reportCounter = 0;
    
    // Offset from the processing task
    vTaskDelay(pdMS_TO_TICKS(30));
    
    while (1) {
        // Receive data from processing queue
        if (xQueueReceive(processedDataQueue, &processedDistance, 0) == pdTRUE) {
            // Take mutex for display update
            if (xSemaphoreTake(serialMutex, pdMS_TO_TICKS(10)) == pdTRUE) {
                // Get noise reduction percentage
                float noiseReduction = signalProcessor.getNoiseReductionPercentage();
                
                // Update LCD display
                display.displayProcessedValue(processedDistance);
                
                // Every 5 cycles, print a full report
                reportCounter++;
                if (reportCounter >= 5) {
                    // Get a raw value for comparison
                    int rawDistance = 0;
                    if (xQueuePeek(rawDataQueue, &rawDistance, 0) != pdTRUE) {
                        rawDistance = processedDistance; // Fallback
                    }
                    
                    // Print comprehensive report to serial
                    SerialIO::printDataReport(rawDistance, processedDistance, noiseReduction);
                    reportCounter = 0;
                }
                
                xSemaphoreGive(serialMutex);
            }
        }
        
        // Schedule next execution with precise timing
        vTaskDelayUntil(&lastWakeTime, pdMS_TO_TICKS(DISPLAY_UPDATE_PERIOD));
    }
} 