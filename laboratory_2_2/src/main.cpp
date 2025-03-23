#include <Arduino.h>
#include <Arduino_FreeRTOS.h>
#include "SerialIO.h"
#include "TaskManager.h"

void setup() {
  // Initialize serial communication
  SerialIO::init(9600);
  
  // Initialize and start tasks
  initTasks();
  
  // Start the scheduler
  vTaskStartScheduler();
}

void loop() {
  // Empty - Tasks are handled by FreeRTOS
}
