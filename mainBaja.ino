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
String timeLogged;

// Define Data Switch
const int dataSwitchPin = 2;      // Use pin 2 for the switch
bool collectingData = false;      // Flag for data collection
unsigned long loopRunningTime = 0;
unsigned long logTime = 15;

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
void processSensorInput(float vehicleSpeed);
void moveEightSteps(bool forward);
float getVehicleSpeed();
float getCVTTemp();
float getPortalTemp();
float getGearboxTemp();
float getVehiclePitch();
float getVehicleRoll();
String getFormattedTime();

void setup() {
  Serial.begin(9600);
  //while (!Serial);
  Serial.println("Starting Project...");
  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while(1);
  }
  Serial.println("Initializing");
  mlxInit();          // Initialize MLX90614
  mpuInit();          // Initialize MPU6050
  rifeInit();         // Initialize RIFE Portal
  hallInit();         // Initialize Hall Effect
  Serial.println("MLX MPU RIFE HALL done");
  stepperInit();      // Initialize Stepper and Limit Switch
  Serial.println("STEP LIMIT done");

  fileName = generateFileName();
  initDataLogger(fileName);
  pinMode(dataSwitchPin, INPUT_PULLUP);     // Configure switch pin with pull-up
  Serial.println("Flip the switch to start or stop data collection.");
}

void loop() {
  unsigned long loopStartTime = millis();
  int switchState = digitalRead(dataSwitchPin); // Read the switch state
  if (switchState == HIGH) {
    if (!collectingData) {
      collectingData = true;
      Serial.println("Data collection started.");
    }
  } else {
    if (collectingData) {
      collectingData = false;
      Serial.println("Data collection stopped.");
    }
  }

  calculateMLXData();
  calculateMPUData();
  displayMPUData();
  calculateRifeData();
  calculateHallData();
  displayHallData();
  float vehicleSpeed = getVehicleSpeed();
  processSensorInput(vehicleSpeed);
  getFormattedTime();
  Serial.println(loopStartTime);

  loopRunningTime = loopRunningTime + (millis() - loopStartTime);
  Serial.println(loopRunningTime);
  Serial.println(getFormattedTime());
  delay(1000);

  if (loopRunningTime >= logTime) {
    float cvtTemp = getCVTTemp();
    float portalTemp = getPortalTemp();
    float gearboxTemp = getGearboxTemp();
    float pitch = getVehiclePitch();
    float roll = getVehicleRoll();
    float speed = getVehicleSpeed();
    timeLogged = getFormattedTime();

    logData(fileName, timeLogged, cvtTemp, portalTemp, gearboxTemp, pitch, roll, speed);
    loopRunningTime = 0;
    Serial.println("Data Logged ***************************************************************");
    delay(2000);
  }
}