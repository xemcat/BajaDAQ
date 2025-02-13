#include <Adafruit_Sensor.h>
#include <Adafruit_MLX90614.h>
#include <Adafruit_MPU6050.h>
#include <Wire.h>
#include <math.h>
#include <SD.h>
#include <TimeLib.h>
#include <RTClib.h>

// Declare global sensor objects and files
Adafruit_MLX90614 mlx = Adafruit_MLX90614();
Adafruit_MPU6050 mpu = Adafruit_MPU6050();
RTC_DS3231 rtc;
String fileName;

// int steps[8][4] = {
//   {LOW, LOW, LOW, HIGH},  // Step 1
//   {LOW, LOW, HIGH, HIGH}, // Step 2
//   {LOW, LOW, HIGH, LOW},  // Step 3
//   {LOW, HIGH, HIGH, LOW}, // Step 4
//   {LOW, HIGH, LOW, LOW},  // Step 5
//   {HIGH, HIGH, LOW, LOW}, // Step 6
//   {HIGH, LOW, LOW, LOW},  // Step 7
//   {HIGH, LOW, LOW, HIGH}  // Step 8
// };

// Define Data Switch
const int switchPin = 2;       // Use pin 2 for the switch
bool collectingData = false;   // Flag for data collection
unsigned long startTime = 0;   // Store the start time

// Forward declarations
void mlxInit();             // CVT Temperature
void calculateMLXData();
void mpuInit();             // Vehicle Orientation
void calculateMPUData();
void displayMPUData();
void rifeInit();            // Portal and Gearbox Oil Temperatures
void calculateRifeData();
void hallInit();            // Vehicle Speed
void calculateHallData();
void displayHallData();
//void stepperInit();         // Stepper Motor and Limit Switch
//void rotateMotor(int stepsToMove, bool forward);
//void stopMotor();
//void processSensorInput(float vehicleSpeed);
//void controlMotor();
//void rotateMotor();
//void stopMotor();
//void nanoInit();            // Arduino Nano
//void nanoCommunication();

void setup() {
  Serial.begin(9600);
  while (!Serial);
  Serial.println("Starting Project...");
  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while(1);
  }
  Serial.println("Initializing");
  mlxInit();          // Initialize MLX90614
  Serial.println("MLX done");
  mpuInit();          // Initialize MPU6050
  Serial.println("MPU done");
  rifeInit();         // Initialize RIFE Portal
  Serial.println("RIFE done");
  hallInit();         // Initialize Hall Effect
  Serial.println("HALL done");
  //stepperInit();      // Initialize Stepper and Limit Switch
  //Serial.println("STEP done");

  fileName = generateFileName();
  initDataLogger(fileName);
  pinMode(switchPin, INPUT_PULLUP);     // Configure switch pin with pull-up
  Serial.println("Flip the switch to start or stop data collection.");
}

void loop() {
  float vehicleSpeed = getVehicleSpeed();  // Get speed from Hall sensor
  //processSensorInput(vehicleSpeed);        // Pass speed to stepper control

  int switchState = digitalRead(switchPin); // Read the switch state
  if (switchState == LOW) {
    if (!collectingData) {
      collectingData = true;
      startTime = millis();
      Serial.println("Data collection started."); 
    }
  } else {
    if (collectingData) {
      collectingData = false;
      Serial.println("Data collection stopped.");
    }
  }

  if (collectingData) {
    calculateMLXData();
    calculateMPUData();
    displayMPUData();
    calculateRifeData();
    calculateHallData();
    displayHallData();

    float cvtTemp = getCVTTemp();
    float portalTemp = getPortalTemp();
    float gearboxTemp = getGearboxTemp();
    float pitch = getVehiclePitch();
    float roll = getVehicleRoll();
    unsigned long timeElapsed = millis() - startTime;

    logData(fileName, timeElapsed, cvtTemp, portalTemp, gearboxTemp, pitch, roll, vehicleSpeed);
    delay(1000);
  }
}
