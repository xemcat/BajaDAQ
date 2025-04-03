# Baja Vehicle Data Logger

This project is a data logging and monitoring system designed for a Baja-style vehicle, built for real-time telemetry in rugged environments. It leverages a Teensy microcontroller to collect data from temperature sensors (CVT belt, portal oil, gearbox oil), a Hall effect sensor (speed), and an MPU6050 IMU (orientation), logging it to an SD card with timestamps from a DS3231 RTC. The system also drives LEDs for temperature alerts, communicates with an Arduino Nano for additional signaling, and uses a stepper motor to mechanically display vehicle speed.

## License

This project, the **2024-2025 Union Baja SAE: Data Acquisition and Display System for Increased Vehicle Performance**, is shared under the "Baja Open Garage License" (BOGL) — an unofficial, homebrewed creation:

- **Use It**: Feel free to use this code and hardware design for your own Baja projects, personal experiments, or educational purposes.
- **Share It**: You can share it with your friends, team, or community—just give a shoutout to me (xemcat) and this repo.
- **Modify It**: Hack it, tweak it, make it better! I’d love to hear about your improvements.
- **No Warranty**: This comes as-is. If your vehicle catches fire or your stepper motor spins into another dimension, I’m not responsible.
- **Have Fun**: The spirit of Baja is ingenuity and adventure—keep that alive!

Not an official legal license, just my way of saying "enjoy, but play nice."

If you have any questions, feel free to email me: emcatwatson@yahoo.com

## Features
- **Temperature Monitoring**: Measures CVT belt temperature (MLX90614 IR sensor) and portal/gearbox oil temperatures (RIFE sensors), with LED alerts for thresholds.
- **Vehicle Speed**: Calculates speed using a Hall effect sensor and displays it via a stepper motor (calibrated to mph).
- **Orientation**: Tracks pitch and roll with an MPU6050 IMU.
- **Data Logging**: Saves data to CSV files on an SD card, named by date and time (e.g., `04.03_14.30.csv`).
- **Nano Integration**: Interfaces with an Arduino Nano for speed-based signaling.
- **Switch Control**: Toggle data collection with a physical switch; limit switch calibrates the stepper motor.

## Major Hardware Requirements
- **Teensy Microcontroller** (e.g., Teensy 4.1)
- **Sensors**:
  - Adafruit MLX90614 (IR temperature sensor for CVT belt)
  - RIFE temperature sensors (for portal and gearbox oil)
  - Hall effect sensor (vehicle speed)
  - Adafruit MPU6050 (IMU for pitch and roll)
- **DS3231 RTC** (real-time clock for timestamps)
- **Stepper Motor** (28BYJ-48 with driver, for speed display)
- **LEDs** (for temperature alerts: CVT, portal, gearbox, FWD)
- **Limit Switch** (for stepper motor zeroing)
- **Toggle Switch** (to start/stop data logging)

## Wiring and PCB
- **Wiring Schematic**:![SystemSchematicOmittingPowerSource](https://github.com/user-attachments/assets/e3470d09-46ee-47d3-aef3-81f8f32e0b75)
- **PCB Files**: ![Motherboard](https://github.com/user-attachments/assets/9e02e9e3-6c23-4ab5-a4cf-af0e8467886d)
![PowerDaughterboard](https://github.com/user-attachments/assets/6b2b460f-a38e-4cf2-acb9-78c2a5274235)
![DashboardDaughterboard](https://github.com/user-attachments/assets/8c1adf21-13ab-404e-9177-b3ebb3008efb)
