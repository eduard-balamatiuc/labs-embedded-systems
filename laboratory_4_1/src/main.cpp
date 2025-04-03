#include <Arduino.h>
#include <stdio.h>
#include "relay.h"

Relay relay(3); // Create relay object connected to pin 3
String inputString = "";
bool stringComplete = false;
unsigned long lastMessageTime = 0;
const unsigned long messageInterval = 3000; // 3 seconds between "Waiting for command" messages

void setup() {
  Serial.begin(9600);
  while(!Serial) {
    ; // Wait for serial port to connect
  }
  Serial.println("Relay control system ready. Enter 'relay on' or 'relay off'");
  inputString.reserve(20); // Reserve some space for the input string
}

void loop() {
  // Print waiting message at intervals
  unsigned long currentMillis = millis();
  if (currentMillis - lastMessageTime >= messageInterval) {
    Serial.println("Waiting for command...");
    lastMessageTime = currentMillis;
  }

  // Process completed command
  if (stringComplete) {
    inputString.trim(); // Remove any whitespace
    Serial.print("Command received: ");
    Serial.println(inputString);
    
    if (inputString.equals("relay on")) {
      relay.turnOn();
      Serial.println("Command executed: Relay turned ON");
    } 
    else if (inputString.equals("relay off")) {
      relay.turnOff();
      Serial.println("Command executed: Relay turned OFF");
    } 
    else {
      Serial.print("Unknown command: ");
      Serial.println(inputString);
      Serial.println("Available commands: 'relay on', 'relay off'");
    }
    
    // Clear the string for next input
    inputString = "";
    stringComplete = false;
  }
  
  // Read serial input
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    Serial.write(inChar); // Echo the character back to the serial monitor
    
    // Add character to input string
    if (inChar == '\n' || inChar == '\r') {
      stringComplete = true;
    } else {
      inputString += inChar;
    }
  }
}
