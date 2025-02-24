// #define IN1_PIN 7
// #define IN2_PIN 8
// #define IN3_PIN 9
// #define IN4_PIN 10
// #define LIMIT_SWITCH_PIN 20  // Limit switch input pin

// int steps[8][4] = {
//   {LOW, LOW, LOW, HIGH},  // Step 1
//   {LOW, LOW, HIGH, LOW},  // Step 2
//   {LOW, HIGH, LOW, LOW},  // Step 3
//   {HIGH, LOW, LOW, LOW},  // Step 4
// };

// const int stepMultiplier = 8;  // 8 steps for each mph
// float previousSpeed = 0.0;  // Store the last speed reading

// void stepperInit() {
//   pinMode(IN1_PIN, OUTPUT);
//   pinMode(IN2_PIN, OUTPUT);
//   pinMode(IN3_PIN, OUTPUT);
//   pinMode(IN4_PIN, OUTPUT);
//   pinMode(LIMIT_SWITCH_PIN, INPUT_PULLUP); // Internal pull-up resistor

//   Serial.println("Moving to home position...");

//   // Move motor counterclockwise until the limit switch is pressed
//   while (digitalRead(LIMIT_SWITCH_PIN) == HIGH) {
//     rotateMotor(40, false); // Move step-by-step counterclockwise
//   }
//   stopMotor();
//   Serial.println("Limit switch reached, motor stopped.");
//   previousSpeed = 0.0; // Reset speed reference
// }

// void rotateMotor(int stepsToMove, bool forward) {
//   for (int i = 0; i < stepsToMove; i++) {
//     int startStep = forward ? 0 : 3;
//     int stepIncrement = forward ? 1 : -1;

//     for (int step = startStep; (forward ? step < 4 : step >= 0); step += stepIncrement) {
//       digitalWrite(IN1_PIN, steps[step][0]);
//       digitalWrite(IN2_PIN, steps[step][1]);
//       digitalWrite(IN3_PIN, steps[step][2]);
//       digitalWrite(IN4_PIN, steps[step][3]);
//       delay(2);  // Small delay to allow motor movement
//     }
//   }
// }

// void stopMotor() {
//   digitalWrite(IN1_PIN, LOW);
//   digitalWrite(IN2_PIN, LOW);
//   digitalWrite(IN3_PIN, LOW);
//   digitalWrite(IN4_PIN, LOW);
// }

// // Function to process vehicle speed and move motor accordingly
// void processSensorInput(float vehicleSpeed) {
//   int currentSteps = round(vehicleSpeed * stepMultiplier);
//   int previousSteps = round(previousSpeed * stepMultiplier);
//   int stepDifference = currentSteps - previousSteps;

//   if (stepDifference > 0) {
//     Serial.print("Moving clockwise: ");
//     Serial.println(stepDifference);
//     rotateMotor(stepDifference, true);
//   } else if (stepDifference < 0) {
//     Serial.print("Moving counterclockwise: ");
//     Serial.println(-stepDifference);
//     rotateMotor(-stepDifference, false);
//   }

//   previousSpeed = vehicleSpeed; // Update previous speed
// }