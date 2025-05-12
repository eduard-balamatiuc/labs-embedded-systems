#include <Arduino.h>
#include "../include/uart_helpers.h"
#include "../include/motor_driver.h"
#include "../include/pid_controller.h"

// L298N Motor Driver Pins
#define EN 9   // Enable pin (PWM)
#define MC1 3  // Motor Control 1
#define MC2 2  // Motor Control 2

// Sensor Pins
#define POT_FEEDBACK 0  // Potentiometer connected to motor (feedback) on A0
#define POT_SETPOINT 1  // Potentiometer for setting desired position on A1

// PID Constants - you may need to tune these values
#define KP 2.0       // Proportional gain
#define KI 0.01      // Integral gain
#define KD 0.5       // Derivative gain
#define DEADZONE 5   // Deadzone to prevent motor oscillation

// Serial plotting refresh rate
#define SERIAL_REFRESH 100  // Refresh rate in ms
unsigned long lastRefresh = 0;

void setup() {
  // Initialize UART for printf
  setup_uart_stdio();
  
  // Initialize motor driver
  initMotorDriver(EN, MC1, MC2);
  
  // Initialize PID controller
  initPIDController(KP, KI, KD, DEADZONE);
  
  // Print header for serial plotter
  printf("PID Position Control System - Variant D\n");
  printf("SetPoint,CurrentPosition,MotorOutput\n");
}

void loop() {
  // Read the setpoint from potentiometer
  double setPoint = analogRead(POT_SETPOINT);
  
  // Read current position from feedback potentiometer
  double currentPosition = analogRead(POT_FEEDBACK);
  
  // Update PID controller
  updatePID(setPoint, currentPosition);
  
  // Drive motor with calculated speed
  driveMotor(getMotorSpeed());
  
  // Send data to Serial Plotter at regular intervals
  unsigned long currentTime = millis();
  if (currentTime - lastRefresh >= SERIAL_REFRESH) {
    printf("%.2f,%.2f,%d\n", getSetPoint(), getCurrentPosition(), getMotorSpeed());
    lastRefresh = currentTime;
  }
} 