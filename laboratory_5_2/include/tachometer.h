#ifndef TACHOMETER_H
#define TACHOMETER_H

// Initialize tachometer
void Tachometer_Init(int tachPin);

// Get current RPM
int Tachometer_GetRPM();

#endif