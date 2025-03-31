#include <Arduino.h>
#include "globals.h"
#include "TaskManager.h"
#include <stdio.h>

void setup() {
  Serial.begin(115200);
  
  // Initialize LCD
  lcd.init();
  lcd.backlight();
  
  // Initialize ultrasonic sensor pins
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  
  printf("Ultrasonic Distance Sensor with Filtering\n");
  
  // Initialize FreeRTOS tasks
  initTasks();
}

void loop() {
  // Empty loop - all work is done in FreeRTOS tasks
  vTaskDelay(pdMS_TO_TICKS(1000)); // Just prevent watchdog resets
}