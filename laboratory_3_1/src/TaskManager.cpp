#include "TaskManager.h"

// Task handles
TaskHandle_t sensorTaskHandle = NULL;
TaskHandle_t displayTaskHandle = NULL;

// Synchronization and communication
QueueHandle_t sensorDataQueue = NULL;
SemaphoreHandle_t serialMutex = NULL;  // Mutex for serial output protection

// Module instances
UltrasonicSensor sensor(TRIGGER_PIN, ECHO_PIN);
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
    
    // Create communication queue for sensor data
    sensorDataQueue = xQueueCreate(5, sizeof(int));
    if (sensorDataQueue == NULL) {
        Serial.println("Failed to create queue");
        while(1); // Stop if queue creation fails
    }
    
    // Create sensor task
    xTaskCreate(
        sensorTask,          // Task function
        "SensorTask",        // Task name
        128,                 // Stack size
        NULL,                // Parameters
        1,                   // Priority
        &sensorTaskHandle    // Task handle
    );
    
    // Create display task
    xTaskCreate(
        displayTask,         // Task function
        "DisplayTask",       // Task name
        128,                 // Stack size
        NULL,                // Parameters
        2,                   // Priority
        &displayTaskHandle   // Task handle
    );
    
    // Log task creation
    SerialIO::printMessage("Tasks initialized");
}

void sensorTask(void* parameters) {
    // Initialize task timing for precise intervals
    TickType_t lastWakeTime = xTaskGetTickCount();
    
    while (1) {
        // Read sensor data
        int distance = sensor.readDistance();
        
        // Send data to queue
        xQueueSend(sensorDataQueue, &distance, 0);
        
        // Take mutex for printing
        if (xSemaphoreTake(serialMutex, pdMS_TO_TICKS(10)) == pdTRUE) {
            SerialIO::printSensorData(distance);
            xSemaphoreGive(serialMutex);
        }
        
        // Schedule next execution with precise timing
        vTaskDelayUntil(&lastWakeTime, pdMS_TO_TICKS(SENSOR_READING_PERIOD));
    }
}

void displayTask(void* parameters) {
    // Initialize task timing for precise intervals
    TickType_t lastWakeTime = xTaskGetTickCount();
    int distance = 0;
    char statusBuffer[20];
    
    while (1) {
        // Receive data from queue
        if (xQueueReceive(sensorDataQueue, &distance, 0) == pdTRUE) {
            // Take mutex for display update
            if (xSemaphoreTake(serialMutex, pdMS_TO_TICKS(10)) == pdTRUE) {
                // Update status message
                sprintf(statusBuffer, "System Active");
                
                // Update display
                display.displaySystemState(statusBuffer);
                display.displaySensorData(distance);
                
                // Release mutex
                xSemaphoreGive(serialMutex);
            }
        }
        
        // Schedule next execution with precise timing
        vTaskDelayUntil(&lastWakeTime, pdMS_TO_TICKS(DISPLAY_UPDATE_PERIOD));
    }
} 