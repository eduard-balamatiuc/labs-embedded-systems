#include <Arduino.h>
#include <Wire.h>
#include <Arduino_FreeRTOS.h>
#include <semphr.h>
#include "../include/uart_helpers.h"
#include "../include/I2CProtocol.h"

// I2C definitions
#define SLAVE_ADDRESS 9
#define LED_PIN 13 // Built-in LED

// Task periods
#define REQUEST_TASK_PERIOD 500 // ms
#define DISPLAY_TASK_PERIOD 500 // ms

// Task handles
TaskHandle_t requestTaskHandle;
TaskHandle_t displayTaskHandle;

// Mutex for shared data protection
SemaphoreHandle_t dataMutex;

// Shared data between tasks
volatile bool dataReceived = false;
DataPacket receivedPacket;

// Function declarations
void requestTask(void *pvParameters);
void displayTask(void *pvParameters);

void setup() {
  // Setup UART for printf
  setup_uart_stdio();
  
  printf("I2C Master Initializing...\n");
  
  // Initialize I2C as master
  Wire.begin();
  
  // LED pin for debugging
  pinMode(LED_PIN, OUTPUT);
  
  // Create mutex
  dataMutex = xSemaphoreCreateMutex();
  
  // Create tasks
  xTaskCreate(
    requestTask,
    "RequestTask",
    128,
    NULL,
    2,
    &requestTaskHandle
  );
  
  xTaskCreate(
    displayTask,
    "DisplayTask",
    128,
    NULL,
    1,
    &displayTaskHandle
  );
  
  // Start scheduler
  vTaskStartScheduler();
}

void loop() {
  // Empty - FreeRTOS scheduler takes over
}

// Task to request data from slave periodically
void requestTask(void *pvParameters) {
  TickType_t xLastWakeTime = xTaskGetTickCount();
  
  while (1) {
    // Flash LED to show task running
    digitalWrite(LED_PIN, HIGH);
    
    // Request data from slave
    Wire.requestFrom(SLAVE_ADDRESS, sizeof(DataPacket));
    
    // Read data if available
    if (Wire.available() >= sizeof(DataPacket)) {
      // Read the packet data
      byte* packetPtr = (byte*)&receivedPacket;
      for (size_t i = 0; i < sizeof(DataPacket); i++) {
        packetPtr[i] = Wire.read();
      }
      
      // Take mutex and update shared flag
      if (xSemaphoreTake(dataMutex, pdMS_TO_TICKS(10)) == pdTRUE) {
        dataReceived = true;
        xSemaphoreGive(dataMutex);
      }
    }
    
    digitalWrite(LED_PIN, LOW);
    
    // Wait for next cycle
    vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(REQUEST_TASK_PERIOD));
  }
}

// Task to display received data periodically
void displayTask(void *pvParameters) {
  TickType_t xLastWakeTime = xTaskGetTickCount();
  
  while (1) {
    bool hasData = false;
    DataPacket localPacket;
    
    // Take mutex and copy data if available
    if (xSemaphoreTake(dataMutex, pdMS_TO_TICKS(10)) == pdTRUE) {
      if (dataReceived) {
        hasData = true;
        localPacket = receivedPacket;
        dataReceived = false; // Reset flag
      }
      xSemaphoreGive(dataMutex);
    }
    
    // Process and display data if received
    if (hasData) {
      if (I2CProtocol::isPacketValid(localPacket)) {
        int distance = I2CProtocol::parseDistanceValue(localPacket);
        
        // Print using printf
        printf("=== Data Packet ===\n");
        printf("Header: 0x%02X\n", localPacket.head);
        printf("Length: %d bytes\n", localPacket.length);
        printf("Distance: %d cm\n", distance);
        printf("Checksum: 0x%02X\n", localPacket.checksum);
        printf("==================\n");
      } else {
        printf("Error: Received invalid packet\n");
      }
    }
    
    // Wait for next cycle
    vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(DISPLAY_TASK_PERIOD));
  }
}