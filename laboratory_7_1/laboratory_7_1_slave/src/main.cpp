#include <Arduino.h>
#include <Wire.h>
#include <Arduino_FreeRTOS.h>
#include <semphr.h>
#include "../include/UltrasonicSensor.h"
#include "../include/I2CProtocol.h"

// Pin definitions for ultrasonic sensor
#define TRIGGER_PIN 9
#define ECHO_PIN 8
#define LED_PIN 13 // Built-in LED

// Task periods
#define SENSOR_TASK_PERIOD 100 // ms
#define BUFFER_UPDATE_PERIOD 50 // ms

// I2C slave address
#define I2C_SLAVE_ADDRESS 9

// Task handles
TaskHandle_t sensorTaskHandle;
TaskHandle_t bufferUpdateTaskHandle;

// Mutex for shared buffer protection
SemaphoreHandle_t bufferMutex;

// Shared buffer for I2C communication
DataPacket responsePacket;
volatile int currentDistance = 0;

// Ultrasonic sensor instance
UltrasonicSensor sensor(TRIGGER_PIN, ECHO_PIN);

// Function declarations
void sensorTask(void *pvParameters);
void bufferUpdateTask(void *pvParameters);
void requestEvent();

void setup() {
  Serial.begin(9600);
  Serial.println("Slave initializing...");
  
  // Initialize sensor
  sensor.init();
  
  // Initialize I2C as slave
  Wire.begin(I2C_SLAVE_ADDRESS);
  Wire.onRequest(requestEvent);
  
  // LED pin for debugging
  pinMode(LED_PIN, OUTPUT);
  
  // Create mutex
  bufferMutex = xSemaphoreCreateMutex();
  
  // Create tasks
  xTaskCreate(
    sensorTask,
    "SensorTask",
    128,
    NULL,
    2,
    &sensorTaskHandle
  );
  
  xTaskCreate(
    bufferUpdateTask,
    "BufferTask",
    128,
    NULL,
    1,
    &bufferUpdateTaskHandle
  );
  
  // Start scheduler
  vTaskStartScheduler();
}

void loop() {
  // Empty - FreeRTOS scheduler takes over
}

// Task to read sensor data periodically
void sensorTask(void *pvParameters) {
  TickType_t xLastWakeTime = xTaskGetTickCount();
  
  while (1) {
    // Read distance from sensor
    int distance = sensor.readDistance();
    
    // Update shared distance variable
    if (xSemaphoreTake(bufferMutex, pdMS_TO_TICKS(10)) == pdTRUE) {
      currentDistance = distance;
      xSemaphoreGive(bufferMutex);
    }
    
    // Debug output
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");
    
    // Flash LED to show task running
    digitalWrite(LED_PIN, HIGH);
    vTaskDelay(pdMS_TO_TICKS(10));
    digitalWrite(LED_PIN, LOW);
    
    // Wait for next cycle
    vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(SENSOR_TASK_PERIOD));
  }
}

// Task to update the shared buffer with latest data
void bufferUpdateTask(void *pvParameters) {
  TickType_t xLastWakeTime = xTaskGetTickCount();
  
  while (1) {
    if (xSemaphoreTake(bufferMutex, pdMS_TO_TICKS(10)) == pdTRUE) {
      // Create packet from latest distance reading
      I2CProtocol::createDistancePacket(responsePacket, currentDistance);
      xSemaphoreGive(bufferMutex);
    }
    
    // Wait for next cycle
    vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(BUFFER_UPDATE_PERIOD));
  }
}

// I2C request event handler - called when master requests data
void requestEvent() {
  // No need for mutex here as we're just sending the current state of the packet
  // which is safely updated by the buffer update task
  Wire.write((byte*)&responsePacket, sizeof(DataPacket));
}