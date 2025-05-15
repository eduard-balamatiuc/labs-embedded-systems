#ifndef FAN_CONTROL_H
#define FAN_CONTROL_H

// Initialize fan control
void FanControl_Init(int pwmPin);

// Set fan speed (PWM value 0-255)
void FanControl_SetSpeed(int speed);

// Get current fan speed setting
int FanControl_GetSpeed();

#endif