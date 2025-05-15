#include "user_interface.h"
#include "pid_control.h"
#include "fan_control.h"
#include "tachometer.h"
#include <Arduino.h>

#define MAX_RPM 3000
#define MIN_RPM 500
#define RPM_STEP 100

void UserInterface_Init() {
  // Initialize built-in LED for alarm
  pinMode(LED_BUILTIN, OUTPUT);
  
  // Display initial help message
  Serial.println("\n--- Fan Control System with Simulated RPM ---");
  Serial.println("Available commands:");
  Serial.println("  + : Increase target RPM");
  Serial.println("  - : Decrease target RPM");
  Serial.println("  d : Run diagnostic test");
  Serial.println("  p : Print current parameters");
  Serial.println("  h : Show this help message");
  Serial.println("---------------------------------------------");
}

void UserInterface_Update() {
  // Check for serial input to adjust setpoint
  if (Serial.available() > 0) {
    char input = Serial.read();
    
    // Current setpoint
    double currentSetpoint = PIDControl_GetSetpoint();
    
    // Adjust setpoint based on input
    if (input == '+') {
      // Increase setpoint
      double newSetpoint = min(currentSetpoint + RPM_STEP, (double)MAX_RPM);
      PIDControl_SetSetpoint(newSetpoint);
      
      // Notify user
      Serial.print("New setpoint: ");
      Serial.println(newSetpoint);
    }
    else if (input == '-') {
      // Decrease setpoint
      double newSetpoint = max(currentSetpoint - RPM_STEP, (double)MIN_RPM);
      PIDControl_SetSetpoint(newSetpoint);
      
      // Notify user
      Serial.print("New setpoint: ");
      Serial.println(newSetpoint);
    }
    else if (input == 'd' || input == 'D') {
      // Run diagnostic test - slowly ramp PWM up and down
      Serial.println("Starting diagnostic test...");
      
      // Save current PID output
      int savedPWM = FanControl_GetSpeed();
      
      // Ramp up slowly
      Serial.println("Ramping PWM up from 0 to 255:");
      for (int pwm = 0; pwm <= 255; pwm += 10) {
        FanControl_SetSpeed(pwm);
        delay(300);  // Give time for fan to respond
        int rpm = Tachometer_GetRPM();
        Serial.print("PWM: ");
        Serial.print(pwm);
        Serial.print(", Simulated RPM: ");
        Serial.println(rpm);
      }
      
      delay(1000);  // Hold at max
      
      // Ramp down slowly
      Serial.println("Ramping PWM down from 255 to 0:");
      for (int pwm = 255; pwm >= 0; pwm -= 10) {
        FanControl_SetSpeed(pwm);
        delay(300);  // Give time for fan to respond
        int rpm = Tachometer_GetRPM();
        Serial.print("PWM: ");
        Serial.print(pwm);
        Serial.print(", Simulated RPM: ");
        Serial.println(rpm);
      }
      
      // Restore original PWM
      Serial.println("Diagnostic test complete, resuming normal operation");
      FanControl_SetSpeed(savedPWM);
    }
    else if (input == 'p' || input == 'P') {
      // Print current parameters
      Serial.println("Current system parameters:");
      Serial.print("Setpoint: ");
      Serial.print(PIDControl_GetSetpoint());
      Serial.print(" RPM, Current Simulated RPM: ");
      Serial.print(Tachometer_GetRPM());
      Serial.print(", PWM: ");
      Serial.println(FanControl_GetSpeed());
    }
    else if (input == 'h' || input == 'H') {
      // Show help message
      Serial.println("\n--- Fan Control System with Simulated RPM ---");
      Serial.println("Available commands:");
      Serial.println("  + : Increase target RPM");
      Serial.println("  - : Decrease target RPM");
      Serial.println("  d : Run diagnostic test");
      Serial.println("  p : Print current parameters");
      Serial.println("  h : Show this help message");
      Serial.println("---------------------------------------------");
    }
  }
}