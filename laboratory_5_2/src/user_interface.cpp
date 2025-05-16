#include "user_interface.h"
#include "pid_control.h"
#include "fan_control.h"
#include "tachometer.h"
#include <Arduino.h>
#include <stdio.h>

#define MAX_RPM 3000
#define MIN_RPM 500
#define RPM_STEP 100
#define COMMAND_BUFFER_SIZE 10

// Buffer for parsing commands
static char commandBuffer[COMMAND_BUFFER_SIZE];
static int bufferPos = 0;

void UserInterface_Init() {
  // Initialize built-in LED for alarm
  pinMode(LED_BUILTIN, OUTPUT);
  
  // Display initial help message
  printf("\n--- Fan Control System ---\n");
  printf("Available commands:\n");
  printf("  + : Increase target RPM by 100\n");
  printf("  - : Decrease target RPM by 100\n");
  printf("  rXXXX : Set target RPM to XXXX (e.g., r2000)\n");
  printf("  d : Run diagnostic test\n");
  printf("  p : Print current parameters\n");
  printf("  h : Show this help message\n");
  printf("---------------------------------------------\n");
}

void UserInterface_Update() {
  // Check for serial input to adjust setpoint
  while (Serial.available() > 0) {
    char input = Serial.read();
    
    // Simple single-character commands
    if (input == '+' || input == '-' || input == 'd' || input == 'D' || input == 'p' || input == 'P' || input == 'h' || input == 'H') {
      // Reset command buffer
      bufferPos = 0;
      memset(commandBuffer, 0, COMMAND_BUFFER_SIZE);
      
      // Current setpoint
      double currentSetpoint = PIDControl_GetSetpoint();
      
      // Adjust setpoint based on input
      if (input == '+') {
        // Increase setpoint
        double newSetpoint = min(currentSetpoint + RPM_STEP, (double)MAX_RPM);
        PIDControl_SetSetpoint(newSetpoint);
        
        // Notify user
        printf("New setpoint: %.0f RPM\n", newSetpoint);
      }
      else if (input == '-') {
        // Decrease setpoint
        double newSetpoint = max(currentSetpoint - RPM_STEP, (double)MIN_RPM);
        PIDControl_SetSetpoint(newSetpoint);
        
        // Notify user
        printf("New setpoint: %.0f RPM\n", newSetpoint);
      }
      else if (input == 'd' || input == 'D') {
        // Run diagnostic test - slowly ramp PWM up and down
        printf("Starting diagnostic test...\n");
        
        // Save current PID output
        int savedPWM = FanControl_GetSpeed();
        
        // Ramp up slowly
        printf("Ramping PWM up from 0 to 255:\n");
        for (int pwm = 0; pwm <= 255; pwm += 10) {
          FanControl_SetSpeed(pwm);
          delay(300);  // Give time for fan to respond
          int rpm = Tachometer_GetRPM();
          printf("PWM: %d, RPM: %d\n", pwm, rpm);
        }
        
        delay(1000);  // Hold at max
        
        // Ramp down slowly
        printf("Ramping PWM down from 255 to 0:\n");
        for (int pwm = 255; pwm >= 0; pwm -= 10) {
          FanControl_SetSpeed(pwm);
          delay(300);  // Give time for fan to respond
          int rpm = Tachometer_GetRPM();
          printf("PWM: %d, RPM: %d\n", pwm, rpm);
        }
        
        // Restore original PWM
        printf("Diagnostic test complete, resuming normal operation\n");
        FanControl_SetSpeed(savedPWM);
      }
      else if (input == 'p' || input == 'P') {
        // Print current parameters
        printf("Current system parameters:\n");
        printf("Setpoint: %.0f RPM, Current RPM: %d, PWM: %d\n", 
               PIDControl_GetSetpoint(), Tachometer_GetRPM(), FanControl_GetSpeed());
      }
      else if (input == 'h' || input == 'H') {
        // Show help message
        printf("\n--- Fan Control System ---\n");
        printf("Available commands:\n");
        printf("  + : Increase target RPM by 100\n");
        printf("  - : Decrease target RPM by 100\n");
        printf("  rXXXX : Set target RPM to XXXX (e.g., r2000)\n");
        printf("  d : Run diagnostic test\n");
        printf("  p : Print current parameters\n");
        printf("  h : Show this help message\n");
        printf("---------------------------------------------\n");
      }
    }
    // Command that requires parsing (r for direct RPM setting)
    else {
      // Add to buffer if not newline or carriage return
      if (input != '\n' && input != '\r') {
        if (bufferPos < COMMAND_BUFFER_SIZE - 1) {
          commandBuffer[bufferPos++] = input;
          commandBuffer[bufferPos] = '\0';
        }
      }
      // Process the command when newline is received
      else if (bufferPos > 0) {
        // Process RPM setting command
        if (commandBuffer[0] == 'r' || commandBuffer[0] == 'R') {
          // Parse the number after 'r'
          int targetRpm = atoi(commandBuffer + 1);
          
          // Ensure it's within valid range
          if (targetRpm >= MIN_RPM && targetRpm <= MAX_RPM) {
            PIDControl_SetSetpoint(targetRpm);
            printf("Target RPM set to: %d\n", targetRpm);
          } else {
            printf("Invalid RPM value. Please use a value between %d and %d\n", MIN_RPM, MAX_RPM);
          }
        }
        
        // Reset buffer after processing
        bufferPos = 0;
        memset(commandBuffer, 0, COMMAND_BUFFER_SIZE);
      }
    }
  }
}