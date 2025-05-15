#include <Arduino.h>
#include "pid_control.h"
#include "fan_control.h"
#include "tachometer.h"
#include "serial_monitor.h"
#include "user_interface.h"

void setup() {
  // Initialize serial communication
  SerialMonitor_Init();
  SerialMonitor_PrintMessage("Fan PID Control System Started");
  
  // Initialize fan control with 25kHz PWM frequency
  FanControl_Init(9);  // PWM pin 9
  
  // Initialize tachometer (now using simulated RPM)
  Tachometer_Init(8);  // Tach pin 8
  
  // Initialize PID controller with tuning parameters
  PIDControl_Init(0.5, 0.2, 0.05);  // kp, ki, kd
  PIDControl_SetSetpoint(1500);     // Initial setpoint (RPM)
  
  // Initialize user interface
  UserInterface_Init();
  
  // Start fan at 50% speed and wait for it to stabilize
  SerialMonitor_PrintMessage("Setting initial fan speed to 50%");
  FanControl_SetSpeed(128);
  
  // Allow time for the simulated RPM to stabilize
  delay(1000);
}

void loop() {
  // Get current time
  unsigned long currentTime = millis();
  
  // Handle user interface (setpoint adjustments)
  UserInterface_Update();
  
  // Update RPM reading from tachometer (simulated based on PWM)
  int currentRPM = Tachometer_GetRPM();
  
  // Update PID controller
  PIDControl_SetInput(currentRPM);
  double pidOutput = PIDControl_Compute();
  
  // Set fan speed based on PID output
  FanControl_SetSpeed((int)pidOutput);
  
  // Check for deviation and set alarm if needed
  if (abs(currentRPM - PIDControl_GetSetpoint()) > 300) {
    // Deviation too large, set alarm
    digitalWrite(LED_BUILTIN, HIGH);
  } else {
    digitalWrite(LED_BUILTIN, LOW);
  }
  
  // Report data periodically
  static unsigned long lastReportTime = 0;
  if (currentTime - lastReportTime >= 500) {
    SerialMonitor_ReportData(PIDControl_GetSetpoint(), currentRPM, (int)pidOutput);
    lastReportTime = currentTime;
  }
}