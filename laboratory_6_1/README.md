# Laboratory 6_1 - Finite State Machine (FSM) Implementation

This project implements a simple finite state machine (FSM) to control an LED using a button as an input event, with state reporting via serial interface.

## Hardware Requirements

- Arduino Uno or compatible board
- Pushbutton
- LED (or use the built-in LED on pin 13)
- 10kΩ resistor (if not using internal pull-up)
- Jumper wires
- Breadboard
- USB cable for programming and serial communication

## Pin Connections

- **Button**: Connected to pin 2 (using internal pull-up resistor)
- **LED**: Connected to pin 13 (built-in LED)

## Software Requirements

- PlatformIO or Arduino IDE
- Serial terminal (built into PlatformIO or Arduino IDE)

## Software Structure

The project is organized in a modular structure with the following components:

1. **main.cpp**: Main program with setup and loop functions
2. **state_machine.cpp/h**: FSM implementation for LED control
3. **uart_helpers.cpp/h**: UART and stdio utility functions for serial communication

## Finite State Machine Design

The FSM has two states:
1. **State 0 (OFF)**: LED is turned off
2. **State 1 (ON)**: LED is turned on

The state transitions occur when the button is pressed (with debounce protection):
- When in OFF state and button is pressed → transition to ON state
- When in ON state and button is pressed → transition to OFF state

## Implementation Details

- **Debounce Mechanism**: Prevents false button readings by waiting for a stable signal (50ms)
- **Serial Reporting**: Current state is reported via serial interface every 500ms
- **Modular Design**: FSM logic is encapsulated in a dedicated state_machine module
- **STDIO Interface**: Uses printf redirected to Serial for state reporting

## Usage

1. Connect the hardware as specified in the pin connections
2. Upload the program to your Arduino
3. Open the serial monitor (9600 baud)
4. Press the button to toggle the LED state
5. Observe the state transitions reported in the serial monitor

## State Machine Implementation

The state machine implementation follows these principles:
- **State Storage**: Current state is stored in an enumeration variable
- **Event Detection**: Button press events are detected with debouncing
- **State Transitions**: Clearly defined transitions between states
- **Output Control**: LED output is directly controlled based on the current state
- **Reporting**: Current state is reported through the serial interface

## Practical Applications

This finite state machine pattern can be extended to:
- More complex systems with multiple states
- Systems with conditional transitions
- Sequential control systems
- User interface implementations 