# PID Position Control System - Modular Version

[Here](https://www.tinkercad.com/things/kRTIJ0ZCl6L-pid-position-control-system-modular-version?sharecode=w5TUWh1my_v9xY6lsh-w0ZV344dWPsa-EvmbFcdnJOg) you can find the Tinkercad Implementation.

This project implements a PID position control system for controlling a DC motor position using a potentiometer as both input and feedback. The code has been modularized for better maintainability and readability.

## Project Structure

The project is organized into the following modules:

- **Motor Driver Module**: Handles controlling the L298N motor driver
- **PID Controller Module**: Implements the PID control algorithm
- **UART Helpers Module**: Provides printf functionality for easier debugging

## Hardware Setup

- Connect L298N motor driver to Arduino:
  - Enable (EN) → Pin 9 (PWM)
  - Motor Control 1 (MC1) → Pin 3
  - Motor Control 2 (MC2) → Pin 2
  
- Connect potentiometers:
  - Feedback potentiometer → A0
  - Setpoint potentiometer → A1

## PID Tuning

The PID controller uses the following default constants which can be adjusted in the main.cpp file:
- KP = 2.0 (Proportional gain)
- KI = 0.01 (Integral gain)
- KD = 0.5 (Derivative gain)

## Building and Uploading

This project uses PlatformIO for development. To build and upload:

1. Open the project in PlatformIO
2. Click the "Upload" button
3. Open the Serial Monitor to view real-time data

## Serial Output

The system outputs real-time data in CSV format:
```
SetPoint,CurrentPosition,MotorOutput
```

This can be used with the Arduino Serial Plotter to visualize the PID control performance. 