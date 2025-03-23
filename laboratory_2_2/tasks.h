#ifndef TASKS_H
#define TASKS_H

#include <Arduino_FreeRTOS.h>
#include <semphr.h>
#include <queue.h>

// Task function declarations
void buttonLedTask(void *pvParameters);
void syncTask(void *pvParameters);
void asyncTask(void *pvParameters);

// Synchronization objects initialization
void initSyncObjects();

#endif // TASKS_H 