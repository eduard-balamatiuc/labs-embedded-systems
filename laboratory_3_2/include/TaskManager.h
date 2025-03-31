#pragma once

#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/queue.h>
#include <esp_task_wdt.h>

// Task handles
extern TaskHandle_t sensorTaskHandle;
extern TaskHandle_t displayTaskHandle;

// Communication data structure
typedef struct {
  float rawDistance;
  float filteredDistance;
} DistanceData;

// Queue handle
extern QueueHandle_t distanceQueue;

// Task timing constants
#define SENSOR_TASK_PERIOD_MS 100
#define DISPLAY_TASK_PERIOD_MS 500

// Function prototypes
void initTasks();
void sensorTask(void* parameter);
void displayTask(void* parameter);