#include <Adafruit_Sensor.h>
#include <Adafruit_MLX90614.h>
#include <Adafruit_MPU6050.h>
#include <Wire.h>
#include <math.h>
#include <SD.h>
#include <TimeLib.h>
#include <RTClib.h>

// Declare global sensor objects
Adafruit_MLX90614 mlx = Adafruit_MLX90614();
Adafruit_MPU6050 mpu = Adafruit_MPU6050();
RTC_DS3231 rtc;
String fileName;

// Define Data Switch
const int dataSwitchPin = 2;
bool collectingData = false;
unsigned long loopRunningTime = 0;
unsigned long logTime = 15; // 1 second for stability
unsigned long timeElapsed = 0;

// Forward declarations for sensor functions
void mlxInit();
void calculateMLXData();
void mpuInit();
void calculateMPUData();
void displayMPUData();
void rifeInit();
void calculateRifeData();
void hallInit();
void calculateHallData();
void displayHallData();
void stepperInit();
void processSensorInput(float vehicleSpeed);
void moveEightSteps(bool forward);
float getVehicleSpeed();
float getCVTTemp();
float getPortalTemp();
float getGearboxTemp();
float getVehiclePitch();
float getVehicleRoll();

// Forward declarations for data storage functions (defined in dataStorage.ino)
void setupRTC();
void initDataLogger(String fileName);
void logData(String fileName, float cvtTemp, float portalTemp, float gearboxTemp, float pitch, float roll, float vehicleSpeed);
String generateFileName();

void setup() {
  Serial.begin(9600);
  Serial.println("Starting Project...");

  // Initialize RTC and SD from dataStorage.ino
  setupRTC();
  fileName = generateFileName();
  Serial.println("File name: " + fileName);
  initDataLogger(fileName);

  // Initialize sensors
  Serial.println("Initializing sensors...");
  mlxInit();
  mpuInit();
  rifeInit();
  hallInit();
  Serial.println("MLX MPU RIFE HALL done");
  stepperInit();
  Serial.println("STEP LIMIT done");

  pinMode(dataSwitchPin, INPUT_PULLUP);
  Serial.println("Flip the switch to start or stop data collection.");
}

void loop() {
  unsigned long loopStartTime = millis();
  int switchState = digitalRead(dataSwitchPin);
  if (switchState == HIGH) {
    if (!collectingData) {
      collectingData = true;
      Serial.println("Data collection started with file: " + fileName);
      // Uncomment to set RTC time when collection starts
      // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
      timeElapsed = millis();
    }
  } else {
    if (collectingData) {
      collectingData = false;
      Serial.println("Data collection stopped.");
    }
  }

  // Sensor calculations (placeholders)
  calculateMLXData();
  calculateMPUData();
  displayMPUData();
  calculateRifeData();
  calculateHallData();
  displayHallData();
  float vehicleSpeed = getVehicleSpeed();
  processSensorInput(vehicleSpeed);

  loopRunningTime += (millis() - loopStartTime);
  Serial.println(loopRunningTime);
  delay(1000);

  if (loopRunningTime >= logTime && collectingData) {
    Serial.println("Logging to file: " + fileName);
    float cvtTemp = getCVTTemp();
    float portalTemp = getPortalTemp();
    float gearboxTemp = getGearboxTemp();
    float pitch = getVehiclePitch();
    float roll = getVehicleRoll();
    float speed = getVehicleSpeed();
    logData(fileName, cvtTemp, portalTemp, gearboxTemp, pitch, roll, speed);
    Serial.println("Data Logged **********************************************************************************");
    loopRunningTime = 0;
    delay(5000);
  }
}