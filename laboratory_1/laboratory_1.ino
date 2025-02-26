#include <stdio.h>
#include "led_control.h"
#include "printf_init.h"

char command[20];

void setup() {
    pinMode(13, OUTPUT);
    Serial.begin(9600);
    printf_init();
}

void loop() {
    if (Serial.available() > 0) {
        scanf("%19s", command); // Read up to 19 characters to prevent buffer overflow
        
        if (strcmp(command, "led_on") == 0) {
            turnOnLED();
        } else if (strcmp(command, "led_off") == 0) {
            turnOffLED();
        } else {
            printf("Invalid command. Use 'led_on' or 'led_off'\n");
        }
    }
}