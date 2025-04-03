# Laboratory 3_2 - Ultrasonic Distance Sensor with Advanced Filtering

This project implements an ultrasonic distance measurement system on an ESP32 using FreeRTOS, with advanced filtering algorithms to improve measurement accuracy and stability.

## Hardware Requirements

- ESP32 Development Board
- HC-SR04 Ultrasonic Sensor
- I2C LCD Display (16x2)
- Jumper wires
- Breadboard
- USB cable for programming and serial communication

## Pin Connections

- **Ultrasonic Sensor**:
  - Trigger Pin: GPIO 5
  - Echo Pin: GPIO 18
  - VCC: 5V (or 3.3V with level shifter)
  - GND: GND

- **LCD Display**:
  - SDA: GPIO 21 (default I2C pins for ESP32)
  - SCL: GPIO 22 (default I2C pins for ESP32)
  - VCC: 5V
  - GND: GND

## Software Requirements

- PlatformIO
- Libraries:
  - LiquidCrystal_I2C (marcoschwartz/LiquidCrystal_I2C@^1.1.4)
  - FreeRTOS (built into ESP32 Arduino core)

## Software Structure

The project is organized in a modular structure with the following components:

1. **filter.cpp**: Implements median and moving average filtering algorithms
2. **globals.cpp/h**: Global variables and constants
3. **TaskManager.cpp/h**: FreeRTOS task creation and management
4. **stdio_redirect.cpp**: Utility for redirecting printf to Serial

## Filtering Algorithms

The project implements two filtering algorithms to improve measurement stability:

1. **Median Filter**: 
   - Collects 5 samples and selects the median value
   - Eliminates outliers and spike noise

2. **Moving Average Filter**:
   - Takes the median-filtered values and applies a moving average
   - Smooths the measurements and reduces jitter

## FreeRTOS Tasks

- **sensorTask**: Reads raw distance from the ultrasonic sensor
- **filterTask**: Applies median and average filtering to the raw readings
- **displayTask**: Updates the LCD display with both raw and filtered values
- **statsTask**: Reports system status and measurements via Serial

## Building and Running

1. Open the project in PlatformIO
2. Build and upload to your ESP32 board
3. Open the serial monitor (115200 baud) to view the measurements and statistics

## Implementation Details

- Uses ESP32's built-in FreeRTOS implementation
- Distance is measured using the HC-SR04 time-of-flight principle
- Implements bound checking (2cm to 400cm measurement range)
- The LCD display shows both raw and filtered distance values
- Serial output provides detailed statistics on the filtering process
- Tasks use semaphores for synchronization and notification mechanisms
- Demonstrates ESP32-specific features and capabilities 