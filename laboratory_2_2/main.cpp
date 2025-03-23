#include <Arduino.h>
#include <Arduino_FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#include <queue.h>
#include "config.h"
#include "tasks.h"

// Task handles
TaskHandle_t buttonTaskHandle = NULL;
TaskHandle_t syncTaskHandle = NULL;
TaskHandle_t asyncTaskHandle = NULL;

void setup() {
    // Initialize GPIO pins first
    pinMode(BUTTON_PIN, INPUT_PULLUP);  // Button with pull-up resistor
    pinMode(BUTTON_LED_PIN, OUTPUT);    // Button detection LED
    pinMode(SYNC_LED_PIN, OUTPUT);      // Synchronization task LED
    
    // Initialize LEDs to OFF state
    digitalWrite(BUTTON_LED_PIN, LOW);
    digitalWrite(SYNC_LED_PIN, LOW);
    
    // Blink LED once to show we're starting
    digitalWrite(BUTTON_LED_PIN, HIGH);
    delay(2000);
    digitalWrite(BUTTON_LED_PIN, LOW);
    
    // Initialize serial communication at 9600 baud - more compatible across boards
    Serial.begin(9600);
    delay(3000); // Longer delay to ensure serial is ready
    
    // Send simple test message
    Serial.println(".");
    Serial.println(".");
    Serial.println("Starting setup...");
    Serial.println("FreeRTOS Multi-Task Application");
    
    Serial.println("GPIO pins initialized");
    
    // Initialize synchronization objects (semaphore and queue)
    initSyncObjects();
    
    Serial.println("Creating Button Task...");
    // Create FreeRTOS tasks
    BaseType_t buttonTaskCreated = xTaskCreate(
        buttonLedTask,           // Task function
        "ButtonLedTask",         // Task name
        BUTTON_TASK_STACK_SIZE,  // Stack size
        NULL,                    // Task parameters
        BUTTON_TASK_PRIORITY,    // Task priority
        &buttonTaskHandle        // Task handle
    );
    
    if (buttonTaskCreated != pdPASS) {
        Serial.println("ERROR: Failed to create Button Task");
    } else {
        Serial.println("Button Task created successfully");
    }
    
    Serial.println("Creating Sync Task...");
    BaseType_t syncTaskCreated = xTaskCreate(
        syncTask,                // Task function
        "SyncTask",              // Task name
        SYNC_TASK_STACK_SIZE,    // Stack size
        NULL,                    // Task parameters
        SYNC_TASK_PRIORITY,      // Task priority
        &syncTaskHandle          // Task handle
    );
    
    if (syncTaskCreated != pdPASS) {
        Serial.println("ERROR: Failed to create Sync Task");
    } else {
        Serial.println("Sync Task created successfully");
    }
    
    Serial.println("Creating Async Task...");
    BaseType_t asyncTaskCreated = xTaskCreate(
        asyncTask,               // Task function
        "AsyncTask",             // Task name
        ASYNC_TASK_STACK_SIZE,   // Stack size
        NULL,                    // Task parameters
        ASYNC_TASK_PRIORITY,     // Task priority
        &asyncTaskHandle         // Task handle
    );
    
    if (asyncTaskCreated != pdPASS) {
        Serial.println("ERROR: Failed to create Async Task");
    } else {
        Serial.println("Async Task created successfully");
    }
    
    if (buttonTaskCreated == pdPASS && syncTaskCreated == pdPASS && asyncTaskCreated == pdPASS) {
        Serial.println("All tasks created successfully");
    } else {
        Serial.println("WARNING: Some tasks failed to create");
    }
    
    // Additional mutex for optional enhancement (mutex to protect serial output)
    // This prevents task outputs from overlapping in the serial console
    SemaphoreHandle_t serialMutex = xSemaphoreCreateMutex();
    if (serialMutex != NULL) {
        Serial.println("Serial mutex created for output protection");
    } else {
        Serial.println("Failed to create serial mutex");
    }
    
    Serial.println("Setup complete, starting scheduler");
}

void loop() {
    // Empty loop as tasks handle everything
    vTaskDelay(pdMS_TO_TICKS(1000));
} 