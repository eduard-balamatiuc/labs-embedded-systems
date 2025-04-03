#include <Arduino.h>
#include <stdio.h>
#include "relay.h"

// Custom printf replacement that uses Serial
void debug_printf(const char* format, ...) {
  char buffer[256];
  va_list args;
  va_start(args, format);
  vsnprintf(buffer, sizeof(buffer), format, args);
  va_end(args);
  Serial.print(buffer);
}

Relay relay(3); // Create relay object connected to pin 3
char inputBuffer[50];
char command[20];
bool commandReceived = false;
unsigned long lastMessageTime = 0;
const unsigned long messageInterval = 3000; // 3 seconds between messages

void setup() {
  Serial.begin(9600);
  // Wait for serial port to connect (especially important for native USB port)
  delay(1000); // Give time for serial connection to establish
  while(!Serial) {
    ; // Wait for serial port to connect
  }
  debug_printf("Relay control system ready. Enter 'relay on' or 'relay off'\n");
}

void loop() {
  // Print waiting message at intervals
  unsigned long currentMillis = millis();
  if (currentMillis - lastMessageTime >= messageInterval) {
    debug_printf("Waiting for command...\n");
    lastMessageTime = currentMillis;
  }

  // Check if data is available on the serial port
  if (Serial.available() > 0) {
    // Read input character by character and echo it back
    char c = Serial.read();
    Serial.write(c); // Echo the character for visibility
    
    // Store characters in buffer until newline
    static int bufferIndex = 0;
    if (c != '\n' && c != '\r') {
      if (bufferIndex < sizeof(inputBuffer) - 1) {
        inputBuffer[bufferIndex++] = c;
      }
    } else {
      // Terminate the string
      inputBuffer[bufferIndex] = '\0';
      commandReceived = true;
      bufferIndex = 0;
    }
  }
  
  // Process command when complete
  if (commandReceived) {
    debug_printf("Command received: %s\n", inputBuffer);
    
    // Use sscanf to parse the command
    if (sscanf(inputBuffer, "%19s", command) == 1) {
      if (strcmp(command, "relay") == 0) {
        // Read the second part of the command
        if (sscanf(inputBuffer, "%*s %19s", command) == 1) {
          if (strcmp(command, "on") == 0) {
            relay.turnOn();
            debug_printf("Command executed: Relay turned ON\n");
          } else if (strcmp(command, "off") == 0) {
            relay.turnOff();
            debug_printf("Command executed: Relay turned OFF\n");
          } else {
            debug_printf("Unknown command parameter: %s\n", command);
            debug_printf("Available commands: 'relay on', 'relay off'\n");
          }
        }
      } else {
        debug_printf("Unknown command: %s\n", command);
        debug_printf("Available commands: 'relay on', 'relay off'\n");
      }
    }
    
    // Reset for next command
    commandReceived = false;
  }
}
