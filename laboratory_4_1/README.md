# Laboratory 4_1 - Relay Control System

This project implements a simple relay control system that can be controlled via serial commands. The system allows for turning a relay on and off through the serial interface.

## Hardware Requirements

- Arduino Uno or compatible board
- Relay module (connected to pin 3)
- USB cable for programming and serial communication
- Optional: Device to be controlled by the relay (e.g., lamp, fan, etc.)

## Pin Connections

- **Relay Module**:
  - Signal Pin: 3
  - VCC: 5V
  - GND: GND

## Software Requirements

- PlatformIO or Arduino IDE
- Serial terminal (built into PlatformIO or Arduino IDE)

## Software Structure

The project is organized in a modular structure with the following components:

1. **main.cpp**: Main program loop with serial communication handling
2. **relay.cpp/h**: Relay control module
3. **stdio_redirect.cpp**: Utility for redirecting printf to Serial

## Command Interface

The system accepts the following commands via serial:
- `relay on`: Turns the relay on
- `relay off`: Turns the relay off

## Command Processing

1. Serial input is read character by character and echoed back to the user
2. Commands are buffered until a newline character is received
3. The command is parsed and validated
4. Appropriate action is taken based on the command
5. Feedback is provided to the user

## Building and Running

1. Open the project in PlatformIO
2. Build and upload to your Arduino board
3. Open the serial monitor (9600 baud)
4. Enter commands to control the relay

## Implementation Details

- Uses a non-blocking approach to read serial input
- Provides helpful feedback and error messages
- Sends a "waiting for command" message every 3 seconds
- Implements a modular design with separate relay control class
- Serial communication is initialized at 9600 baud
- Commands are case-sensitive

## Practical Applications

This relay control system can be used for:
- Home automation projects
- Remote device control
- Automated switching of high-voltage devices
- Integration into larger control systems 