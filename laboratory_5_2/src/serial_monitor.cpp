#include "serial_monitor.h"
#include <Arduino.h>
#include <stdio.h>

#define SERIAL_BAUD_RATE 9600

// Setup FILE streams for stdin and stdout
FILE uart_output;
FILE uart_input;

// Function to redirect printf to Serial
int uart_putchar(char c, FILE *stream) {
  Serial.write(c);
  return 0;
}

// Function to redirect stdin to Serial
int uart_getchar(FILE *stream) {
  while (!Serial.available());
  return Serial.read();
}

void SerialMonitor_Init() {
  // Initialize serial communication
  Serial.begin(SERIAL_BAUD_RATE);
  while (!Serial) {
    ; // Wait for serial port to connect
  }
  
  // Initialize stdio
  fdev_setup_stream(&uart_output, uart_putchar, NULL, _FDEV_SETUP_WRITE);
  fdev_setup_stream(&uart_input, NULL, uart_getchar, _FDEV_SETUP_READ);
  stdout = &uart_output;
  stdin = &uart_input;
  
  delay(100);
}

void SerialMonitor_PrintMessage(const char* message) {
  printf("%s\n", message);
}

void SerialMonitor_ReportData(int setpoint, int rpm, int pwm) {
  // Format for Arduino Serial Plotter
  printf("SetPoint:%d,RPM:%d,PWM:%d\n", setpoint, rpm, pwm);
}