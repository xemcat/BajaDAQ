void setupRTC() {
  Wire1.begin();
  if (!rtc.begin(&Wire1)) {
    Serial.println("Failed to detect DS3231 RTC! Check connections.");
  }
  Serial.println("DS3231 RTC initialized successfully!");
  // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
}

String generateFileName() {
  DateTime now = rtc.now();
  char buffer[20];
  snprintf(buffer, sizeof(buffer), "%02d.%02d_%02d.%02d.csv",
           now.month(), now.day(), now.hour(), now.minute());
  return String(buffer);
}

String getFormattedTime() {
  DateTime now = rtc.now();
  char buffer[12];
  snprintf(buffer, sizeof(buffer), "%02d:%02d:%02d",
           now.hour(), now.minute(), now.second());
  return String(buffer);
}

void initDataLogger(String fileName) {
  Serial.println("Initializing SD card...");
  if (!SD.begin(BUILTIN_SDCARD)) {
    Serial.println("SD card initialization failed! Check card or connections.");
    while (1);
  }
  Serial.println("SD card initialized successfully!");

  const char *fileNameChar = fileName.c_str();
  File dataFile = SD.open(fileNameChar, FILE_WRITE);
  if (dataFile) {
    dataFile.println("timeLogged,cvtTemp,portalTemp,gearboxTemp,vehiclePitch,vehicleRoll,vehicleSpeed");
    dataFile.close();
    Serial.println("File " + fileName + " created.");
  } else {
    Serial.println("Failed to create file " + fileName + "! Check SD card.");
    while (1);
  }
}

void logData(String fileName, float cvtTemp, float portalTemp, float gearboxTemp, float pitch, float roll, float vehicleSpeed) {
  const char *fileNameChar = fileName.c_str();
  File dataFile = SD.open(fileNameChar, FILE_WRITE);
  if (dataFile) {
    String timeLogged = getFormattedTime(); // Get current time
    dataFile.print(timeLogged);
    dataFile.print(",");
    dataFile.print(cvtTemp);
    dataFile.print(",");
    dataFile.print(portalTemp);
    dataFile.print(",");
    dataFile.print(gearboxTemp);
    dataFile.print(",");
    dataFile.print(pitch);
    dataFile.print(",");
    dataFile.print(roll);
    dataFile.print(",");
    dataFile.println(vehicleSpeed);
    dataFile.close();
    Serial.println("Data written to " + fileName + " successfully.");
  } else {
    Serial.println("Error: Could not open file " + fileName + " for writing!");
  }
}