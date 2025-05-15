#ifndef SERIAL_MONITOR_H
#define SERIAL_MONITOR_H

// Initialize serial communication
void SerialMonitor_Init();

// Print a message to serial
void SerialMonitor_PrintMessage(const char* message);

// Report data for plotting
void SerialMonitor_ReportData(int setpoint, int rpm, int pwm);

#endif