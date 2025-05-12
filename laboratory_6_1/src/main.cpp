#include <Arduino.h>
#include "uart_helpers.h"
#include "state_machine.h"

// Pin definitions
#define BUTTON_PIN  2   // Button connected to digital pin 2
#define LED_PIN     13  // LED connected to digital pin 13 (built-in LED)

// Timing for status updates
unsigned long lastStatusUpdate = 0;
const unsigned long updateInterval = 500; // Print status every 500ms

void setup() {
  // Initialize UART for printf
  setup_uart_stdio();
  
  // Initialize the state machine
  initStateMachine(BUTTON_PIN, LED_PIN);
  
  // Print welcome message
  printf("FSM LED Control System\n");
  printf("Press the button to toggle LED state\n");
}

void loop() {
  // Update the state machine (check for button presses)
  updateStateMachine();
  
  // Print status periodically
  unsigned long currentMillis = millis();
  if (currentMillis - lastStatusUpdate >= updateInterval) {
    lastStatusUpdate = currentMillis;
    printf("Current state: %s\n", getStateString());
  }
}
