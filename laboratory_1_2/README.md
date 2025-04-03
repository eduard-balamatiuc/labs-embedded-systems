# Laboratory 1.2 - Keypad Security System

This project implements a keypad-based security system using Arduino, with an LCD display for feedback and LEDs for status indication.

## Hardware Requirements

- Arduino Uno or compatible board
- 4x4 Matrix Keypad
- I2C LCD Display (16x2)
- Green LED (status indicator for valid code) - pin 13 (onboard LED)
- Red LED (status indicator for invalid code) - pin 12
- Jumper wires
- Breadboard
- USB cable for programming and serial communication

## Software Requirements

- Arduino IDE
- Libraries:
  - Wire.h (I2C communication)
  - LiquidCrystal_I2C.h (LCD control)
  - Keypad.h (matrix keypad interfacing)

## Pin Connections

- **Keypad**:
  - Rows: Pins 9, 8, 7, 6
  - Columns: Pins 5, 4, 3, 2

- **LCD Display**:
  - SDA: A4
  - SCL: A5
  - VCC: 5V
  - GND: GND

- **LEDs**:
  - Green LED: Pin 13 (onboard LED)
  - Red LED: Pin 12

## Functionality

The system acts as a simple security code entry system:
- Users can enter a 4-digit code using the keypad
- The entered code is displayed on the LCD
- If the correct code (1234) is entered, access is granted and the green LED lights up
- If an incorrect code is entered, access is denied and the red LED lights up
- '#' key functions as Enter/Submit
- '*' key clears the current input

## Project Structure

- **laboratory_1_2.ino**: Main Arduino sketch file
- **led_control.h/cpp**: Module for LED control functions
- **printf_init.h/cpp**: Utility for redirecting printf to Serial
- **images/**: Documentation images showing setup and circuit

## Building and Running

1. Wire the components according to the pin connections
2. Open the project in Arduino IDE
3. Install required libraries if not already installed
4. Connect your Arduino board via USB
5. Upload the sketch to the board
6. The system will initialize and display "Enter Code:" on the LCD
7. Use the keypad to interact with the system

## Implementation Details

- LCD is initialized with I2C communication
- Matrix keypad is configured in a 4x4 layout
- Serial monitor (115200 baud) provides debugging information
- The project demonstrates:
  - User interface development
  - Input validation
  - Visual feedback with LCD and LEDs
  - Modular code structure 