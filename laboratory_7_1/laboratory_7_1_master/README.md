# I2C Master-Slave Communication System (Master MCU)

This project implements an I2C communication system between two Arduino UNO boards. The master periodically requests distance data from the slave, which is equipped with an ultrasonic sensor, and displays the received data using standard I/O functions.

## Features

- FreeRTOS task management for concurrent operations
- I2C communication protocol with structured data packets
- Standard I/O (printf) for formatted display of received data
- Robust error checking with checksum verification

## Hardware Requirements

- Arduino UNO board
- I2C connection wires (SDA, SCL)
- USB cable for power and serial communication

## Wiring Connection

1. Connect the I2C pins between Master and Slave:
   - Arduino UNO Master SDA (A4) → Arduino UNO Slave SDA (A4)
   - Arduino UNO Master SCL (A5) → Arduino UNO Slave SCL (A5)
   - GND → GND (common ground between boards)

## Software Implementation

### Task Structure

The system uses two FreeRTOS tasks:

1. **Request Task**: Periodically sends data requests to the slave MCU
   - Period: 500ms
   - Priority: 2 (higher)
   - Handles I2C communication protocol

2. **Display Task**: Processes and displays received data
   - Period: 500ms
   - Priority: 1 (lower)
   - Uses printf for formatted output

### Communication Protocol

Data packets follow this format:
- **HEAD**: 0xAA (Fixed start byte)
- **LENGTH**: Number of bytes in the payload
- **PAYLOAD**: Distance data (2 bytes - high byte and low byte)
- **CHECKSUM**: Verification byte calculated from other fields

## Libraries Used

- Arduino FreeRTOS - Task management and synchronization
- Wire - I2C communication
- Standard I/O - Formatted data output

## Usage

1. Upload the master code to one Arduino UNO board
2. Upload the slave code to another Arduino UNO board
3. Connect the boards according to the wiring instructions
4. Open the serial monitor at 115200 baud rate to view the distance data

The system will automatically begin requesting and displaying the distance data from the ultrasonic sensor attached to the slave board.
