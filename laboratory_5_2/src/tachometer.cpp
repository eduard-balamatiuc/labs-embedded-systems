#include "tachometer.h"
#include <Arduino.h>

// Constants
#define FILTER_SIZE 5 // Size of moving average filter
#define FAN_POLES 2   // Most PC fans have 2 poles (2 pulses per revolution)

// Variables
static int tachometerPin = 2;  // Default tachometer pin
static int currentRPM = 0;
static volatile unsigned long pulseCount = 0;
static unsigned long lastMeasurementTime = 0;
static const unsigned long measurementInterval = 1000; // 1 second

// Filter variables
static int rpmReadings[FILTER_SIZE];
static int readIndex = 0;
static int rpmTotal = 0;

// Interrupt handler for tachometer pulses
void countPulse() {
  pulseCount++;
}

void Tachometer_Init(int tachPin) {
  tachometerPin = tachPin;
  pinMode(tachometerPin, INPUT_PULLUP); 
  
  // Initialize the moving average filter
  for (int i = 0; i < FILTER_SIZE; i++) {
    rpmReadings[i] = 0;
  }
  rpmTotal = 0;
  readIndex = 0;
  
  // Attach interrupt to count pulses
  attachInterrupt(digitalPinToInterrupt(tachometerPin), countPulse, FALLING);
  
  printf("Tachometer initialized on pin %d\n", tachometerPin);
}

int Tachometer_GetRPM() {
  unsigned long currentTime = millis();
  
  // Update RPM measurement every measurementInterval
  if (currentTime - lastMeasurementTime >= measurementInterval) {
    // Calculate RPM: (pulses * 60) / (time in seconds * poles)
    // Detach interrupt temporarily to avoid race conditions
    detachInterrupt(digitalPinToInterrupt(tachometerPin));
    
    // Calculate RPM
    int rpm = (pulseCount * 60) / (FAN_POLES);
    
    // Reset pulse counter
    pulseCount = 0;
    
    // Reattach interrupt
    attachInterrupt(digitalPinToInterrupt(tachometerPin), countPulse, FALLING);
    
    // Update filter
    rpmTotal = rpmTotal - rpmReadings[readIndex];
    rpmReadings[readIndex] = rpm;
    rpmTotal = rpmTotal + rpm;
    readIndex = (readIndex + 1) % FILTER_SIZE;
    
    // Calculate filtered RPM
    currentRPM = rpmTotal / FILTER_SIZE;
    
    lastMeasurementTime = currentTime;
  }
  
  return currentRPM;
}