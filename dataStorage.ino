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
  char buffer[25];
  snprintf(buffer, sizeof(buffer), "%04d_%02d_%02d_%02d_%02d.csv",
           now.year(), now.month(), now.day(),
           now.hour(), now.minute());
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
