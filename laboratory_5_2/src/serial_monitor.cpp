#include "serial_monitor.h"
#include <Arduino.h>

#define SERIAL_BAUD_RATE 9600

void SerialMonitor_Init() {
  Serial.begin(SERIAL_BAUD_RATE);
  while (!Serial) {
    ; // Wait for serial port to connect
  }
  delay(100);
}

void SerialMonitor_PrintMessage(const char* message) {
  Serial.println(message);
}

void SerialMonitor_ReportData(int setpoint, int rpm, int pwm) {
  // Format for Arduino Serial Plotter
  Serial.print("SetPoint:");
  Serial.print(setpoint);
  Serial.print(",RPM:");
  Serial.print(rpm);
  Serial.print(",PWM:");
  Serial.println(pwm);
}