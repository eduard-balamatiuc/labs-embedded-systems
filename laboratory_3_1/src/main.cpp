#include <Arduino.h>
#include <Arduino_FreeRTOS.h>
#include "SerialIO.h"
#include "TaskManager.h"

void setup() {
  // Initialize serial communication
  SerialIO::init(115200);
  
  // Initialize and start tasks
  initTasks();
  
  // Start the scheduler
  vTaskStartScheduler();
}

void loop() {
  // Empty - Tasks are handled by FreeRTOS
} 