/*
 * 2024-2025 Union Baja SAE: Data Acquisition and Display System for Increased Vehicle Performance
 * File: nanoCommunication.ino
 * Author: xemcat
 * 
 * This code is released under the "Baja Open Garage License" (BOGL):
 * - Use it for your Baja builds, learning, or tinkering.
 * - Share it with credit to xemcat and the GitHub repo.
 * - Modify it freely—let me know if you make it cooler!
 * - No guarantees; use at your own risk.
 * - Keep the Baja spirit: build, break, repeat.
 */

#define TEENSY_OUT 11  // Teensy sends signal to Nano
#define TEENSY_IN  12  // Teensy receives signal from Nano

float fwdSpeedAlert = 25;   // Dangerous speed for fwd 
const int ledFWDPin = 13;   // Channel 1 for FWD Drive Engage

void nanoInit() {
  pinMode(TEENSY_OUT, OUTPUT);
  pinMode(TEENSY_IN, INPUT);
  analogWrite(ledFWDPin, 0);
}

void nanoCommunication() {
  float fwdSpeed = getVehicleSpeed();
  if (fwdSpeed > fwdSpeedAlert) {
      digitalWrite(TEENSY_OUT, HIGH);  // Send HIGH if speed > 30
  } else {
      digitalWrite(TEENSY_OUT, LOW);   // Send LOW otherwise
  }
  // Read signal from Nano
  int nanoSignal = digitalRead(TEENSY_IN);
  Serial.print("Nano Signal: ");
  Serial.println(nanoSignal);
  
  if (nanoSignal == HIGH) {
    analogWrite(ledFWDPin, 0);       // LED brightness
    Serial.print("Nano LED OFF");
  } else if (nanoSignal == LOW) {
    analogWrite(ledFWDPin, 15);        // Turn off LED
    Serial.println("Nano LED ON");
  }
}
