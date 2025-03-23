#include <Arduino.h>
#include <Arduino_FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#include "config.h"

// External references to sync objects
extern SemaphoreHandle_t buttonSemaphore;

// Task function for button monitoring and LED control
void buttonLedTask(void *pvParameters) {
    // Add initialization message
    Serial.println("Button Task started");
    
    // Initial delay to ensure semaphore is created
    vTaskDelay(pdMS_TO_TICKS(500));
    
    // Debug the button pin configuration
    Serial.print("Button pin (GPIO ");
    Serial.print(BUTTON_PIN);
    Serial.println(") configured as INPUT_PULLUP");
    
    // Test the LED to make sure it works
    digitalWrite(BUTTON_LED_PIN, HIGH);
    vTaskDelay(pdMS_TO_TICKS(500));
    digitalWrite(BUTTON_LED_PIN, LOW);
    Serial.println("Button LED test complete");
    
    // Simple button state tracking
    bool previousButtonState = digitalRead(BUTTON_PIN); 
    bool currentButtonState;
    
    // For debouncing
    unsigned long lastDebounceTime = 0;
    
    // Main task loop
    while (true) {
        // Read current button state
        currentButtonState = digitalRead(BUTTON_PIN);
        
        // Check if state changed (potential button press)
        if (currentButtonState != previousButtonState) {
            // Reset debounce timer
            lastDebounceTime = millis();
        }
        
        // If state has been stable for debounce period
        if ((millis() - lastDebounceTime) > DEBOUNCE_DELAY) {
            // Check if button is pressed (LOW when using INPUT_PULLUP)
            if (currentButtonState == LOW && previousButtonState == HIGH) {
                // Button press detected (transition from released to pressed)
                Serial.println("Button pressed - turning ON LED");
                
                // Turn ON the button LED
                digitalWrite(BUTTON_LED_PIN, HIGH);
                
                // Signal Task 2 via semaphore
                if (xSemaphoreGive(buttonSemaphore) == pdTRUE) {
                    Serial.println("Semaphore given successfully");
                } else {
                    Serial.println("Failed to give semaphore");
                }
                
                // Keep LED on for exactly 1 second
                vTaskDelay(pdMS_TO_TICKS(BUTTON_LED_ON_TIME));
                
                // Turn OFF the button LED
                digitalWrite(BUTTON_LED_PIN, LOW);
                Serial.println("Button LED turned OFF after delay");
            }
        }
        
        // Save the current state for next comparison
        previousButtonState = currentButtonState;
        
        // Small delay before checking again
        vTaskDelay(pdMS_TO_TICKS(BUTTON_CHECK_DELAY));
    }
} 