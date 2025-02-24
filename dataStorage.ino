void initDataLogger(String fileName) {
  if (!SD.begin(BUILTIN_SDCARD)) {
    Serial.println("SD card initialization failed!");
    while (1);
  }
  // Convert String to const char*
  const char *fileNameChar = fileName.c_str();
  File dataFile = SD.open(fileNameChar, FILE_WRITE);
  if (dataFile) {
    dataFile.println("timeElapsed,cvtTemp,portalTemp,gearboxTemp,vehiclePitch,vehicleRoll,vehicleSpeed");
    dataFile.close();
  } else {
    Serial.println("Failed to create the file!");
    while (1);
  }
}

String generateFileName() {
  DateTime now = rtc.now();
  // Format the file name as yyyy_mm_dd_hh_mm_ss.csv
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  //rtc.adjust(DateTime(2025, 2, 24, 12, 01, 0));
  char buffer[25];
  snprintf(buffer, sizeof(buffer), "%02d/%02d/%04d_%02d:%02d.csv",
           now.month(), now.day(), now.year(), 
           now.hour(), now.minute());
  Serial.println(buffer);
  return String(buffer);
}

void logData(String fileName, float timeElapsed, float cvtTemp, float portalTemp, float gearboxTemp, float pitch, float roll, float vehicleSpeed) {
  // Convert String to const char*
  const char *fileNameChar = fileName.c_str();
  File dataFile = SD.open(fileNameChar, FILE_WRITE);
  if (dataFile) {
    dataFile.print(timeElapsed);
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
  } else {
    Serial.println("Error writing to the file!");
  }
}