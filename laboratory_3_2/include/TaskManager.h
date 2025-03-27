#ifndef TASK_MANAGER_H
#define TASK_MANAGER_H

#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <LiquidCrystal_I2C.h>

// Task handles
extern TaskHandle_t sensorTaskHandle;
extern TaskHandle_t displayTaskHandle;

// Communication queue
extern QueueHandle_t sensorDataQueue;

// Task parameters
#define SENSOR_READING_PERIOD    100     // Sensor reading period (ms)
#define DISPLAY_UPDATE_PERIOD    500     // Display update period (ms)

// Pin definitions
#define TRIG_PIN    5
#define ECHO_PIN    18

// Function prototypes
void initTasks();
void sensorTask(void* parameters);
void displayTask(void* parameters);

#endif // TASK_MANAGER_H