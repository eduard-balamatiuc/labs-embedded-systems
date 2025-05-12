#include "state_machine.h"

// Pin definitions
static uint8_t _buttonPin;
static uint8_t _ledPin;

// State machine variables
static LedState _currentState = STATE_OFF;
static unsigned long _lastDebounceTime = 0;
static const unsigned long _debounceDelay = 50; // debounce time in ms
static int _lastButtonState = HIGH; // assuming pull-up resistor
static int _buttonState = HIGH;     // assuming pull-up resistor

void initStateMachine(uint8_t buttonPin, uint8_t ledPin) {
    _buttonPin = buttonPin;
    _ledPin = ledPin;
    
    // Initialize pins
    pinMode(_buttonPin, INPUT_PULLUP);
    pinMode(_ledPin, OUTPUT);
    
    // Set initial LED state
    digitalWrite(_ledPin, LOW); // LED starts OFF
}

void updateStateMachine() {
    // Read the button state
    int reading = digitalRead(_buttonPin);
    
    // Check if button state changed
    if (reading != _lastButtonState) {
        // Reset debounce timer
        _lastDebounceTime = millis();
    }
    
    // Check if enough time has passed for debounce
    if ((millis() - _lastDebounceTime) > _debounceDelay) {
        // If the button state has changed
        if (reading != _buttonState) {
            _buttonState = reading;
            
            // If the button is pressed (LOW due to pull-up)
            if (_buttonState == LOW) {
                // Toggle the state
                if (_currentState == STATE_OFF) {
                    _currentState = STATE_ON;
                    digitalWrite(_ledPin, HIGH);
                } else {
                    _currentState = STATE_OFF;
                    digitalWrite(_ledPin, LOW);
                }
            }
        }
    }
    
    // Save the reading for next comparison
    _lastButtonState = reading;
}

LedState getCurrentState() {
    return _currentState;
}

const char* getStateString() {
    return (_currentState == STATE_OFF) ? "OFF" : "ON";
} 