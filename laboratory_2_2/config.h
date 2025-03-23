#ifndef CONFIG_H
#define CONFIG_H

// GPIO Pin Definitions
#define BUTTON_PIN          2
#define BUTTON_LED_PIN      12
#define SYNC_LED_PIN        13

// Task Delays (in milliseconds)
#define BUTTON_CHECK_DELAY  50     // Increased from 10ms for more stability
#define BUTTON_LED_ON_TIME  1000
#define SYNC_TASK_DELAY     100    // Increased from 50ms
#define ASYNC_TASK_DELAY    500    // Increased from 200ms to reduce serial traffic

// LED Blink Intervals (in milliseconds)
#define SYNC_LED_ON_TIME    500    // Increased from 300ms - more visible
#define SYNC_LED_OFF_TIME   700    // Increased from 500ms - more visible

// Button Debounce
#define DEBOUNCE_DELAY      100    // Increased from 50ms for better debouncing

// FreeRTOS Configuration
#define QUEUE_SIZE          10
#define BUTTON_TASK_PRIORITY      1
#define SYNC_TASK_PRIORITY        2
#define ASYNC_TASK_PRIORITY       1
#define BUTTON_TASK_STACK_SIZE    2048
#define SYNC_TASK_STACK_SIZE      2048
#define ASYNC_TASK_STACK_SIZE     2048

#endif // CONFIG_H 