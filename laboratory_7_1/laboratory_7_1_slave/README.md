# I2C Master-Slave Communication System (Slave MCU)

This project implements the slave component of an I2C communication system between two Arduino UNO boards. The slave is equipped with an ultrasonic sensor to measure distances and responds to data requests from the master with structured data packets.

## Features

- FreeRTOS task management for concurrent operations
- Ultrasonic sensor integration for distance measurement
- I2C slave functionality with request event handling
- Thread-safe buffer management with mutex protection

## Hardware Requirements

- Arduino UNO board
- HC-SR04 Ultrasonic Sensor
- I2C connection wires (SDA, SCL)
- LED for status indication (built-in LED used)
- USB cable for power and debugging

## Wiring Connection

1. Connect the Ultrasonic Sensor:
   - HC-SR04 VCC → Arduino 5V
   - HC-SR04 GND → Arduino GND
   - HC-SR04 TRIG → Arduino Digital Pin 9
   - HC-SR04 ECHO → Arduino Digital Pin 8

2. Connect the I2C pins to Master:
   - Arduino UNO Slave SDA (A4) → Arduino UNO Master SDA (A4)
   - Arduino UNO Slave SCL (A5) → Arduino UNO Master SCL (A5)
   - GND → GND (common ground between boards)

## Software Implementation

### Task Structure

The system uses two FreeRTOS tasks:

1. **Sensor Task**: Periodically reads distance from the ultrasonic sensor
   - Period: 100ms
   - Priority: 2 (higher)
   - Updates shared distance variable

2. **Buffer Update Task**: Updates the I2C response buffer with latest data
   - Period: 50ms
   - Priority: 1 (lower)
   - Creates properly formatted data packets

### Communication Protocol

Data packets follow this format:
- **HEAD**: 0xAA (Fixed start byte)
- **LENGTH**: Number of bytes in the payload
- **PAYLOAD**: Distance data (2 bytes - high byte and low byte)
- **CHECKSUM**: Verification byte calculated from other fields

## Sensor Implementation

The ultrasonic sensor (HC-SR04) is used to measure distances:
- Sends ultrasonic pulses and measures the echo time
- Converts time to distance using the speed of sound
- Typical range: 2cm to 400cm

## Libraries Used

- Arduino FreeRTOS - Task management and synchronization
- Wire - I2C communication
- UltrasonicSensor - Custom sensor interface

## Usage

1. Upload the slave code to an Arduino UNO board
2. Upload the master code to another Arduino UNO board
3. Connect the boards and sensor according to the wiring instructions
4. The system will automatically begin measuring distances and responding to master requests

The built-in LED will flash briefly each time a sensor reading is taken.
