#ifndef MOTOR_DRIVER_H
#define MOTOR_DRIVER_H

#include <Arduino.h>

// Initialize the motor driver pins
void initMotorDriver(uint8_t enablePin, uint8_t motorControl1, uint8_t motorControl2);

// Drive the motor with specific speed (positive for forward, negative for reverse)
void driveMotor(int speed);

// Forward movement at specified rate
void forward(int rate);

// Reverse movement at specified rate
void reverse(int rate);

// Stop the motor with active braking
void brake();

#endif // MOTOR_DRIVER_H 