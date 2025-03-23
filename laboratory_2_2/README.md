# FreeRTOS Modular Embedded Application

This project demonstrates a modular embedded application using FreeRTOS on an Arduino-compatible microcontroller. The application showcases real-time scheduling, task synchronization using semaphores, and inter-task communication via queues.

## Project Structure

The project follows a modular architecture:

- **Button Module**: Handles button input detection
- **LED Module**: Controls LED outputs with various blinking patterns
- **SerialIO Module**: Manages serial communication
- **TaskManager**: Orchestrates FreeRTOS tasks and their interactions

## Hardware Requirements

- Arduino-compatible MCU with FreeRTOS support
- 3 LEDs with current-limiting resistors (220-330 ohm)
- 1 push button
- 1 pull-down resistor (10k ohm) for button (if not using internal pull-down)

See `docs/schematic.txt` for detailed hardware connections.

## Task Description

### Task 1: Button Task (Priority 1)
- Checks the button state every 10 ms
- When the button is pressed, turns LED1 ON for exactly 1 second
- Signals a binary semaphore to trigger Task 2
- Ensures precise timing using vTaskDelayUntil

### Task 2: Sequence Task (Priority 2)
- Waits for semaphore from Task 1
- Upon receiving the semaphore:
  - Increments counter N by 1 (initially N = a)
  - Sends a sequence of bytes (1, 2, ..., N) to a buffer
  - Blinks LED2 exactly N times:
    - ON duration: 300 ms
    - OFF duration: 500 ms
  - Adds a 50 ms delay after completing the blinking sequence

### Task 3: Output Task (Priority 3)
- Every 200 ms, reads data from the buffer
- Outputs the received bytes via serial port
- When receiving a byte value of 0, prints a newline for clarity

## Task Interaction Diagram

See `docs/block_diagram.txt` for a visual representation of task interactions.

## Building and Running

1. Open the project in PlatformIO
2. Build the project
3. Upload to your Arduino-compatible board
4. Open the serial monitor at 9600 baud to view the output

## Dependencies

- Arduino framework
- FreeRTOS library 