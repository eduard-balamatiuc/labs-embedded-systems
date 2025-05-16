#include <Arduino.h>
#include <stdio.h>
#include "pid_control.h"
#include "fan_control.h"
#include "tachometer.h"
#include "serial_monitor.h"
#include "user_interface.h"

// Pin definitions
#define PWM_PIN 9      // Blue wire (PWM)
#define TACH_PIN 2     // Yellow wire (tachometer)
#define ALARM_LED_PIN LED_BUILTIN

// PID and fan control parameters
#define INITIAL_SETPOINT 1500    // Initial target RPM
#define INITIAL_PWM_VALUE 128    // Initial PWM value (50%)
#define REPORT_INTERVAL 500      // Report data every 500ms
#define MAX_RPM_DEVIATION 300    // Maximum allowed RPM deviation before alarm

void setup() {
  // Initialize serial communication with stdio support
  SerialMonitor_Init();
  printf("Fan PID Control System Started\n");
  
  // Set up LED pin for alarm
  pinMode(ALARM_LED_PIN, OUTPUT);
  
  // Initialize fan control with PWM
  FanControl_Init(PWM_PIN);
  
  // Initialize tachometer for RPM measurement
  Tachometer_Init(TACH_PIN);
  
  // Initialize PID controller with tuning parameters
  PIDControl_Init(0.5, 0.2, 0.05);  // kp, ki, kd
  PIDControl_SetSetpoint(INITIAL_SETPOINT);
  
  // Initialize user interface
  UserInterface_Init();
  
  // Start fan at 50% speed and wait for it to start
  printf("Setting initial fan speed to 50%%\n");
  FanControl_SetSpeed(INITIAL_PWM_VALUE);
  
  // Allow time for the fan to start
  delay(2000);
}

void loop() {
  // Get current time
  unsigned long currentTime = millis();
  
  // Handle user interface (setpoint adjustments)
  UserInterface_Update();
  
  // Read current RPM from tachometer
  int currentRPM = Tachometer_GetRPM();
  
  // Update PID controller with current RPM
  PIDControl_SetInput(currentRPM);
  double pidOutput = PIDControl_Compute();
  
  // Set fan speed based on PID output
  FanControl_SetSpeed((int)pidOutput);
  
  // Check for large deviation and set alarm if needed
  int setpoint = PIDControl_GetSetpoint();
  if (abs(currentRPM - setpoint) > MAX_RPM_DEVIATION) {
    // Deviation too large, set alarm
    digitalWrite(ALARM_LED_PIN, HIGH);
  } else {
    digitalWrite(ALARM_LED_PIN, LOW);
  }
  
  // Report data periodically
  static unsigned long lastReportTime = 0;
  if (currentTime - lastReportTime >= REPORT_INTERVAL) {
    SerialMonitor_ReportData(setpoint, currentRPM, (int)pidOutput);
    lastReportTime = currentTime;
  }
}