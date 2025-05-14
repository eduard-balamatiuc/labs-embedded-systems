#include "traffic_light.h"
#include <stdio.h>
#include <task.h>

// Pin definitions for traffic lights
// East-West traffic light
#define EW_GREEN_PIN 5
#define EW_YELLOW_PIN 6
#define EW_RED_PIN 7

// North-South traffic light
#define NS_GREEN_PIN 8
#define NS_YELLOW_PIN 9
#define NS_RED_PIN 10

// Task handles
TaskHandle_t xTrafficControlTaskHandle = NULL;

// Global state for night mode
volatile bool nightModeActive = false;
volatile bool northSouthRequest = false;

// Current state of both traffic lights
TrafficLightState eastWestState = TRAFFIC_GREEN;
TrafficLightState northSouthState = TRAFFIC_RED;

// Task function prototypes
void vTrafficControlTask(void *pvParameters);

// Helper functions
void setTrafficLightState(TrafficDirection direction, TrafficLightState state);
void turnOffAllLights(TrafficDirection direction);
void handleNightMode();

// Initialize traffic light system
void initTrafficLights() {
    // Setup pins for East-West traffic light
    pinMode(EW_GREEN_PIN, OUTPUT);
    pinMode(EW_YELLOW_PIN, OUTPUT);
    pinMode(EW_RED_PIN, OUTPUT);
    
    // Setup pins for North-South traffic light
    pinMode(NS_GREEN_PIN, OUTPUT);
    pinMode(NS_YELLOW_PIN, OUTPUT);
    pinMode(NS_RED_PIN, OUTPUT);
    
    // Setup button pins
    pinMode(REQUEST_BUTTON_PIN, INPUT_PULLUP);
    pinMode(NIGHT_MODE_BUTTON_PIN, INPUT_PULLUP);
    
    // Initial state: East-West is green, North-South is red
    setTrafficLightState(DIRECTION_EAST_WEST, TRAFFIC_GREEN);
    setTrafficLightState(DIRECTION_NORTH_SOUTH, TRAFFIC_RED);
    
    printf("System initialized\n");
}

// Create and start traffic light tasks
void createTrafficLightTasks() {
    // Create a single task to control everything
    xTaskCreate(
        vTrafficControlTask,
        "TrafficControl",
        96,  // Reduced stack size
        NULL,
        1,
        &xTrafficControlTaskHandle
    );
    
    printf("Task created\n");
}

// Combined traffic control task
void vTrafficControlTask(void *pvParameters) {
    uint8_t lastRequestButtonState = HIGH;
    uint8_t lastNightModeButtonState = HIGH;
    TickType_t lastStateChangeTime = 0;
    TickType_t lastButtonCheckTime = 0;
    
    for (;;) {
        TickType_t currentTime = xTaskGetTickCount();
        
        // Check buttons every 50ms
        if (currentTime - lastButtonCheckTime >= pdMS_TO_TICKS(50)) {
            lastButtonCheckTime = currentTime;
            
            // Read button states
            uint8_t requestButtonState = digitalRead(REQUEST_BUTTON_PIN);
            uint8_t nightModeButtonState = digitalRead(NIGHT_MODE_BUTTON_PIN);
            
            // Handle request button
            if (requestButtonState != lastRequestButtonState) {
                if (requestButtonState == LOW && !northSouthRequest && 
                    eastWestState == TRAFFIC_GREEN && !nightModeActive) {
                    northSouthRequest = true;
                }
                lastRequestButtonState = requestButtonState;
            }
            
            // Handle night mode button
            if (nightModeButtonState != lastNightModeButtonState) {
                if (nightModeButtonState == LOW) {
                    nightModeActive = !nightModeActive;
                    
                    if (!nightModeActive) {
                        // Return to normal mode
                        eastWestState = TRAFFIC_GREEN;
                        northSouthState = TRAFFIC_RED;
                        setTrafficLightState(DIRECTION_EAST_WEST, TRAFFIC_GREEN);
                        setTrafficLightState(DIRECTION_NORTH_SOUTH, TRAFFIC_RED);
                        lastStateChangeTime = currentTime;
                    }
                }
                lastNightModeButtonState = nightModeButtonState;
            }
        }
        
        // Night mode handling
        if (nightModeActive) {
            static bool yellowOn = false;
            static TickType_t lastBlinkTime = 0;
            
            if (currentTime - lastBlinkTime >= pdMS_TO_TICKS(NIGHT_MODE_BLINK_INTERVAL)) {
                lastBlinkTime = currentTime;
                yellowOn = !yellowOn;
                
                // East-West yellow
                digitalWrite(EW_GREEN_PIN, LOW);
                digitalWrite(EW_YELLOW_PIN, yellowOn);
                digitalWrite(EW_RED_PIN, LOW);
                
                // North-South yellow
                digitalWrite(NS_GREEN_PIN, LOW);
                digitalWrite(NS_YELLOW_PIN, yellowOn);
                digitalWrite(NS_RED_PIN, LOW);
            }
            
            vTaskDelay(pdMS_TO_TICKS(20));
            continue;
        }
        
        // Normal traffic light control
        switch (eastWestState) {
            case TRAFFIC_GREEN:
                if (northSouthRequest && (currentTime - lastStateChangeTime >= pdMS_TO_TICKS(500))) {
                    eastWestState = TRAFFIC_YELLOW;
                    setTrafficLightState(DIRECTION_EAST_WEST, TRAFFIC_YELLOW);
                    lastStateChangeTime = currentTime;
                }
                break;
                
            case TRAFFIC_YELLOW:
                if (currentTime - lastStateChangeTime >= pdMS_TO_TICKS(YELLOW_TIME)) {
                    eastWestState = TRAFFIC_RED;
                    northSouthState = TRAFFIC_GREEN;
                    setTrafficLightState(DIRECTION_EAST_WEST, TRAFFIC_RED);
                    setTrafficLightState(DIRECTION_NORTH_SOUTH, TRAFFIC_GREEN);
                    northSouthRequest = false;
                    lastStateChangeTime = currentTime;
                }
                break;
                
            case TRAFFIC_RED:
                // Controlled by North-South state
                break;
        }
        
        switch (northSouthState) {
            case TRAFFIC_GREEN:
                if (currentTime - lastStateChangeTime >= pdMS_TO_TICKS(GREEN_TIME)) {
                    northSouthState = TRAFFIC_YELLOW;
                    setTrafficLightState(DIRECTION_NORTH_SOUTH, TRAFFIC_YELLOW);
                    lastStateChangeTime = currentTime;
                }
                break;
                
            case TRAFFIC_YELLOW:
                if (currentTime - lastStateChangeTime >= pdMS_TO_TICKS(YELLOW_TIME)) {
                    northSouthState = TRAFFIC_RED;
                    eastWestState = TRAFFIC_GREEN;
                    setTrafficLightState(DIRECTION_NORTH_SOUTH, TRAFFIC_RED);
                    setTrafficLightState(DIRECTION_EAST_WEST, TRAFFIC_GREEN);
                    lastStateChangeTime = currentTime;
                }
                break;
                
            case TRAFFIC_RED:
                // Controlled by East-West state
                break;
        }
        
        vTaskDelay(pdMS_TO_TICKS(20));
    }
}

// Set traffic light to specific state
void setTrafficLightState(TrafficDirection direction, TrafficLightState state) {
    // Get the pins for the specified direction
    uint8_t greenPin, yellowPin, redPin;
    
    switch (direction) {
        case DIRECTION_EAST_WEST:
            greenPin = EW_GREEN_PIN;
            yellowPin = EW_YELLOW_PIN;
            redPin = EW_RED_PIN;
            break;
            
        case DIRECTION_NORTH_SOUTH:
            greenPin = NS_GREEN_PIN;
            yellowPin = NS_YELLOW_PIN;
            redPin = NS_RED_PIN;
            break;
            
        default:
            return; // Invalid direction
    }
    
    // Turn off all lights first
    digitalWrite(greenPin, LOW);
    digitalWrite(yellowPin, LOW);
    digitalWrite(redPin, LOW);
    
    // Set the light according to the state
    switch (state) {
        case TRAFFIC_GREEN:
            digitalWrite(greenPin, HIGH);
            break;
            
        case TRAFFIC_YELLOW:
            digitalWrite(yellowPin, HIGH);
            break;
            
        case TRAFFIC_RED:
            digitalWrite(redPin, HIGH);
            break;
            
        case TRAFFIC_NIGHT_MODE:
            // Night mode is handled separately
            break;
    }
    
    // Print the state change (minimize)
    printf("%s: %s\n", getDirectionName(direction), getStateName(state));
}

// Get state name as string for display
const char* getStateName(TrafficLightState state) {
    switch (state) {
        case TRAFFIC_GREEN:
            return "GREEN";
        case TRAFFIC_YELLOW:
            return "YELLOW";
        case TRAFFIC_RED:
            return "RED";
        case TRAFFIC_NIGHT_MODE:
            return "NIGHT";
        default:
            return "UNK";
    }
}

// Get direction name as string for display
const char* getDirectionName(TrafficDirection direction) {
    switch (direction) {
        case DIRECTION_EAST_WEST:
            return "EW";
        case DIRECTION_NORTH_SOUTH:
            return "NS";
        default:
            return "UNK";
    }
} 