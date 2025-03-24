#include "TaskManager.h"

// Task handles
TaskHandle_t buttonTaskHandle = NULL;
TaskHandle_t sequenceTaskHandle = NULL;
TaskHandle_t outputTaskHandle = NULL;

// Synchronization and communication
SemaphoreHandle_t buttonSemaphore = NULL;
QueueHandle_t dataQueue = NULL;
SemaphoreHandle_t serialMutex = NULL;  // Mutex for serial output protection

// Module instances
Button button(BUTTON_PIN);
LED led1(LED1_PIN);  // LED for Task 1 (button press)
LED led2(LED2_PIN);  // LED for Task 2 (blinking sequence)
LED led3(LED3_PIN);  // LED for Task 3 (optional)

// Global counter
static uint8_t sequenceCounter = 0;

void initTasks() {
    // Create synchronization primitives
    buttonSemaphore = xSemaphoreCreateBinary();
    
    // Create communication queue
    dataQueue = xQueueCreate(10, sizeof(uint8_t) * 10);
    
    // Create mutex for serial output protection
    serialMutex = xSemaphoreCreateMutex();
    if (serialMutex == NULL) {
        // Failed to create the mutex - handle error (in this case, just log it)
        Serial.println("Failed to create mutex");
        while(1); // Stop if mutex creation fails
    }
    
    // Create tasks
    xTaskCreate(
        buttonTask,         // Task function
        "ButtonTask",       // Task name
        128,                // Stack size
        NULL,               // Parameters
        1,                  // Priority
        &buttonTaskHandle   // Task handle
    );
    
    xTaskCreate(
        sequenceTask,       // Task function
        "SequenceTask",     // Task name
        128,                // Stack size
        NULL,               // Parameters
        2,                  // Priority
        &sequenceTaskHandle // Task handle
    );
    
    xTaskCreate(
        outputTask,         // Task function
        "OutputTask",       // Task name
        128,                // Stack size
        NULL,               // Parameters
        3,                  // Priority
        &outputTaskHandle   // Task handle
    );
    
    // Log task creation
    SerialIO::printMessage("Tasks initialized");
}

void buttonTask(void* parameters) {
    // Initialize task timing
    TickType_t lastWakeTime = xTaskGetTickCount();
    
    while (1) {
        // Check button state every 10ms
        if (button.isPressed()) {
            // Take the mutex before serial output
            if (xSemaphoreTake(serialMutex, portMAX_DELAY) == pdTRUE) {
                SerialIO::printMessage("Button pressed - Task 1 got mutex");
                
                // Turn on LED1 for exactly 1 second
                led1.turnOn();
                
                // Signal the semaphore to trigger Task 2
                xSemaphoreGive(buttonSemaphore);
                
                // Wait for 1 second
                vTaskDelay(pdMS_TO_TICKS(LED1_ON_DURATION));
                
                // Turn off LED1
                led1.turnOff();
                
                SerialIO::printMessage("Task 1 releasing mutex");
                xSemaphoreGive(serialMutex);
            }
            
            // Add delay after releasing mutex to give other tasks a chance
            vTaskDelay(pdMS_TO_TICKS(100));
        }
        
        // Ensure precise timing
        vTaskDelayUntil(&lastWakeTime, pdMS_TO_TICKS(BUTTON_CHECK_PERIOD));
    }
}

void sequenceTask(void* parameters) {
    while (1) {
        // Wait for semaphore from Task 1
        if (xSemaphoreTake(buttonSemaphore, portMAX_DELAY) == pdTRUE) {
            // Take the mutex before serial output and critical section
            if (xSemaphoreTake(serialMutex, portMAX_DELAY) == pdTRUE) {
                SerialIO::printMessage("Task 2 got mutex");
            
                // Increment the counter
                sequenceCounter++;
                
                // Create a sequence of bytes (1, 2, ..., N)
                uint8_t sequence[10] = {0}; // Initialize with zeros
                for (uint8_t i = 0; i < sequenceCounter; i++) {
                    sequence[i] = i + 1;
                }
                
                // Send the sequence to the buffer
                xQueueSend(dataQueue, &sequence, portMAX_DELAY);
                
                // Blink LED2 N times
                led2.blink(sequenceCounter, LED2_ON_DURATION, LED2_OFF_DURATION);
                
                SerialIO::printMessage("Task 2 releasing mutex");
                xSemaphoreGive(serialMutex);
            }
            
            // Add delay after releasing mutex to give other tasks a chance
            vTaskDelay(pdMS_TO_TICKS(150));
            
            // Add 50ms delay after completing the blinking sequence
            vTaskDelay(pdMS_TO_TICKS(TASK2_DELAY));
        }
    }
}

void outputTask(void* parameters) {
    uint8_t receivedData[10] = {0};
    TickType_t lastWakeTime = xTaskGetTickCount();
    
    while (1) {
        // Read data from the buffer every 200ms
        if (xQueueReceive(dataQueue, &receivedData, 0) == pdTRUE) {
            // Take the mutex before serial output
            if (xSemaphoreTake(serialMutex, portMAX_DELAY) == pdTRUE) {
                SerialIO::printMessage("Task 3 got mutex");
                
                // Output the received bytes via serial port
                SerialIO::printByteArray(receivedData, 10);
                
                SerialIO::printMessage("Task 3 releasing mutex");
                xSemaphoreGive(serialMutex);
            }
            
            // Add delay after releasing mutex to give other tasks a chance
            vTaskDelay(pdMS_TO_TICKS(200));
        }
        
        // Ensure task runs every 200ms
        vTaskDelayUntil(&lastWakeTime, pdMS_TO_TICKS(TASK3_PERIOD));
    }
} 