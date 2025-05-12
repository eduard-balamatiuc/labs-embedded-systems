#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include <Arduino.h>

// States for LED control
typedef enum {
    STATE_OFF = 0,
    STATE_ON = 1
} LedState;

// Initialize the state machine and set up pins
void initStateMachine(uint8_t buttonPin, uint8_t ledPin);

// Update the state machine (called in the main loop)
void updateStateMachine();

// Get the current state (for display)
LedState getCurrentState();

// Get a string representation of the current state
const char* getStateString();

#endif // STATE_MACHINE_H 