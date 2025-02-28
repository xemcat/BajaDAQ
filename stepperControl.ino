#define IN1_PIN 7
#define IN2_PIN 8
#define IN3_PIN 9
#define IN4_PIN 10
#define LIMIT_SWITCH_PIN 20  // Limit switch input pin

int steps[4][4] = {  // Extended to 8 steps for a full cycle (2 rotations of 4 steps)
  {HIGH, LOW, LOW, LOW},  // Step 1
  {LOW, HIGH, LOW, LOW},  // Step 2
  {LOW, LOW, HIGH, LOW},  // Step 3
  {LOW, LOW, LOW, HIGH},  // Step 4
};

const int stepMultiplier = 32;  // 8 steps = 1 mph
const unsigned long stepInterval = 9;  // 20 ms between bursts
int currentStepPosition = 0;   // Current position of the stepper in steps
int targetStepPosition = 0;    // Target position based on vehicle speed
unsigned long lastStepTime = 0; // Time of the last step burst
int stepIndex = 0;             // Current step in the sequence (0-7)

// Declare external variables from mainBaja.ino
extern bool collectingData;

// Initialize the stepper motor and calibrate to 0 mph
void stepperInit() {
  pinMode(IN1_PIN, OUTPUT);
  pinMode(IN2_PIN, OUTPUT);
  pinMode(IN3_PIN, OUTPUT);
  pinMode(IN4_PIN, OUTPUT);
  pinMode(LIMIT_SWITCH_PIN, INPUT_PULLUP); // Internal pull-up resistor

  Serial.println("Moving to 0mph...");
  while (digitalRead(LIMIT_SWITCH_PIN) == HIGH) {
    moveEightSteps(false); // Move counterclockwise (false)
    delay(2);              // Small delay during calibration
  }
  stopMotor();
  currentStepPosition = 0;
  targetStepPosition = 0;
  Serial.println("Limit switch reached, motor stopped.");
}

// Move the motor 8 steps: true = clockwise, false = counterclockwise
void moveEightSteps(bool forward) {
  if (forward) {
    for (int i = 0; i < 8; i++) {
      for (int j = 0; j < 4; j++) {
        digitalWrite(IN1_PIN, steps[j][3]);
        digitalWrite(IN2_PIN, steps[j][2]);
        digitalWrite(IN3_PIN, steps[j][1]);
        digitalWrite(IN4_PIN, steps[j][0]);
        delay(2);
      }
    }
    Serial.println("Moved 1 mph clockwise");
  }
  else {
    for (int i = 7; i > -1; i--) {
      for (int j = 3; j > -1; j--) {
        digitalWrite(IN1_PIN, steps[j][3]);
        digitalWrite(IN2_PIN, steps[j][2]);
        digitalWrite(IN3_PIN, steps[j][1]);
        digitalWrite(IN4_PIN, steps[j][0]);
        delay(2);
      }
    }
    Serial.println("Moved 1 mph counterclockwise");
  }
}

void stopMotor() {
  digitalWrite(IN1_PIN, LOW);
  digitalWrite(IN2_PIN, LOW);
  digitalWrite(IN3_PIN, LOW);
  digitalWrite(IN4_PIN, LOW);
}

// Update stepper position based on vehicle speed, moving 8 steps at a time
void processSensorInput(float vehicleSpeed) {
  targetStepPosition = round(vehicleSpeed) * stepMultiplier; // e.g.,

  Serial.print("vehicleSpeed: "); Serial.print(vehicleSpeed);
  Serial.print(" | targetStepPosition: "); Serial.print(targetStepPosition);
  Serial.print(" | currentStepPosition: "); Serial.println(currentStepPosition);

  unsigned long currentTime = millis();
  while (currentStepPosition < targetStepPosition) {
    moveEightSteps(true); // Clockwise (true)
    currentStepPosition += 32;
    lastStepTime = currentTime;
    //Serial.println("Increased by 8 steps clockwise");
  }
  while (currentStepPosition > targetStepPosition) {
    moveEightSteps(false); // Counterclockwise (false)
    currentStepPosition -= 32;
    lastStepTime = currentTime;
    //Serial.println("Decreased by 8 steps counterclockwise");
  }
}