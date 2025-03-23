# FreeRTOS Multi-Task Button LED Application

This project demonstrates a modular FreeRTOS application for ESP32 or Arduino-compatible microcontrollers using three synchronized tasks.

## Hardware Requirements

- ESP32 or Arduino-compatible microcontroller
- 2 LEDs
- 1 pushbutton
- 2 resistors (220Ω) for LEDs
- 1 breadboard and jumper wires

## Hardware Connections

- Button is connected to GPIO pin 2
- First LED (Button detection LED) is connected to GPIO pin 12
- Second LED (Synchronization task LED) is connected to GPIO pin 13
- LEDs are protected by 220Ω resistors

## Electrical Schematic

```
                  VCC
                   |
                   R
                   R 10kΩ (Pull-up)
                   |
                   |-----------|
                   |           |
    GPIO 2 --------+           |
(BUTTON_PIN)                   |
                             Button
                               |
                              GND
                              
                  VCC
                   |
                   R
                   R 220Ω
                   |
    GPIO 12 -------+----- LED 1 ----- GND
(BUTTON_LED_PIN)

                  VCC
                   |
                   R
                   R 220Ω
                   |
    GPIO 13 -------+----- LED 2 ----- GND
(SYNC_LED_PIN)
```

## Block Diagram

```
             +----------------+
             |                |
 Button ---->| Task 1:        |
 (GPIO 2)    | ButtonLedTask  |-----> LED 1 (GPIO 12)
             |                |
             +-------+--------+
                     |
                     | Binary Semaphore
                     | (buttonSemaphore)
                     v
             +-------+--------+
             |                |
             | Task 2:        |
             | SyncTask       |-----> LED 2 (GPIO 13)
             |                |
             +-------+--------+
                     |
                     | Queue
                     | (byteQueue)
                     v
             +-------+--------+
             |                |
             | Task 3:        |
             | AsyncTask      |-----> Serial Output
             |                |
             +----------------+
```

## Project Structure

- `config.h`: Contains pin definitions and constants
- `main.cpp`: Main application setup and FreeRTOS scheduler
- `task1_button.cpp`: Button monitoring task with LED feedback
- `task2_sync.cpp`: Synchronization task with counter-based LED blinking
- `task3_async.cpp`: Asynchronous task for queue reading and serial output
- `sync_objects.cpp`: FreeRTOS synchronization objects (semaphore, queue)
- `tasks.h`: Task function declarations

## Functionality

1. **Task 1 (Button Task)**:
   - Checks the button state every 10ms
   - When the button is pressed, turns on LED 1 for 1 second
   - Signals Task 2 via a binary semaphore

2. **Task 2 (Sync Task)**:
   - Waits indefinitely for the semaphore from Task 1
   - Increments a counter (N) when the semaphore is received
   - Sends N bytes (sequence: 1,2,3...N) into a queue
   - Blinks LED 2 N times with specific ON/OFF intervals

3. **Task 3 (Async Task)**:
   - Every 200ms, reads data from the queue
   - Outputs each byte to the serial terminal with formatting
   - Moves to a new line when encountering byte "0"

## Additional Enhancement

A mutex is implemented to protect the serial output, preventing task outputs from overlapping in the serial console.

## Note on Installation

Upload this sketch to your ESP32 or Arduino-compatible board using the Arduino IDE. Ensure you have installed the necessary FreeRTOS libraries for your specific board. 