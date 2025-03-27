#include "TaskManager.h"
#include "globals.h"

// Task handles
TaskHandle_t sensorTaskHandle = NULL;
TaskHandle_t displayTaskHandle = NULL;

// Communication queue
QueueHandle_t sensorDataQueue = NULL;

// Filtering constants
#define MEDIAN_SAMPLES 5
#define AVERAGE_SAMPLES 5
#define SOUND_SPEED 0.034
#define MIN_DISTANCE_CM 2.0
#define MAX_DISTANCE_CM 400.0

// Function declarations from original code
float saltPepperFilter(float newValue);
float weightedAverageFilter(float newValue);
float convertPulseToCm(long pulseTime);
float saturateValue(float value, float min, float max);

void initTasks() {
    // Initialize LCD
    lcd.init();
    lcd.backlight();
    
    // Initialize sensor pins
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);
    
    // Create communication queue
    sensorDataQueue = xQueueCreate(5, sizeof(float));
    if (sensorDataQueue == NULL) {
        printf("Failed to create queue\n");
        while(1);
    }
    
    // Create tasks
    xTaskCreate(
        sensorTask,
        "SensorTask",
        2048,           // Larger stack for ESP32
        NULL,
        1,
        &sensorTaskHandle
    );
    
    xTaskCreate(
        displayTask,
        "DisplayTask",
        2048,           // Larger stack for ESP32
        NULL,
        1,
        &displayTaskHandle
    );
    
    printf("Tasks initialized\n");
}

void sensorTask(void* parameters) {
    TickType_t lastWakeTime = xTaskGetTickCount();
    
    while (1) {
        // Send ultrasonic pulse
        digitalWrite(TRIG_PIN, LOW);
        delayMicroseconds(2);
        digitalWrite(TRIG_PIN, HIGH);
        delayMicroseconds(10);
        digitalWrite(TRIG_PIN, LOW);
        
        // Read the pulse duration
        long duration = pulseIn(ECHO_PIN, HIGH);
        float rawDistance = convertPulseToCm(duration);
        
        // Process data
        rawDistance = saturateValue(rawDistance, MIN_DISTANCE_CM, MAX_DISTANCE_CM);
        float medianFiltered = saltPepperFilter(rawDistance);
        float filteredDistance = weightedAverageFilter(medianFiltered);
        
        // Send to queue
        xQueueSend(sensorDataQueue, &filteredDistance, 0);
        
        // Precise timing
        vTaskDelayUntil(&lastWakeTime, pdMS_TO_TICKS(SENSOR_READING_PERIOD));
    }
}

void displayTask(void* parameters) {
    TickType_t lastWakeTime = xTaskGetTickCount();
    float distance;
    
    // Add initial offset
    vTaskDelay(pdMS_TO_TICKS(100));
    
    while (1) {
        if (xQueueReceive(sensorDataQueue, &distance, 0) == pdTRUE) {
            // Print to Serial
            printf("Distance: %.1f cm\n", distance);
            
            // Update LCD
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Distance:");
            lcd.setCursor(0, 1);
            lcd.print(distance, 1);
            lcd.print(" cm");
        }
        
        // Precise timing
        vTaskDelayUntil(&lastWakeTime, pdMS_TO_TICKS(DISPLAY_UPDATE_PERIOD));
    }
}