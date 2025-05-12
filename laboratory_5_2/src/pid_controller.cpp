#include "../include/pid_controller.h"
#include "../include/motor_driver.h"

// PID Variables
static double setPoint = 0;         // Desired position
static double currentPosition = 0;  // Current measured position
static double error = 0;            // Difference between setPoint and currentPosition
static double lastError = 0;        // Previous error for derivative calculation
static double sumError = 0;         // Sum of errors for integral calculation

// PID Constants
static double KP = 2.0;       // Proportional gain
static double KI = 0.01;      // Integral gain
static double KD = 0.5;       // Derivative gain

// Control Variables
static int motorSpeed = 0;          // Speed to apply to motor (0-255)
static unsigned long lastTime = 0;  // For calculating delta time
static double deltaTime = 0;        // Time difference between iterations
static int deadzone = 5;            // Deadzone to prevent motor oscillation

void initPIDController(double kp, double ki, double kd, int deadzone_value) {
  KP = kp;
  KI = ki;
  KD = kd;
  deadzone = deadzone_value;
  
  // Initialize timing
  lastTime = millis();
}

void updatePID(double newSetPoint, double newCurrentPosition) {
  // Calculate time elapsed since last iteration
  unsigned long currentTime = millis();
  deltaTime = (currentTime - lastTime) / 1000.0;  // Convert to seconds
  lastTime = currentTime;
  
  // Update values
  setPoint = newSetPoint;
  currentPosition = newCurrentPosition;
  
  // Calculate PID output
  calculatePID();
}

void calculatePID() {
  // Calculate error
  error = setPoint - currentPosition;
  
  // If error is within deadzone, stop motor to prevent oscillation
  if (abs(error) <= deadzone) {
    motorSpeed = 0;
    brake();
    return;
  }
  
  // Calculate the integral term with anti-windup (limit the sum)
  sumError += error * deltaTime;
  // Anti-windup - limit the sum to prevent excessive buildup
  sumError = constrain(sumError, -100, 100);
  
  // Calculate derivative term
  double derivative = (error - lastError) / deltaTime;
  lastError = error;
  
  // Calculate PID output
  double output = (KP * error) + (KI * sumError) + (KD * derivative);
  
  // Constrain output to valid motor speed range (-255 to 255)
  // Sign indicates direction (positive = forward, negative = reverse)
  motorSpeed = constrain(output, -255, 255);
}

int getMotorSpeed() {
  return motorSpeed;
}

double getSetPoint() {
  return setPoint;
}

double getCurrentPosition() {
  return currentPosition;
}

double getError() {
  return error;
} 