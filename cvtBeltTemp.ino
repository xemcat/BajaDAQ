extern Adafruit_MLX90614 mlx;

const int ledCVTPin = 3;             // Channel 1 for CVT Belt Temp
const float TEMP_CVT_ON = 71.0;      // CVT Belt Temp threshold ON (°F)  CHANGE TO 285
const float TEMP_CVT_OFF = 68.0;     // CVT Belt Temp threshold OFF (°F) CHANGE TO 244
float cvtTemp = 0.0;                 // Globally define cvtTemp
float ambientTemp = 0.0;             // Globally define ambientTemp
bool blinkCVT = false;
int blinkCVTCount = 0;

void mlxInit() {
  if (!mlx.begin()) {
    Serial.println("Error: Could not find a valid MLX90614 sensor!");
    //while (1);                     // Stop execution if initialization fails
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
    analogWrite(ledCVTPin, 15);
  } else if (cvtTemp < TEMP_CVT_ON && cvtTemp > TEMP_CVT_OFF) {
    if (!blinkCVT) {
      analogWrite(ledCVTPin, 15);
      blinkCVTCount = blinkCVTCount + 1;
      if (blinkCVTCount == 5) {
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