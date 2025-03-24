#ifndef TASK_MANAGER_H
#define TASK_MANAGER_H

#include <Arduino.h>
#include <Arduino_FreeRTOS.h>
#include "task.h"
#include "semphr.h"
#include "queue.h"
#include "SerialIO.h"
#include "UltrasonicSensor.h"
#include "SignalProcessor.h"
#include "LCDDisplay.h"

// Task handles
extern TaskHandle_t sensorTaskHandle;
extern TaskHandle_t processingTaskHandle;
extern TaskHandle_t displayTaskHandle;

// Synchronization and communication
extern QueueHandle_t rawDataQueue;
extern QueueHandle_t processedDataQueue;
extern SemaphoreHandle_t serialMutex;  // Mutex for protecting serial output

// Task parameters
#define SENSOR_READING_PERIOD    100     // Sensor reading period (ms)
#define PROCESSING_PERIOD        50      // Processing period (ms)
#define DISPLAY_UPDATE_PERIOD    500     // Display update period (ms)

// Pin definitions for ultrasonic sensor
#define TRIGGER_PIN              9       // Ultrasonic sensor trigger pin
#define ECHO_PIN                 8       // Ultrasonic sensor echo pin

// Function prototypes
void initTasks();
void sensorTask(void* parameters);
void processingTask(void* parameters);
void displayTask(void* parameters);

#endif // TASK_MANAGER_H 