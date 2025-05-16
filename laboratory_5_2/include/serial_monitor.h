#ifndef SERIAL_MONITOR_H
#define SERIAL_MONITOR_H

#include <Arduino.h>
#include <stdio.h>

// Initialize serial communication with stdio support
void SerialMonitor_Init();

// Print a message to serial
void SerialMonitor_PrintMessage(const char* message);

// Report data for plotting
void SerialMonitor_ReportData(int setpoint, int rpm, int pwm);

// Setup FILE streams for stdin and stdout
extern FILE uart_output;
extern FILE uart_input;

#endif