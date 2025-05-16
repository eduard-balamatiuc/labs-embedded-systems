# Fan PID Control System

This project implements a PID controller for a PC fan, measuring and controlling RPM in real-time using the Arduino platform.

## Hardware Requirements

- Arduino Uno or compatible board
- 12V PC fan with 4 pins
  - Black: GND
  - Red: +12V power
  - Yellow: Tachometer signal (RPM sensor)
  - Blue: PWM control input
- 10kΩ pull-up resistor for tachometer signal
- Power supply (12V for fan, 5V for Arduino)
- Optional: LED and 220Ω resistor for alarm indication

## Wiring Diagram

```
PC Fan 4-Pin Connections:
- Black wire → GND
- Red wire → External 12V supply
- Yellow wire → Digital pin 2 (through 10kΩ pull-up resistor)
- Blue wire → Digital pin 9 (PWM output)

Optional:
- LED anode → Digital pin 13 (or built-in LED)
- LED cathode → GND (through 220Ω resistor)
```

## Features

- Real-time RPM measurement using tachometer input
- PID closed-loop control of fan speed
- Serial interface for:
  - Setting target RPM
  - Reading current system parameters
  - Running diagnostic tests
- Alarm indication when RPM deviates significantly from setpoint
- Data visualization using Arduino Serial Plotter

## Serial Commands

- `+`: Increase target RPM by 100
- `-`: Decrease target RPM by 100
- `rXXXX`: Set target RPM directly (e.g., r2000 sets target to 2000 RPM)
- `d`: Run diagnostic test
- `p`: Print current parameters
- `h`: Show help message

## Code Structure

This project is organized in a modular fashion:

- **main.cpp**: Main program flow and initialization
- **pid_control**: PID controller implementation
- **tachometer**: RPM measurement from tachometer input
- **fan_control**: PWM signal generation for fan speed control
- **serial_monitor**: Serial interface with printf support
- **user_interface**: User interaction through serial commands

## Using the Serial Plotter

To visualize the PID controller performance:
1. Upload the code to Arduino
2. Open Arduino IDE
3. Select Tools → Serial Plotter
4. Set baud rate to 9600
5. The plotter will display Setpoint, RPM, and PWM values in real-time

## PID Tuning

The default PID parameters are:
- Kp = 0.5
- Ki = 0.2
- Kd = 0.05

These values can be adjusted in main.cpp for optimal performance with your specific fan. 