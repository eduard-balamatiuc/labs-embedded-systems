#ifndef TASK_MANAGER_H
#define TASK_MANAGER_H

#include <Arduino.h>
#include <Arduino_FreeRTOS.h>
#include "task.h"
#include "semphr.h"
#include "queue.h"
#include "SerialIO.h"
#include "DHT11Sensor.h"
#include "SensorFilter.h"
#include "LCDDisplay.h"

// Task handles
extern TaskHandle_t sensorTaskHandle;
extern TaskHandle_t displayTaskHandle;
extern TaskHandle_t statusTaskHandle;

// Synchronization and communication
extern QueueHandle_t sensorDataQueue;
extern SemaphoreHandle_t serialMutex;  // Mutex for protecting serial output

// Task parameters
#define SENSOR_READING_PERIOD    100     // Sensor reading period (ms)
#define DISPLAY_UPDATE_PERIOD    200     // Display update period (ms)
#define STATUS_UPDATE_PERIOD     500     // Status reporting period (ms)

// Pin definitions for DHT11 sensor
#define DHT11_PIN            2

// Filter parameters
#define FILTER_WINDOW_SIZE   5
#define TEMP_MIN             0.0
#define TEMP_MAX             50.0
#define HUM_MIN              20.0
#define HUM_MAX              90.0

// Data structure to hold sensor readings
struct SensorData {
    float rawTemperature;
    float rawHumidity;
    float filteredTemperature;
    float filteredHumidity;
};

// Function prototypes
void initTasks();
void sensorTask(void* parameters);
void displayTask(void* parameters);
void statusTask(void* parameters);

#endif // TASK_MANAGER_H 