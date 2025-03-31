#include "TaskManager.h"
#include "globals.h"
#include "filter.h"
#include <stdio.h>

// Define task handles
TaskHandle_t sensorTaskHandle = NULL;
TaskHandle_t displayTaskHandle = NULL;

// Define queue
QueueHandle_t distanceQueue = NULL;

void initTasks() {
  // Create the queue - only need to store a few readings
  distanceQueue = xQueueCreate(1, sizeof(DistanceData));
  if (distanceQueue == NULL) {
    printf("Failed to create queue\n");
    return;
  }
  
  // Create tasks with sufficient stack sizes
  xTaskCreatePinnedToCore(
    sensorTask,
    "SensorTask",
    2048,
    NULL,
    1,
    &sensorTaskHandle,
    0  // Run on core 0 (WiFi/BT core)
  );
  
  xTaskCreatePinnedToCore(
    displayTask,
    "DisplayTask",
    2048,
    NULL,
    1,
    &displayTaskHandle,
    1  // Run on core 1 (Arduino loop core)
  );
  
  printf("Tasks initialized\n");
}

// Task to read sensor and perform filtering
void sensorTask(void* parameter) {
  TickType_t lastWakeTime = xTaskGetTickCount();
  
  while (true) {
    // Send ultrasonic pulse
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);
    
    // Read the pulse duration
    long duration = pulseIn(ECHO_PIN, HIGH);
    
    // Convert to distance and apply filters
    float rawDistance = convertPulseToCm(duration);
    rawDistance = saturateValue(rawDistance, MIN_DISTANCE_CM, MAX_DISTANCE_CM);
    
    float medianFiltered = saltPepperFilter(rawDistance);
    float filteredDistance = weightedAverageFilter(medianFiltered);
    
    // Package data
    DistanceData data = {
      .rawDistance = rawDistance,
      .filteredDistance = filteredDistance
    };
    
    // Send to queue - don't wait if queue is full (overwrite old data)
    xQueueOverwrite(distanceQueue, &data);
    
    // Use vTaskDelayUntil for precise timing
    vTaskDelayUntil(&lastWakeTime, pdMS_TO_TICKS(SENSOR_TASK_PERIOD_MS));
  }
}

// Task to display data on LCD and serial
void displayTask(void* parameter) {
  TickType_t lastWakeTime = xTaskGetTickCount();
  DistanceData data;
  
  // Offset display task to avoid contention
  vTaskDelay(pdMS_TO_TICKS(50));
  
  while (true) {
    // Get latest distance reading
    if (xQueueReceive(distanceQueue, &data, 0) == pdTRUE) {
      // Print to serial
      printf("Raw: %.1f cm | Filtered: %.1f cm\n", 
             data.rawDistance, data.filteredDistance);
      
      // Update LCD
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Filt: ");
      lcd.print(data.filteredDistance, 1);
      lcd.print(" cm");
      
      lcd.setCursor(0, 1);
      lcd.print("Raw: ");
      lcd.print(data.rawDistance, 1);
      lcd.print(" cm");
    }
    
    // Update display at defined intervals
    vTaskDelayUntil(&lastWakeTime, pdMS_TO_TICKS(DISPLAY_TASK_PERIOD_MS));
  }
}