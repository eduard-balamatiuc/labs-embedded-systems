/*
 * FreeRTOS Multi-Task Button LED Application
 * 
 * This sketch demonstrates a modular FreeRTOS application with three tasks:
 * 1. Button monitoring task with LED feedback
 * 2. Synchronization task that blinks an LED N times
 * 3. Asynchronous task for serial output
 * 
 * Hardware:
 * - Button on GPIO 2
 * - Button LED on GPIO 12
 * - Sync LED on GPIO 13
 * 
 * Required Libraries:
 * - Arduino FreeRTOS Library by Richard Barry (install via Arduino Library Manager)
 * 
 * The actual implementation is in the .cpp files:
 * - main.cpp: Main application setup and task creation
 * - task1_button.cpp: Button monitoring and LED control
 * - task2_sync.cpp: Synchronization and counter-based LED blinking
 * - task3_async.cpp: Queue reading and serial output
 * - sync_objects.cpp: FreeRTOS synchronization objects
 */

// No code needed here as Arduino will automatically include
// and compile all .cpp and .h files in the sketch folder 