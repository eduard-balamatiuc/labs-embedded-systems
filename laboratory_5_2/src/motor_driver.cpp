#include "../include/motor_driver.h"

// Pin variables
static uint8_t EN_PIN;
static uint8_t MC1_PIN;
static uint8_t MC2_PIN;

void initMotorDriver(uint8_t enablePin, uint8_t motorControl1, uint8_t motorControl2) {
  // Store pin values
  EN_PIN = enablePin;
  MC1_PIN = motorControl1;
  MC2_PIN = motorControl2;
  
  // Configure motor driver pins
  pinMode(EN_PIN, OUTPUT);
  pinMode(MC1_PIN, OUTPUT);
  pinMode(MC2_PIN, OUTPUT);
  
  // Initialize motor to stopped state
  brake();
}

void driveMotor(int speed) {
  // If speed is zero, brake the motor
  if (speed == 0) {
    brake();
    return;
  }
  
  // Set direction based on sign of speed
  if (speed > 0) {
    // Forward direction
    digitalWrite(MC1_PIN, HIGH);
    digitalWrite(MC2_PIN, LOW);
    analogWrite(EN_PIN, abs(speed));  // Apply PWM to enable pin
  } else {
    // Reverse direction
    digitalWrite(MC1_PIN, LOW);
    digitalWrite(MC2_PIN, HIGH);
    analogWrite(EN_PIN, abs(speed));  // Apply PWM to enable pin
  }
}

// Forward movement at specified rate
void forward(int rate) {
  digitalWrite(MC1_PIN, HIGH);
  digitalWrite(MC2_PIN, LOW);
  analogWrite(EN_PIN, rate);
}

// Reverse movement at specified rate
void reverse(int rate) {
  digitalWrite(MC1_PIN, LOW);
  digitalWrite(MC2_PIN, HIGH);
  analogWrite(EN_PIN, rate);
}

// Stop the motor with active braking
void brake() {
  // Apply active brake (both inputs HIGH or both LOW)
  digitalWrite(MC1_PIN, HIGH);
  digitalWrite(MC2_PIN, HIGH);
  analogWrite(EN_PIN, 255);  // Apply full power for braking
} 