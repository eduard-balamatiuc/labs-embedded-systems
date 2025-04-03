# Laboratory 2.1 - Sequential Task System

This project demonstrates a sequential task system implementation on Arduino, simulating a simple real-time operating system without using actual RTOS libraries.

## Hardware Requirements

- Arduino Uno or compatible board
- 2 LEDs with current-limiting resistors (220-330 ohm)
- 3 push buttons
- Jumper wires
- Breadboard
- USB cable for programming and serial communication

## Software Requirements

- PlatformIO or Arduino IDE

## Pin Connections

- **LEDs**:
  - LED 1: Pin 11
  - LED 2: Pin 12

- **Buttons**:
  - Button 1: Pin 2 (with internal pull-up)
  - Increment Button: Pin 3 (with internal pull-up)
  - Decrement Button: Pin 4 (with internal pull-up)

## Functionality

The project implements a sequential task system with three main tasks:

1. **Button LED Task (50ms recurrence)**:
   - Monitors Button 1
   - Toggles LED 1 when the button is pressed

2. **Blinking LED Task (100ms recurrence)**:
   - Controls LED 2 with a blinking pattern
   - Independent of user input

3. **State Variable Task (100ms recurrence)**:
   - Manages a state variable that can be incremented/decremented
   - Monitors increment and decrement buttons
   - Reports state changes via serial

An idle task also reports system status information periodically.

## Project Structure

- **src/**
  - **main.cpp**: Main program with task scheduler implementation
  - **ButtonLED.cpp**: Task 1 implementation
  - **BlinkingLED.cpp**: Task 2 implementation
  - **StateVariable.cpp**: Task 3 implementation
  - **IdleTask.cpp**: Idle task for system reporting
  - **PrintfRedirect.cpp**: Utility for redirecting printf to Serial

- **include/**
  - **ButtonLED.h**: Task 1 header
  - **BlinkingLED.h**: Task 2 header
  - **StateVariable.h**: Task 3 header
  - **IdleTask.h**: Idle task header
  - **Config.h**: System configuration parameters
  - **Globals.h**: Global variables and shared resources

- **images/**: Documentation images

## Task Execution Details

- Tasks are executed in a specific order with precise timing
- Each task has a defined recurrence interval:
  - Task 1: 50ms
  - Task 2: 100ms
  - Task 3: 100ms
  - Idle Reporting: 1000ms
- Tasks are initiated with specific offsets to avoid simultaneous execution:
  - Task 1: 0ms offset
  - Task 2: 20ms offset
  - Task 3: 40ms offset

## Building and Running

1. Open the project in PlatformIO or Arduino IDE
2. Connect your Arduino board via USB
3. Build and upload the sketch to the board
4. Open the Serial Monitor (9600 baud) to view task execution and state information

## Implementation Details

- Demonstrates a basic task scheduler without using an RTOS
- Uses millis() for non-blocking timing
- Implements task offsets to avoid simultaneous task execution
- Provides system status reporting through the serial interface 