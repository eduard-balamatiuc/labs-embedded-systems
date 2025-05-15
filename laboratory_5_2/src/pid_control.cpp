#include "pid_control.h"
#include <Arduino.h>

// PID variables
static double kp = 0.5;      // Proportional gain
static double ki = 0.2;      // Integral gain
static double kd = 0.05;     // Derivative gain
static double input = 0;     // Current input value (measured RPM)
static double setpoint = 1500; // Target value (desired RPM)
static double lastError = 0; // Previous error for derivative
static double integral = 0;  // Accumulated error for integral
static double output = 128;  // PID output - start at 50% duty cycle
static unsigned long lastTime = 0; // Last computation time

void PIDControl_Init(double p, double i, double d) {
  kp = p;
  ki = i;
  kd = d;
  input = 0;
  lastError = 0;
  integral = 0;
  output = 128; // Start at midpoint
  lastTime = millis();
}

void PIDControl_SetSetpoint(double sp) {
  setpoint = sp;
  // Reset integral term when setpoint changes to avoid overshooting
  integral = 0;
}

double PIDControl_GetSetpoint() {
  return setpoint;
}

void PIDControl_SetInput(double in) {
  input = in;
}

double PIDControl_Compute() {
  // Calculate time since last computation
  unsigned long now = millis();
  double timeChange = (double)(now - lastTime) / 1000.0; // Convert to seconds
  
  // Don't compute if no time has passed
  if (timeChange < 0.01) {
    return output;
  }
  
  // Calculate error
  double error = setpoint - input;
  
  // Calculate integral with anti-windup
  integral += error * timeChange;
  
  // Apply stricter limits to integral to prevent windup
  if (integral > 50) integral = 50;
  if (integral < -50) integral = -50;
  
  // Calculate derivative
  double derivative = 0;
  if (timeChange > 0) {
    derivative = (error - lastError) / timeChange;
  }
  
  // Calculate PID components
  double pTerm = kp * error;
  double iTerm = ki * integral;
  double dTerm = kd * derivative;
  
  // Calculate new output
  double newOutput = pTerm + iTerm + dTerm;
  
  // With simulated RPM, we can map this directly to PWM
  // Map from RPM error to PWM scale
  double pwmAdjustment = newOutput * 255.0 / 3000.0; // Scale based on max RPM
  
  // Apply smooth adjustment - 15% of change per cycle
  output += pwmAdjustment * 0.15;
  
  // Apply limits to output
  if (output > 255) output = 255;
  if (output < 60) output = 60;  // Minimum threshold to ensure fan runs
  
  // Occasionally print PID details for debugging
  static unsigned long lastDebugTime = 0;
  if (now - lastDebugTime > 5000) { // Every 5 seconds
    char buffer[120];
    sprintf(buffer, "PID Details - Error: %.1f, P: %.1f, I: %.1f, D: %.1f, Out: %.1f", 
            error, pTerm, iTerm, dTerm, output);
    Serial.println(buffer);
    lastDebugTime = now;
  }
  
  // Save values for next computation
  lastError = error;
  lastTime = now;
  
  return output;
}