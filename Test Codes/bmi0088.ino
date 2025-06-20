#include <Wire.h>
#include "BMI088.h"

// Define I2C pins for ESP32
#define SDA_PIN 4
#define SCL_PIN 5

// Create accelerometer and gyroscope objects with specified I2C addresses
Bmi088Accel accel(Wire, 0x19); // Accelerometer address
Bmi088Gyro gyro(Wire, 0x69);   // Gyroscope address

void setup() {
  Serial.begin(115200);
  delay(100);

  // Initialize I2C with custom pins
  Wire.begin(SDA_PIN, SCL_PIN);

  // Initialize accelerometer
  if (accel.begin() < 0) {
    Serial.println("Failed to initialize accelerometer. Check connections.");
    while (1);
  }

  // Initialize gyroscope
  if (gyro.begin() < 0) {
    Serial.println("Failed to initialize gyroscope. Check connections.");
    while (1);
  }

  Serial.println("BMI088 sensor initialized successfully.");
}

void loop() {
  // Read sensor data
  accel.readSensor();
  gyro.readSensor();

  // Get accelerometer data (m/s^2)
  float ax = accel.getAccelX_mss();
  float ay = accel.getAccelY_mss();
  float az = accel.getAccelZ_mss();

  // Get gyroscope data (rad/s)
  float gx = gyro.getGyroX_rads();
  float gy = gyro.getGyroY_rads();
  float gz = gyro.getGyroZ_rads();

  // Output labeled data for Serial Plotter
  Serial.print("Accel_X:"); Serial.print(ax); Serial.print("\t");
  Serial.print("Accel_Y:"); Serial.print(ay); Serial.print("\t");
  Serial.print("Accel_Z:"); Serial.print(az); Serial.print("\t");
  Serial.print("Gyro_X:");  Serial.print(gx); Serial.print("\t");
  Serial.print("Gyro_Y:");  Serial.print(gy); Serial.print("\t");
  Serial.print("Gyro_Z:");  Serial.println(gz);

  delay(100); // Delay for readability
}
