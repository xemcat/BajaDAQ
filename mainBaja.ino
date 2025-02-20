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

// Define Data Switch
const int dataSwitchPin = 2;       // Use pin 2 for the switch
bool collectingData = false;   // Flag for data collection
unsigned long startDataTime = 0;   // Store the start time

//const int systemTime = 0;

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
void stepperInit();         // Stepper Motor and Limit Switch
void rotateMotor(int stepsToMove, bool forward);
void stopMotor();
void processSensorInput(float vehicleSpeed);
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
  stepperInit();      // Initialize Stepper and Limit Switch
  Serial.println("STEP done");

  fileName = generateFileName();
  initDataLogger(fileName);
  pinMode(dataSwitchPin, INPUT_PULLUP);     // Configure switch pin with pull-up
  Serial.println("Flip the switch to start or stop data collection.");
}

void loop() {
  float vehicleSpeed = getVehicleSpeed();  // Get speed from Hall sensor
  processSensorInput(vehicleSpeed);        // Pass speed to stepper control
  int switchState = digitalRead(dataSwitchPin); // Read the switch state
  if (switchState == LOW) {
    if (!collectingData) {
      collectingData = true;
      startDataTime = millis();
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
    float speed = getVehicleSpeed();
    unsigned long timeElapsed = millis() - startDataTime;

    logData(fileName, timeElapsed, cvtTemp, portalTemp, gearboxTemp, pitch, roll, speed);
    //delay(systemTime);
  }
}
