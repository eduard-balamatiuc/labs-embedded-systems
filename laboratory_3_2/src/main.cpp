#include <Arduino.h>
#include "TaskManager.h"
#include "SerialIO.h"

void setup() {
    // Initialize serial communication at 115200 baud
    SerialIO::init(115200);
    SerialIO::printMessage(F("System starting..."));
    
    // Initialize tasks and start the scheduler
    initTasks();
}

void loop() {
    // FreeRTOS takes over - this loop won't be used
} 