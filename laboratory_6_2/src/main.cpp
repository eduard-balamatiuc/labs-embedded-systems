#include <Arduino.h>
#include <Arduino_FreeRTOS.h>
#include <stdio.h>
#include "uart_helpers.h"
#include "traffic_light.h"

void setup() {
  // Initialize serial communication
  Serial.begin(9600);
  
  // Initialize stdio for printf functionality
  setup_uart_stdio();
  
  // Print welcome message
  printf("\n\n==============================\n");
  printf("Smart Traffic Light Controller\n");
  printf("==============================\n\n");
  
  // Initialize traffic light system
  initTrafficLights();
  
  // Create traffic light tasks
  createTrafficLightTasks();
  
  // No need for a standard loop function as FreeRTOS will manage the tasks
  printf("System startup complete. FreeRTOS scheduler starting...\n\n");
}

void loop() {
  // Empty - all work is done in FreeRTOS tasks
  // This function is required by Arduino but not used
}
