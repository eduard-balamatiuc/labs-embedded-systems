#ifndef PID_CONTROLLER_H
#define PID_CONTROLLER_H

#include <Arduino.h>

// Initialize the PID controller with constants
void initPIDController(double kp, double ki, double kd, int deadzone_value);

// Update PID controller with new setpoint and current position
void updatePID(double newSetPoint, double newCurrentPosition);

// Calculate PID output based on current values
void calculatePID();

// Get the current motor speed
int getMotorSpeed();

// Get current PID values for display/debugging
double getSetPoint();
double getCurrentPosition();
double getError();

#endif // PID_CONTROLLER_H 