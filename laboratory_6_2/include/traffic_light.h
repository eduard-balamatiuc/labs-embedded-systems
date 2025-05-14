#ifndef TRAFFIC_LIGHT_H
#define TRAFFIC_LIGHT_H

#include <Arduino.h>
#include <Arduino_FreeRTOS.h>

// Traffic light states
typedef enum {
    TRAFFIC_GREEN,
    TRAFFIC_YELLOW,
    TRAFFIC_RED,
    TRAFFIC_NIGHT_MODE
} TrafficLightState;

// Traffic light directions
typedef enum {
    DIRECTION_EAST_WEST,
    DIRECTION_NORTH_SOUTH
} TrafficDirection;

// State timing configuration (in milliseconds)
#define GREEN_TIME 5000
#define YELLOW_TIME 2000
#define RED_TIME 7000  // Equal to GREEN_TIME + YELLOW_TIME of other direction
#define NIGHT_MODE_BLINK_INTERVAL 500

// Button pins
#define REQUEST_BUTTON_PIN 2
#define NIGHT_MODE_BUTTON_PIN 3

// Initialize traffic light system
void initTrafficLights();

// Get state name as string for display
const char* getStateName(TrafficLightState state);

// Get direction name as string for display
const char* getDirectionName(TrafficDirection direction);

// Create tasks for the traffic light system
void createTrafficLightTasks();

// External variable for night mode state
extern volatile bool nightModeActive;

#endif // TRAFFIC_LIGHT_H 