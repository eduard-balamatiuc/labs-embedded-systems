# Laboratory 3_1 - Sensor Data Acquisition with FreeRTOS

This project implements sensor data acquisition using FreeRTOS with an ultrasonic distance sensor (HC-SR04), displaying the results on both an I2C LCD display and serial monitor.

## Hardware Requirements

- Arduino Uno board
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

## Software Requirements

- PlatformIO
- Libraries:
  - FreeRTOS (feilipu/FreeRTOS@^11.1.0-3)
  - LiquidCrystal_I2C (marcoschwartz/LiquidCrystal_I2C@^1.1.4)

## Software Structure

The project is organized in a modular structure with the following components:

1. **UltrasonicSensor**: Handles the ultrasonic sensor initialization and distance reading
2. **LCDDisplay**: Manages the LCD display to show distance measurements
3. **SerialIO**: Handles serial communication for debugging and reporting
4. **TaskManager**: Coordinates the FreeRTOS tasks and manages inter-task communication

## FreeRTOS Tasks

- **sensorTask**: Reads distance from the ultrasonic sensor every 100ms and places it in a queue
- **displayTask**: Updates the LCD display every 200ms with the latest distance reading
- **statusTask**: Reports system status via Serial every 500ms including uptime and distance values

## Building and Running

1. Open the project in PlatformIO
2. Build and upload to your Arduino board
3. Open the serial monitor (115200 baud) to view the system status reports

## Implementation Details

- The project demonstrates the use of FreeRTOS for real-time multitasking
- Task scheduling uses vTaskDelayUntil() for precise timing with offsets to avoid task collision
- Task offsets are implemented using vTaskDelay() at task creation
- Inter-task communication is handled with a queue for passing distance measurements
- Access to shared resources is protected with a mutex
- System status reporting provides uptime, latest sensor values, and task statistics
- The ultrasonic sensor measures distances in centimeters with filtering for reliable readings 