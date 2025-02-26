#include <Arduino.h>

// Redirect printf to Serial
int serial_putchar(char c, FILE* f) {
    if (c == '\n') Serial.write('\r');
    return Serial.write(c);
}

// Redirect scanf to Serial
int serial_getchar(FILE* f) {
    while (!Serial.available());
    return Serial.read();
}

void printf_init() {
    fdevopen(&serial_putchar, &serial_getchar);
}