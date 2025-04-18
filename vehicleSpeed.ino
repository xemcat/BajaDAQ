/*
 * 2024-2025 Union Baja SAE: Data Acquisition and Display System for Increased Vehicle Performance
 * File: vehicleSpeed.ino
 * Author: xemcat
 * 
 * This code is released under the "Baja Open Garage License" (BOGL):
 * - Use it for your Baja builds, learning, or tinkering.
 * - Share it with credit to xemcat and the GitHub repo.
 * - Modify it freely—let me know if you make it cooler!
 * - No guarantees; use at your own risk.
 * - Keep the Baja spirit: build, break, repeat.
 */

#define hallSensorPin 6                     // Pin connected to the Hall effect sensor 
volatile unsigned long lastPulseTime = 0;   // Timestamp of the last pulse
volatile unsigned long pulseInterval = 0;   // Time between two consecutive pulses
float wheelCircumference = 2.1;             // Circumference of the wheel in meters
const int pulsesPerWheel = 12;
float vehicleSpeed = 0.0;                   // Vehicle speed in mph
// unsigned long lastCalculationTime = 0;      // Timestamp for the last speed calculation
unsigned long timeout = 1000000;            // 1 second timeout (in microseconds)

volatile unsigned long magnetCount = 0;     // Magnet count for pulse detection
const unsigned long debounceDelay = 5000;   // 5 ms debounce
volatile unsigned long lastInterruptTime = 0; // Last interrupt time for debounce

// Interrupt service routine (ISR) for the Hall sensor with debounce
void hallSensorISR() {
  unsigned long currentTime = micros(); // Get the current time in microseconds

  // Check if enough time has passed since the last interrupt
  if (currentTime - lastInterruptTime > debounceDelay) {
    if (currentTime > lastPulseTime) {              // Prevent rollover issues
      pulseInterval = currentTime - lastPulseTime;  // Time difference between pulses
      lastPulseTime = currentTime;                  // Update the last pulse time
      magnetCount++;                                // Increment the magnet count
    }
    lastInterruptTime = currentTime;  // Update the last interrupt time
  }
}

void hallInit() {
  pinMode(hallSensorPin, INPUT_PULLUP);           // Configure the Hall sensor pin
  attachInterrupt(digitalPinToInterrupt(hallSensorPin), hallSensorISR, FALLING); // Attach interrupt
  if (!digitalRead(hallSensorPin)) {
    Serial.println("Error: Could not find a valid Hall Effect sensor!");
    // while (1);                            // Stop execution if initialization fails
  }
}

void calculateHallData() {
  unsigned long currentTime = micros();
  // If no pulse detected within the timeout period, assume the vehicle is stationary
  if (currentTime - lastPulseTime > timeout) {
    vehicleSpeed = 0.0;
    return;
  }
  if (pulseInterval > 0) {
    // Calculate speed: speed (m/s) = circumference / time per revolution
    float distancePerPulse = wheelCircumference / pulsesPerWheel;
    vehicleSpeed = (distancePerPulse / (pulseInterval / 1e6)); // m/s
    // Convert to mph: 1 m/s = 2.23694 mph
    vehicleSpeed *= 2.23694;
  } else {
    vehicleSpeed = 0.0; // No valid pulse interval
  }
}

void displayHallData() {
  Serial.println("Vehicle Speed:");
  Serial.print("   Speed: "); Serial.print(vehicleSpeed); Serial.println(" mph");
  Serial.print("   Magnet Count: "); Serial.println(magnetCount);
}

float getVehicleSpeed() {
  return vehicleSpeed;
}
