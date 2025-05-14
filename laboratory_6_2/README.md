# Smart Traffic Light Controller (Optimized)

This project implements a smart traffic light system for an intersection with two traffic directions (East-West and North-South) using a finite state machine and the FreeRTOS real-time operating system.

## Project Requirements

The project implements the following requirements:

1. Modular application using a microcontroller (MCU) that implements a smart traffic light for an intersection with two traffic directions:
   - East-West direction
   - North-South direction

2. Uses a finite state machine and the FreeRTOS real-time operating system

3. Displays the current state of the traffic light through a serial interface using STDIO

4. Transitions from one green direction to another based on an active request simulated with a button

5. If there is no request for the North direction, the East direction has priority and remains green

6. Transitions follow traffic signaling rules: green → yellow → red

7. Display time for each state (green, yellow, red) is controlled through FreeRTOS delays (vTaskDelay)

8. Implements a finite state machine to control each direction

9. Night mode feature with flashing yellow activated by a separate button

## Memory Optimization

This implementation has been optimized for memory usage on Arduino:

1. Uses a single task instead of multiple tasks to reduce stack memory usage
2. Minimizes printf output to reduce memory consumption
3. Eliminates unused features (semaphores) to save resources
4. Uses more compact state detection logic to reduce code size
5. Shortened direction and state names to save memory

## Hardware Components

The project requires the following hardware components:

* Arduino Uno or compatible board
* 6 LEDs (2 green, 2 yellow, 2 red)
* 2 pushbuttons
* Resistors (220Ω for LEDs, 10kΩ for buttons)
* Breadboard and jumper wires

## Pin Connections

| Component | Pin |
|-----------|-----|
| East-West Green LED | 5 |
| East-West Yellow LED | 6 |
| East-West Red LED | 7 |
| North-South Green LED | 8 |
| North-South Yellow LED | 9 |
| North-South Red LED | 10 |
| Request Button | 2 |
| Night Mode Button | 3 |

## Software Architecture

The project is organized into the following components:

1. **main.cpp**: Entry point of the application, initializes the system and starts the FreeRTOS scheduler

2. **traffic_light.h/cpp**: Implements the traffic light state machine, defines the states, and contains the logic for controlling the traffic lights

3. **uart_helpers.h/cpp**: Provides helper functions for UART communication and redirects printf output to the Serial interface

## State Machine

Each traffic light direction has the following states:
- GREEN: Light is green
- YELLOW: Light is yellow
- RED: Light is red
- NIGHT_MODE: Yellow light flashing

The state transitions follow the traffic signaling rules:
1. East-West direction starts with GREEN, North-South with RED
2. When a request for North-South is received (button press), East-West transitions to YELLOW
3. After a delay, East-West transitions to RED and North-South transitions to GREEN
4. After a fixed time, North-South transitions to YELLOW
5. After a delay, North-South transitions to RED and East-West transitions to GREEN
6. If no request is made for North-South, East-West remains GREEN
7. If night mode is activated, both directions flash YELLOW

## Building and Running

The project is built using PlatformIO. To build and upload the project:

1. Connect the Arduino board to your computer
2. Open the project in PlatformIO
3. Click on the "Build" button
4. Click on the "Upload" button
5. Open the Serial Monitor to view the system status
