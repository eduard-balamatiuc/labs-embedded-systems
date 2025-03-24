# Laboratory 3_2 - Signal Conditioning with Digital Filters

This project implements digital signal conditioning for an ultrasonic distance sensor (HC-SR04), applying filtering techniques to improve signal quality. The processed data is displayed on both an LCD display and serial monitor.

## Hardware Requirements

- Arduino Uno or compatible board
- HC-SR04 Ultrasonic Sensor
- I2C LCD Display (16x2)
- Jumper wires
- Breadboard

## Pin Connections

- **Ultrasonic Sensor**:
  - Trigger Pin: 9
  - Echo Pin: 8
  - VCC: 5V
  - GND: GND

- **LCD Display**:
  - SDA: A4
  - SCL: A5
  - VCC: 5V
  - GND: GND

## Software Structure

The project is organized in a modular structure with the following components:

1. **UltrasonicSensor**: Handles the ultrasonic sensor initialization and distance reading
2. **SignalProcessor**: Implements digital filtering algorithms for signal conditioning
3. **LCDDisplay**: Manages the LCD display
4. **SerialIO**: Handles serial communication for debugging and reporting
5. **TaskManager**: Coordinates the FreeRTOS tasks

## Digital Filters Implemented

- **Salt and Pepper Filter**: Removes impulsive noise by identifying and replacing outliers
- **Weighted Moving Average Filter**: Smooths the signal by averaging recent readings with different weights

## FreeRTOS Tasks

- **sensorTask**: Reads distance from the ultrasonic sensor
- **processingTask**: Applies digital filters to the sensor data
- **displayTask**: Updates the LCD display and serial output with processed data

## Building and Running

1. Open the project in PlatformIO
2. Build and upload to your Arduino board
3. Open the serial monitor (115200 baud) to view the processed data and system status 