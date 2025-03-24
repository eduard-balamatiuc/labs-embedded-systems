#include <Arduino.h>
#include <Arduino_FreeRTOS.h>
#include "SerialIO.h"
#include "TaskManager.h"

void setup() {
  // Initialize serial communication with higher baud rate for detailed reports
  SerialIO::init(115200);
  
  // Initialize and start tasks
  initTasks();
  
  // Start the scheduler
  vTaskStartScheduler();
}

void loop() {
  // Empty - Tasks are handled by FreeRTOS
} 