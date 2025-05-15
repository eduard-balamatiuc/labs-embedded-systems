#ifndef PID_CONTROL_H
#define PID_CONTROL_H

// Initialize PID controller with tuning parameters
void PIDControl_Init(double kp, double ki, double kd);

// Set the target value (setpoint)
void PIDControl_SetSetpoint(double setpoint);

// Get the current setpoint
double PIDControl_GetSetpoint();

// Set the current input value (measured value)
void PIDControl_SetInput(double input);

// Compute the PID output based on current input and setpoint
double PIDControl_Compute();

#endif