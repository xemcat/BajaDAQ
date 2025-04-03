/*
 * 2024-2025 Union Baja SAE: Data Acquisition and Display System for Increased Vehicle Performance
 * File: cvtBeltTemp.ino
 * Author: xemcat
 * 
 * This code is released under the "Baja Open Garage License" (BOGL):
 * - Use it for your Baja builds, learning, or tinkering.
 * - Share it with credit to xemcat and the GitHub repo.
 * - Modify it freely—let me know if you make it cooler!
 * - No guarantees; use at your own risk.
 * - Keep the Baja spirit: build, break, repeat.
 */

extern Adafruit_MLX90614 mlx;

const int ledCVTPin = 3;             // Channel 1 for CVT Belt Temp
const float TEMP_CVT_ON = 5.0;      // CVT Belt Temp threshold ON (°F)  CHANGE TO 285
const float TEMP_CVT_OFF = 4.0;     // CVT Belt Temp threshold OFF (°F) CHANGE TO 245
float cvtTemp = 0.0;                 // Globally define cvtTemp
float ambientTemp = 0.0;             // Globally define ambientTemp
bool blinkCVT = false;
int blinkCVTCount = 0;

void mlxInit() {
  if (!mlx.begin()) {
    Serial.println("Error: Could not find a valid MLX90614 sensor!");
  //   while (1);                     // Stop execution if initialization fails
  }
  pinMode(ledCVTPin, OUTPUT);
  analogWrite(ledCVTPin, 0);         // Ensure LED is off at the start
  }

void calculateMLXData() {
  float cvtTempC = mlx.readObjectTempC();
  cvtTemp = (cvtTempC * 9.0 / 5.0) + 32.0;
  float ambientTempC = mlx.readAmbientTempC();
  ambientTemp = (ambientTempC * 9.0 / 5.0) + 32.0;
  
  Serial.println("CVT: ");
  Serial.print("   Object Temperature: "); Serial.print(cvtTemp); Serial.println(" °F");
  Serial.print("   Ambient Temperature: "); Serial.print(ambientTemp); Serial.println(" °F");

  if (cvtTemp > TEMP_CVT_ON) {
    analogWrite(ledCVTPin, 8);
    Serial.println("CVT LED ON");
  } else if (cvtTemp < TEMP_CVT_ON && cvtTemp > TEMP_CVT_OFF) {
    if (!blinkCVT) {
      analogWrite(ledCVTPin, 8);
      blinkCVTCount = blinkCVTCount + 1;
      if (blinkCVTCount == 100) {
        blinkCVT = true;
      }
    }
    if (blinkCVT) {
      analogWrite(ledCVTPin, 0);
      blinkCVTCount = blinkCVTCount - 1;
      if (blinkCVTCount == 0) {
        blinkCVT = false;
      }
    }
  } else if (cvtTemp < TEMP_CVT_OFF) {
    analogWrite(ledCVTPin, 0);
  }
}

float getCVTTemp() {
  return cvtTemp;
}
