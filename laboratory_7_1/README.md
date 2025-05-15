# Laboratory 7.1 - I2C Master-Slave Communication System

This project implements an I2C communication system between two Arduino UNO boards using the FreeRTOS real-time operating system. The system consists of a master and a slave unit, where the slave is equipped with an ultrasonic sensor to measure distances, and the master periodically requests and displays these measurements.

## Project Structure

The project is divided into two separate components:

1. **Master MCU** (`laboratory_7_1_master/`): Requests data from the slave and displays it using printf
2. **Slave MCU** (`laboratory_7_1_slave/`): Reads distance measurements and responds to the master's requests

## System Features

- **Real-time task execution** using FreeRTOS
- **Modular architecture** separating sensor operations from communication
- **Structured data protocol** with header, length, payload, and checksum
- **Synchronization mechanisms** (mutex) for thread-safe operation
- **Standard I/O functions** for formatted data display

## Hardware Requirements

- 2× Arduino UNO boards
- HC-SR04 Ultrasonic sensor
- Jumper wires
- USB cables for power and programming

## Software Architecture

### Slave MCU Software

1. **Sensor Task**: 
   - Periodically reads distance values from the HC-SR04 ultrasonic sensor
   - Updates a shared variable protected by a mutex

2. **Buffer Update Task**:
   - Creates properly formatted data packets with the latest sensor readings
   - Ensures data integrity with checksums

3. **I2C Request Handler**:
   - Responds to master's requests with the latest prepared data packet

### Master MCU Software

1. **Request Task**:
   - Periodically sends data requests to the slave
   - Receives and stores the response data

2. **Display Task**:
   - Processes received data packets
   - Validates packet integrity
   - Displays formatted output using printf

## Communication Protocol

The communication follows a structured format:

```
+--------+--------+----------------+--------+
| HEAD   | LENGTH | PAYLOAD        | CHECK  |
| (1B)   | (1B)   | (variable)     | (1B)   |
+--------+--------+----------------+--------+
```

- **HEAD**: Fixed value (0xAA) marking the start of a packet
- **LENGTH**: Number of bytes in the payload (2 for distance data)
- **PAYLOAD**: Distance data (high byte, low byte)
- **CHECK**: Checksum calculated as the sum of all previous bytes

## Getting Started

1. Upload the appropriate code to each Arduino board:
   - `laboratory_7_1_slave/src/main.cpp` to the slave Arduino
   - `laboratory_7_1_master/src/main.cpp` to the master Arduino

2. Connect the hardware according to the wiring diagrams in the docs folder

3. Open the serial monitor (115200 baud) to view the distance measurements

## Implementation Details

### Used Libraries

- Arduino Core
- FreeRTOS for Arduino
- Wire (I2C)
- Standard I/O (stdio)

### Key Files

- **UltrasonicSensor**: Encapsulates the sensor functionality
- **I2CProtocol**: Handles packet creation and validation
- **uart_helpers**: Implements stdio functions for printf

## Learning Outcomes

This project demonstrates:
- Implementing inter-processor communication using I2C
- Working with real-time task scheduling in FreeRTOS
- Using mutexes to protect shared resources
- Creating structured communication protocols
- Using standard I/O functions on embedded systems 