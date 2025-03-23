#include <Arduino.h>
#include <Arduino_FreeRTOS.h>
#include <task.h>
#include <queue.h>
#include "config.h"

// External reference to queue
extern QueueHandle_t byteQueue;

// Task function for asynchronous byte reception and display
void asyncTask(void *pvParameters) {
    // Add initialization message
    Serial.println("Async Task started");
    
    // Wait for other tasks to initialize
    vTaskDelay(pdMS_TO_TICKS(300));
    
    // Initialize timing
    TickType_t xLastWakeTime = xTaskGetTickCount();
    
    // Buffer for received data
    uint8_t receivedByte;
    
    // Print queue status
    Serial.print("Queue ready to receive data (size: ");
    Serial.print(QUEUE_SIZE);
    Serial.println(" bytes)");
    
    while (true) {
        // Check if queue has data with timeout
        if (xQueueReceive(byteQueue, &receivedByte, pdMS_TO_TICKS(10)) == pdPASS) {
            // If we received the end marker (0), print a new line
            if (receivedByte == 0) {
                Serial.println(); // Move to new line
                Serial.println("------ End of sequence ------");
            } else {
                // Print the received byte with formatting
                Serial.print("Received: ");
                Serial.print(receivedByte);
                Serial.print(" | ");
            }
        }
        
        // Run this task precisely every ASYNC_TASK_DELAY ms
        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(ASYNC_TASK_DELAY));
    }
} 