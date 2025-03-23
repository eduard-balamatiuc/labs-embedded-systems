#include <Arduino.h>
#include <Arduino_FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#include <queue.h>
#include "config.h"

// External references to sync objects
extern SemaphoreHandle_t buttonSemaphore;
extern QueueHandle_t byteQueue;

// Counter for button presses
static uint8_t buttonPressCounter = 0;

// Flag to prevent multiple executions at startup
static bool firstRun = true;

// Task function for synchronization after button press
void syncTask(void *pvParameters) {
    // Add initialization message
    Serial.println("Sync Task started");
    
    // Initial delay to ensure other tasks are ready
    vTaskDelay(pdMS_TO_TICKS(1000));
    
    // Make sure LED is initially off
    digitalWrite(SYNC_LED_PIN, LOW);
    Serial.println("Sync LED turned off at startup");
    
    // Reset counter at startup
    buttonPressCounter = 0;
    
    // Wait a bit more to ensure everything is ready
    vTaskDelay(pdMS_TO_TICKS(1000));
    
    while (true) {
        // Only print waiting message the first time and when restarting the wait cycle
        static bool waitingMsgPrinted = false;
        if (!waitingMsgPrinted) {
            Serial.println("Sync Task: Waiting for semaphore");
            waitingMsgPrinted = true;
        }
        
        // Wait indefinitely for the semaphore from Task 1
        if (xSemaphoreTake(buttonSemaphore, portMAX_DELAY) == pdTRUE) {
            waitingMsgPrinted = false;
            
            // Skip first execution if it happened at startup without button press
            if (firstRun) {
                firstRun = false;
                Serial.println("First run - ignoring initial semaphore");
                continue;
            }
            
            Serial.println("Sync Task: Semaphore received!");
            
            // Increment the counter
            buttonPressCounter++;
            Serial.print("Button press count: ");
            Serial.println(buttonPressCounter);
            
            // Send N bytes (1, 2, 3, ... N) to the queue
            for (uint8_t i = 1; i <= buttonPressCounter; i++) {
                // Send byte to the front of the queue (LIFO behavior)
                if (xQueueSendToFront(byteQueue, &i, pdMS_TO_TICKS(100)) != pdPASS) {
                    Serial.println("Failed to send byte to queue");
                }
            }
            
            // Send a zero to indicate end of sequence
            uint8_t endMarker = 0;
            xQueueSendToFront(byteQueue, &endMarker, pdMS_TO_TICKS(100));
            
            // Blink the sync LED N times
            Serial.print("Blinking LED ");
            Serial.print(buttonPressCounter);
            Serial.println(" times");
            
            for (uint8_t i = 0; i < buttonPressCounter; i++) {
                // Turn ON LED
                digitalWrite(SYNC_LED_PIN, HIGH);
                Serial.println("Sync LED ON");
                vTaskDelay(pdMS_TO_TICKS(SYNC_LED_ON_TIME));
                
                // Turn OFF LED
                digitalWrite(SYNC_LED_PIN, LOW);
                Serial.println("Sync LED OFF");
                vTaskDelay(pdMS_TO_TICKS(SYNC_LED_OFF_TIME));
            }
            
            // Final delay before next cycle
            vTaskDelay(pdMS_TO_TICKS(SYNC_TASK_DELAY));
        }
    }
} 