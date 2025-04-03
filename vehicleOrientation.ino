/*
 * 2024-2025 Union Baja SAE: Data Acquisition and Display System for Increased Vehicle Performance
 * File: vehicleOrientation.ino
 * Author: xemcat
 * 
 * This code is released under the "Baja Open Garage License" (BOGL):
 * - Use it for your Baja builds, learning, or tinkering.
 * - Share it with credit to xemcat and the GitHub repo.
 * - Modify it freely—let me know if you make it cooler!
 * - No guarantees; use at your own risk.
 * - Keep the Baja spirit: build, break, repeat.
 */

extern Adafruit_MPU6050 mpu;

float pitch = 0.0;
float roll = 0.0;

void mpuInit() {
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip. Check wiring!");
  //   while (1);
  }
  mpu.setAccelerometerRange(MPU6050_RANGE_2_G);
  mpu.setGyroRange(MPU6050_RANGE_250_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
}

void calculateMPUData() {
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  float ax_g = a.acceleration.x;
  float ay_g = a.acceleration.y;
  float az_g = a.acceleration.z;

  pitch = atan2(-ax_g, sqrt(ay_g * ay_g + az_g * az_g)) * 180.0 / PI;
  roll = atan2(ay_g, az_g) * 180.0 / PI;
}

void displayMPUData() {
  Serial.println("Orientation:");
  Serial.print("   Pitch: "); Serial.print(pitch); Serial.println("°");
  Serial.print("   Roll: "); Serial.print(roll); Serial.println("°");
}

float getVehiclePitch() {
  return pitch;
}

float getVehicleRoll() {
  return roll;
}
