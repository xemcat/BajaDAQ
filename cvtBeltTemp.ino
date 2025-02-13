extern Adafruit_MLX90614 mlx;

const int ledCVTPin = 3;              // Channel 1 for CVT Belt Temp
const float TEMP_CVT_ON = 50.0;       // CVT Belt Temp threshold ON (°F)  CHANGE TO 250
const float TEMP_CVT_OFF = 30.0;      // CVT Belt Temp threshold OFF (°F) CHANGE TO 245
float cvtTemp = 0.0;                  // Globally define cvtTemp
float ambientTemp = 0.0;              // Globally define ambientTemp

void mlxInit() {
  if (!mlx.begin()) {
    Serial.println("Error: Could not find a valid MLX90614 sensor!");
    while (1);                        // Stop execution if initialization fails
  }
  pinMode(ledCVTPin, OUTPUT);
  analogWrite(ledCVTPin, 0);          // Ensure LED is off at the start
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
    analogWrite(ledCVTPin, 80);       // LED brightness
  } else if (cvtTemp < TEMP_CVT_OFF) {
    analogWrite(ledCVTPin, 0);        // Turn off LED
  }
}

float getCVTTemp() {
  return cvtTemp;
}