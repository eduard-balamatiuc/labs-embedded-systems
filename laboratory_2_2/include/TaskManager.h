#ifndef TASK_MANAGER_H
#define TASK_MANAGER_H

#include <Arduino.h>
#include <Arduino_FreeRTOS.h>
#include "task.h"
#include "semphr.h"
#include "queue.h"
#include "Button.h"
#include "LED.h"
#include "SerialIO.h"

// Task handles
extern TaskHandle_t buttonTaskHandle;
extern TaskHandle_t sequenceTaskHandle;
extern TaskHandle_t outputTaskHandle;

// Synchronization and communication
extern SemaphoreHandle_t buttonSemaphore;
extern QueueHandle_t dataQueue;

// Pin definitions
#define BUTTON_PIN          2   // Button connected to pin 2
#define LED1_PIN            12  // LED1 connected to pin 12
#define LED2_PIN            13  // LED2 connected to pin 13
#define LED3_PIN            11  // LED3 connected to pin 11

// Task parameters
#define BUTTON_CHECK_PERIOD     10      // Button check period (ms)
#define LED1_ON_DURATION        1000    // LED1 on duration after button press (ms)
#define LED2_ON_DURATION        300     // LED2 on duration (ms)
#define LED2_OFF_DURATION       500     // LED2 off duration (ms)
#define TASK3_PERIOD            200     // Task 3 period (ms)
#define TASK2_DELAY             50      // Task 2 delay after blinking (ms)

// Function prototypes
void initTasks();
void buttonTask(void* parameters);
void sequenceTask(void* parameters);
void outputTask(void* parameters);

#endif // TASK_MANAGER_H 