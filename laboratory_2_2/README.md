# FreeRTOS Modular Embedded Application

This project demonstrates a modular embedded application using FreeRTOS on an Arduino-compatible microcontroller. The application showcases real-time scheduling, task synchronization using semaphores and mutexes, and inter-task communication via queues.

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


## Task Description

### Task 1: Button Task (Priority 1)
- Checks the button state every 10 ms
- When the button is pressed:
  - Acquires the serialMutex for exclusive access to serial port
  - Prints "Button pressed - Task 1 got mutex"
  - Turns LED1 ON for exactly 1 second
  - Signals a binary semaphore to trigger Task 2
  - Releases the serialMutex with a "Task 1 releasing mutex" message
  - Waits for 100 ms after releasing the mutex to give other tasks a chance
- Ensures precise timing using vTaskDelayUntil

### Task 2: Sequence Task (Priority 2)
- Waits for semaphore from Task 1
- Upon receiving the semaphore:
  - Acquires the serialMutex for exclusive access to serial port
  - Prints "Task 2 got mutex"
  - Increments counter N by 1 (initially N = 0)
  - Sends a sequence of bytes (1, 2, ..., N) to a buffer
  - Blinks LED2 exactly N times:
    - ON duration: 300 ms
    - OFF duration: 500 ms
  - Releases the serialMutex with a "Task 2 releasing mutex" message
  - Waits for 150 ms after releasing the mutex to give other tasks a chance
  - Adds a 50 ms delay after completing the blinking sequence

### Task 3: Output Task (Priority 3)
- Every 200 ms, reads data from the buffer
- When data is received:
  - Acquires the serialMutex for exclusive access to serial port
  - Prints "Task 3 got mutex"
  - Outputs the received bytes via serial port
  - Releases the serialMutex with a "Task 3 releasing mutex" message
  - Waits for 200 ms after releasing the mutex to give other tasks a chance
- When receiving a byte value of 0, prints a newline for clarity

## Synchronization Mechanisms

The application employs two main synchronization mechanisms:

### Binary Semaphore
- Used for task signaling between Task 1 (Button Task) and Task 2 (Sequence Task)
- Ensures Task 2 only executes its sequence when the button is pressed

### Mutex
- Used for resource protection and exclusive access to the serial port
- Prevents multiple tasks from writing to the serial port simultaneously
- Each task follows this pattern:
  1. Request mutex with `xSemaphoreTake(serialMutex, portMAX_DELAY)`
  2. Execute critical section (serial output operations)
  3. Release mutex with `xSemaphoreGive(serialMutex)`
  4. Wait for a specified delay period to prevent immediate reacquisition
- Delay periods after mutex release:
  - Task 1: 100 ms
  - Task 2: 150 ms
  - Task 3: 200 ms


## Building and Running

1. Open the project in PlatformIO
2. Build the project
3. Upload to your Arduino-compatible board
4. Open the serial monitor at 9600 baud to view the output

## Expected Serial Output

When running, the application will produce serial output like:

```
Serial communication initialized
Tasks initialized
Button pressed - Task 1 got mutex
Task 1 releasing mutex
Task 2 got mutex
Task 2 releasing mutex
Task 3 got mutex
Received data: 1 
Task 3 releasing mutex
Button pressed - Task 1 got mutex
Task 1 releasing mutex
Task 2 got mutex
Task 2 releasing mutex
Task 3 got mutex
Received data: 1 2 
Task 3 releasing mutex
```

This output demonstrates the mutex acquisition and release by each task, along with the task's specific operations.

## Dependencies

- Arduino framework
- FreeRTOS library 