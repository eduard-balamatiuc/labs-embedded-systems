#include <Arduino.h>
#include <Arduino_FreeRTOS.h>
#include <semphr.h>
#include <queue.h>
#include "config.h"

// Binary semaphore for synchronization between Task 1 and Task 2
SemaphoreHandle_t buttonSemaphore = NULL;

// Global queue for communication between Task 2 and Task 3
QueueHandle_t byteQueue = NULL;

// Function to initialize all synchronization objects
void initSyncObjects() {
    // Create binary semaphore
    Serial.println("Creating binary semaphore...");
    
    // Create the semaphore - will be in "empty" state initially
    buttonSemaphore = xSemaphoreCreateBinary();
    
    if (buttonSemaphore == NULL) {
        Serial.println("ERROR: Failed to create button semaphore");
    } else {
        Serial.println("Button semaphore created successfully");
        
        // Binary semaphores are created empty, so we don't need to explicitly take it
        // However, we'll try to take it just to be sure it's empty
        if (xSemaphoreTake(buttonSemaphore, 0) == pdTRUE) {
            Serial.println("WARNING: Semaphore was not empty as expected, now taken");
        } else {
            Serial.println("Semaphore confirmed to be in empty state");
        }
        
        // Important: we DO NOT want to give the semaphore here, as this would 
        // trigger Task 2 immediately without a button press
    }
    
    // Create queue for byte communication
    Serial.println("Creating byte queue...");
    byteQueue = xQueueCreate(QUEUE_SIZE, sizeof(uint8_t));
    
    if (byteQueue == NULL) {
        Serial.println("ERROR: Failed to create byte queue");
    } else {
        Serial.println("Byte queue created successfully");
        
        // Clear the queue just to be sure it's empty
        xQueueReset(byteQueue);
        Serial.println("Queue reset to empty state");
    }
    
    if (buttonSemaphore != NULL && byteQueue != NULL) {
        Serial.println("Synchronization objects initialized successfully");
    } else {
        Serial.println("WARNING: Some synchronization objects failed to initialize");
    }
} 