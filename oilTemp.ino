// Portal Temp
const int tempPortalPin = A0;
const int ledPortalPin = 4;             // Channel 2 for Portal Oil Temp
const float TEMP_PORTAL_ON = 70.0;      // Portal Temp threshold ON (°F)   CHANGE TO 285
const float TEMP_PORTAL_OFF = 60.0;     // Portal Temp threshold OFF (°F)  CHANGE TO 245
float portalTemp = 0.0;                 // Globally define portalTemp
// Gearbox Temp
const int tempGearboxPin = A1;
const int ledGearboxPin = 5;            // Channel 3 for Gearbox Oil Temp
const float TEMP_GEARBOX_ON = 70.0;     // Gearbox Temp threshold ON (°F)  CHANGE TO 285
const float TEMP_GEARBOX_OFF = 60.0;    // Gearbox Temp threshold OFF (°F) CHANGE TO 245
float gearboxTemp = 0.0;                // Globally define gearboxTemp
// Lookup Table
const int lookupTableSize = 32;
const float rifeTemperatures[lookupTableSize] = {-20, -10, 0, 10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 110, 120, 130, 140, 150, 160, 170, 180, 190, 200, 210, 220, 230, 240, 250, 260, 270, 280, 290};
const float rifeResistances[lookupTableSize] = {189726, 132092, 93425, 67059, 48804, 35983, 26855, 20274, 15473, 11929, 9287, 7295, 5781, 4618, 3718, 3016, 2463, 2025, 1675, 1395, 1167, 983, 832, 707, 604, 519, 447, 387, 336, 294, 257, 226};
// Circuit Values
const float inputVoltage = 3.01;            // Input voltage for sensor of 3.01V actual
const float seriesResistancePortal = 983;   // Series portal resistor value of 1kΩ actual
const float seriesResistanceGearbox = 983;  // Series gearbox resistor value of 1kΩ actual
const float analogMax = 1023;               // Max analog value read from sensor

void rifeInit() {
  if (!analogRead(tempPortalPin)) {
    Serial.println("Error: Could not find a valid RIFE portal sensor!");
    //while (1);                              // Stop execution if initialization fails
  }
  // Portal Temp
  pinMode(ledPortalPin, OUTPUT);
  analogWrite(ledPortalPin, 0);             // Ensure LED is off at the start
  // Gearbox Temp
  pinMode(ledGearboxPin, OUTPUT);
  analogWrite(ledGearboxPin, 0);            // Ensure LED is off at the start
}

float interpolatePortalTemperature(float portalResistance) {
  for (int i = 0; i < lookupTableSize - 1; i++) {
    if (portalResistance >= rifeResistances[i] && portalResistance <= rifeResistances[i - 1]) {
      float portalTempDifference = rifeTemperatures[i] - rifeTemperatures[i + 1];
      float portalResistanceDifference = rifeResistances[i - 1] - rifeResistances[i];
      float portalResistanceOffset = portalResistance - rifeResistances[i];
      return rifeTemperatures[i] + (portalResistanceOffset / portalResistanceDifference) * portalTempDifference;
    }
  }
  return -999.0;
}

float interpolateGearboxTemperature(float gearboxResistance) {
  for (int n = 0; n < lookupTableSize - 1; n++) {
    if (gearboxResistance >= rifeResistances[n] && gearboxResistance <= rifeResistances[n - 1]) {
      float gearboxTempDifference = rifeTemperatures[n] - rifeTemperatures[n + 1];
      float gearboxResistanceDifference = rifeResistances[n - 1] - rifeResistances[n];
      float gearboxResistanceOffset = gearboxResistance - rifeResistances[n];
      return rifeTemperatures[n] + (gearboxResistanceOffset / gearboxResistanceDifference) * gearboxTempDifference;
    }
  }
  return -999.0;
}

void calculateRifeData() {
  //Portal Temp
  int sensorPortalValue = analogRead(tempPortalPin);
  float portalVoltage = sensorPortalValue * (inputVoltage / analogMax);          // To voltage 0 - 3.3V
  float portalCurrent = (inputVoltage - portalVoltage) / seriesResistancePortal; // Find current
  float portalResistance = portalVoltage / portalCurrent;                        // Find variable resistance
  portalTemp = interpolatePortalTemperature(portalResistance);                   // Get temperature from resistance
  //Gearbox Temp
  int sensorGearboxValue = analogRead(tempGearboxPin);
  float gearboxVoltage = sensorGearboxValue * (inputVoltage / analogMax);           // To voltage 0 - 3.3V
  float gearboxCurrent = (inputVoltage - gearboxVoltage) / seriesResistanceGearbox; // Find current
  float gearboxResistance = gearboxVoltage / gearboxCurrent;                        // Find variable resistance
  gearboxTemp = interpolateGearboxTemperature(gearboxResistance);                   // Get temperature from resistance

  Serial.println("Portal: ");
  Serial.print("   Voltage: "); Serial.print(portalVoltage); Serial.print(" V    "); Serial.print("Resistance: "); Serial.print(portalResistance); Serial.println(" Ω");
  Serial.print("   Temperature: "); Serial.print(portalTemp); Serial.println(" °F");
  Serial.println("Gearbox: ");
  Serial.print("   Voltage: "); Serial.print(gearboxVoltage); Serial.print(" V    "); Serial.print("Resistance: "); Serial.print(gearboxResistance); Serial.println(" Ω");
  Serial.print("   Temperature: "); Serial.print(gearboxTemp); Serial.println(" °F");
 
  // Portal Temp
  if (portalTemp > TEMP_PORTAL_ON) {
    analogWrite(ledPortalPin, 15);
  // } else if (portalTemp < TEMP_PORTAL_ON && portalTemp > TEMP_PORTAL_OFF) {
  //   analogWrite(ledPortalPin, 15);
  //   delay(10);
  //   analogWrite(ledPortalPin, 0);
  } else if (portalTemp < TEMP_PORTAL_OFF) {
    analogWrite(ledPortalPin, 0);      
  }

  // Gearbox Temp
  if (gearboxTemp > TEMP_GEARBOX_ON) {
    analogWrite(ledGearboxPin, 15);
  // } else if (gearboxTemp < TEMP_GEARBOX_ON && gearboxTemp > TEMP_GEARBOX_OFF) {
  //   analogWrite(ledGearboxPin, 15);
  //   delay(10);
  //   analogWrite(ledGearboxPin, 0);
  } else if (gearboxTemp < TEMP_GEARBOX_OFF) {
    analogWrite(ledGearboxPin, 0);
  }
}

float getPortalTemp() {
  return portalTemp;
}

float getGearboxTemp() {
  return gearboxTemp;
}
