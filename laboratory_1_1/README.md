# Laboratory 1.1 - Basic LED Control via Serial

This project implements a simple LED control system using Arduino, where the onboard LED can be turned on and off through serial commands.

## Hardware Requirements

- Arduino Uno or compatible board
- Built-in LED (connected to pin 13)
- USB cable for programming and serial communication

## Software Requirements

- Arduino IDE

## Pin Connections

- **LED**: Uses the built-in LED connected to pin 13

## Functionality

The application allows for controlling the onboard LED through serial commands:
- `led_on`: Turns on the LED
- `led_off`: Turns off the LED

Any other command will result in an error message.

## Project Structure

- **laboratory_1_1.ino**: Main Arduino sketch file
- **led_control.h/cpp**: Module for LED control functions
- **printf_init.h/cpp**: Utility for redirecting printf to Serial

## Building and Running

1. Open the project in Arduino IDE
2. Connect your Arduino board via USB
3. Upload the sketch to the board
4. Open the Serial Monitor (9600 baud)
5. Enter `led_on` or `led_off` to control the LED

## Implementation Details

- The project demonstrates basic Arduino programming concepts
- Uses a modular approach with separate files for LED control
- Implements printf capability for better serial output formatting 