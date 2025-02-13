#define TEENSY_OUT 11  // Teensy sends signal to Nano
#define TEENSY_IN  12  // Teensy receives signal from Nano

float fwdSpeedAlert = 30;   // Dangerous speed for fwd
const int ledFWDPin = 41;   // Channel 1 for FWD Drive Engage

void nanoInit() {
  Serial.begin(115200);
  if (!mpu.begin()) {
    Serial.println("Failed to find Arduino Nano. Check wiring!");
    while (1);
  }
  pinMode(TEENSY_OUT, OUTPUT);
  pinMode(TEENSY_IN, INPUT);
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
    analogWrite(ledFWDPin, 80);       // LED brightness
  } else if (nanoSignal == LOW) {
    analogWrite(ledFWDPin, 0);        // Turn off LED
  }
}