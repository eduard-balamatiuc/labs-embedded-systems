#include <Arduino.h>
#include <stdio.h>

// Redirect stdout to Serial
extern "C" {
  int _write(int file, char *ptr, int len) {
    // Send characters to Serial port
    for (int i = 0; i < len; i++) {
      Serial.write(ptr[i]);
    }
    return len;
  }
} 