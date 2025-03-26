#include "TaskManager.h"

// Task handles
TaskHandle_t sensorTaskHandle = NULL;
TaskHandle_t displayTaskHandle = NULL;
TaskHandle_t statusTaskHandle = NULL;

// Synchronization and communication
QueueHandle_t sensorDataQueue = NULL;
SemaphoreHandle_t serialMutex = NULL;

// Module instances
DHT11Sensor sensor(DHT11_PIN);
LCDDisplay display(0x27, 16, 2);  // I2C address 0x27, 16 columns, 2 rows

// Filter buffers
float tempBuffer[FILTER_WINDOW_SIZE];
float humBuffer[FILTER_WINDOW_SIZE];

// Weights for weighted average filter (must sum to 1.0)
float weights[FILTER_WINDOW_SIZE] = {0.1, 0.15, 0.5, 0.15, 0.1};

void initTasks() {
    // Initialize modules
    sensor.init();
    display.init();
    
    // Initialize filter buffers
    for (int i = 0; i < FILTER_WINDOW_SIZE; i++) {
        tempBuffer[i] = 0.0;
        humBuffer[i] = 0.0;
    }
    
    // Create synchronization primitives
    serialMutex = xSemaphoreCreateMutex();
    if (serialMutex == NULL) {
        Serial.println(F("Failed to create mutex"));
        while(1); // Stop if mutex creation fails
    }
    
    // Create communication queue for sensor data
    sensorDataQueue = xQueueCreate(3, sizeof(SensorData));
    if (sensorDataQueue == NULL) {
        Serial.println(F("Failed to create queue"));
        while(1); // Stop if queue creation fails
    }
    
    // Create sensor task
    xTaskCreate(
        sensorTask,          // Task function
        "SensorTask",        // Task name
        256,                 // Stack size
        NULL,                // Parameters
        3,                   // Priority
        &sensorTaskHandle    // Task handle
    );
    
    // Create display task with offset
    xTaskCreate(
        displayTask,         // Task function
        "DisplayTask",       // Task name
        192,                 // Stack size
        NULL,                // Parameters
        2,                   // Priority
        &displayTaskHandle   // Task handle
    );
    
    // Create status task with different offset
    xTaskCreate(
        statusTask,          // Task function
        "StatusTask",        // Task name
        192,                 // Stack size
        NULL,                // Parameters
        1,                   // Priority
        &statusTaskHandle    // Task handle
    );
    
    SerialIO::printMessage(F("All tasks initialized"));
}

void sensorTask(void* parameters) {
    // For precise timing with vTaskDelayUntil
    TickType_t lastWakeTime = xTaskGetTickCount();
    
    // Buffer index for circular buffer
    int bufIndex = 0;
    
    while (1) {
        // Read raw sensor data
        float rawTemp = sensor.readTemperature();
        float rawHum = sensor.readHumidity();
        
        // Update circular buffers
        tempBuffer[bufIndex] = rawTemp;
        humBuffer[bufIndex] = rawHum;
        bufIndex = (bufIndex + 1) % FILTER_WINDOW_SIZE;
        
        // Apply salt and pepper filter (median filter)
        float medianTemp = SensorFilter::saltAndPepperFilter(tempBuffer, FILTER_WINDOW_SIZE);
        float medianHum = SensorFilter::saltAndPepperFilter(humBuffer, FILTER_WINDOW_SIZE);
        
        // Apply weighted average filter for additional smoothing
        float filteredTemp = SensorFilter::weightedAverageFilter(tempBuffer, FILTER_WINDOW_SIZE, weights);
        float filteredHum = SensorFilter::weightedAverageFilter(humBuffer, FILTER_WINDOW_SIZE, weights);
        
        // Apply saturation to keep values in valid range
        filteredTemp = SensorFilter::applySaturation(filteredTemp, TEMP_MIN, TEMP_MAX);
        filteredHum = SensorFilter::applySaturation(filteredHum, HUM_MIN, HUM_MAX);
        
        // Create data structure
        SensorData data = {
            rawTemp,
            rawHum,
            filteredTemp,
            filteredHum
        };
        
        // Send data to queue for other tasks
        xQueueOverwrite(sensorDataQueue, &data);
        
        // Schedule next execution with precise timing
        vTaskDelayUntil(&lastWakeTime, pdMS_TO_TICKS(SENSOR_READING_PERIOD));
    }
}

void displayTask(void* parameters) {
    // Apply offset from sensor task
    vTaskDelay(pdMS_TO_TICKS(50));
    
    // For precise timing
    TickType_t lastWakeTime = xTaskGetTickCount();
    SensorData data;
    char buffer[20];
    
    while (1) {
        // Get latest sensor data from queue
        if (xQueuePeek(sensorDataQueue, &data, 0) == pdTRUE) {
            // Take mutex for display update
            if (xSemaphoreTake(serialMutex, pdMS_TO_TICKS(10)) == pdTRUE) {
                // Display system status on first line
                display.displaySystemState("System Running");
                
                // Display processed sensor data on second line
                static bool showTemp = true;
                
                if (showTemp) {
                    sprintf(buffer, "Temp: %.1f C", data.filteredTemperature);
                } else {
                    sprintf(buffer, "Humidity: %.1f%%", data.filteredHumidity);
                }
                
                display.displaySensorData(buffer);
                showTemp = !showTemp;  // Toggle for next update
                
                xSemaphoreGive(serialMutex);
            }
        }
        
        // Schedule next execution with precise timing
        vTaskDelayUntil(&lastWakeTime, pdMS_TO_TICKS(DISPLAY_UPDATE_PERIOD));
    }
}

void statusTask(void* parameters) {
    // Apply offset from other tasks
    vTaskDelay(pdMS_TO_TICKS(100));
    
    // For precise timing
    TickType_t lastWakeTime = xTaskGetTickCount();
    SensorData data;
    char buffer[100];
    
    // For calculating uptime
    unsigned long startTime = millis();
    
    while (1) {
        // Get latest sensor data from queue
        if (xQueuePeek(sensorDataQueue, &data, 0) == pdTRUE) {
            // Calculate system uptime
            unsigned long uptime = (millis() - startTime) / 1000;
            
            // Take mutex for serial output
            if (xSemaphoreTake(serialMutex, pdMS_TO_TICKS(10)) == pdTRUE) {
                // Print status report using printf
                SerialIO::printMessage("--- System Status Report ---");
                sprintf(buffer, "Uptime: %lu seconds", uptime);
                SerialIO::printMessage(buffer);
                
                sprintf(buffer, "Raw Temperature: %.1f°C, Filtered: %.1f°C", 
                        data.rawTemperature, data.filteredTemperature);
                SerialIO::printMessage(buffer);
                
                sprintf(buffer, "Raw Humidity: %.1f%%, Filtered: %.1f%%", 
                        data.rawHumidity, data.filteredHumidity);
                SerialIO::printMessage(buffer);
                
                SerialIO::printMessage("---------------------------");
                
                xSemaphoreGive(serialMutex);
            }
        }
        
        // Schedule next execution with precise timing
        vTaskDelayUntil(&lastWakeTime, pdMS_TO_TICKS(STATUS_UPDATE_PERIOD));
    }
} 