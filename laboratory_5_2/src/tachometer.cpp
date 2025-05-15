#include "tachometer.h"
#include "fan_control.h"
#include <Arduino.h>

// Constants
#define MIN_RPM 0
#define MAX_RPM 3000
#define MIN_PWM 60    // Minimum PWM where fan starts moving
#define MAX_PWM 255   // Maximum PWM value
#define FILTER_SIZE 3 // Size of moving average filter

// Variables
static int tachometerPin = 8;  // Default tachometer pin
static int currentRPM = 0;
static unsigned long lastUpdateTime = 0;
static int targetRPM = 0; // Target RPM based on current PWM

// Filter variables
static int rpmReadings[FILTER_SIZE];
static int readIndex = 0;
static int rpmTotal = 0;

void Tachometer_Init(int tachPin) {
  tachometerPin = tachPin;
  pinMode(tachometerPin, INPUT_PULLUP); // Keep this for potential future use
  
  // Initialize the moving average filter
  for (int i = 0; i < FILTER_SIZE; i++) {
    rpmReadings[i] = 0;
  }
  rpmTotal = 0;
  readIndex = 0;
  
  // Note: We're no longer using interrupts since we're simulating RPM
  
  Serial.println("Tachometer initialized (using simulated RPM based on PWM)");
}

int Tachometer_GetRPM() {
  // Get current PWM value from fan control
  int currentPWM = FanControl_GetSpeed();
  
  // Calculate target RPM based on PWM value
  if (currentPWM < MIN_PWM) {
    targetRPM = 0;
  } else {
    // Map PWM to RPM - use exponential-like curve for more realistic behavior
    // Formula: RPM = MIN_RPM + (MAX_RPM - MIN_RPM) * ((PWM - MIN_PWM) / (MAX_PWM - MIN_PWM))^1.2
    float normalizedPWM = (float)(currentPWM - MIN_PWM) / (float)(MAX_PWM - MIN_PWM);
    float scaleFactor = pow(normalizedPWM, 1.2); // Slightly non-linear response
    targetRPM = MIN_RPM + (int)((MAX_RPM - MIN_RPM) * scaleFactor);
  }
  
  // Gradually approach the target RPM to simulate fan inertia
  unsigned long currentTime = millis();
  if (currentTime - lastUpdateTime >= 100) { // Update every 100ms
    // Move current RPM 10% closer to target RPM each time
    if (abs(currentRPM - targetRPM) > 10) {
      currentRPM = currentRPM + (targetRPM - currentRPM) / 10;
    } else {
      currentRPM = targetRPM;
    }
    
    // Add small random fluctuation (±2%) to make it more realistic
    int fluctuation = random(-currentRPM / 50, currentRPM / 50);
    currentRPM += fluctuation;
    
    // Ensure RPM stays in valid range
    currentRPM = constrain(currentRPM, MIN_RPM, MAX_RPM);
    
    lastUpdateTime = currentTime;
  }
  
  // Debug output occasionally
  static unsigned long lastDebugTime = 0;
  if (millis() - lastDebugTime > 3000) { // Every 3 seconds
    char buffer[70];
    sprintf(buffer, "Sim RPM: %d (target: %d, PWM: %d)", currentRPM, targetRPM, currentPWM);
    Serial.println(buffer);
    lastDebugTime = millis();
  }
  
  return currentRPM;
}