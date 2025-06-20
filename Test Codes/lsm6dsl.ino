#include <Wire.h>
#include <Adafruit_LSM6DSL.h>


Adafruit_LSM6DSL lsm6dsl;


#define SDA_PIN 4
#define SCL_PIN 5

void setup() {
  Serial.begin(115200);
  while (!Serial) delay(10);
  
  Serial.println("LSM6DSL ESP32-S3 with Adafruit Library");
  
 
  Wire.begin(SDA_PIN, SCL_PIN);
  Wire.setClock(400000); // 400kHz I2C speed
  
  delay(100);
  
 
  if (!lsm6dsl.begin_I2C()) {
    Serial.println("Failed to find LSM6DSL chip");
    Serial.println("Check wiring and I2C address");
    while (1) delay(1000);
  }
  
  Serial.println("LSM6DSL Found!");
  
  lsm6dsl.setAccelRange(LSM6DS_ACCEL_RANGE_2_G);     // ±2g
  lsm6dsl.setGyroRange(LSM6DS_GYRO_RANGE_250_DPS);   // ±250 degrees/s
  lsm6dsl.setAccelDataRate(LSM6DS_RATE_104_HZ);      // 104 Hz
  lsm6dsl.setGyroDataRate(LSM6DS_RATE_104_HZ);       // 104 Hz
  
  Serial.println("Sensor configured:");
  Serial.println("- Accelerometer: ±2g, 104Hz");
  Serial.println("- Gyroscope: ±250dps, 104Hz");
  Serial.println();
  Serial.println("Ready! Data format for CSV: Time,Accel_X,Accel_Y,Accel_Z,Gyro_X,Gyro_Y,Gyro_Z,Temp");
  Serial.println("Serial Plotter will show: Accel_X, Accel_Y, Accel_Z, Gyro_X, Gyro_Y, Gyro_Z");
}

void loop() {
  
  sensors_event_t accel;
  sensors_event_t gyro;
  sensors_event_t temp;
  
  lsm6dsl.getEvent(&accel, &gyro, &temp);
  
  // for serial plotter cause it looks cool
  Serial.print(accel.acceleration.x, 3);    // Accel_X
  Serial.print(",");
  Serial.print(accel.acceleration.y, 3);    // Accel_Y  
  Serial.print(",");
  Serial.print(accel.acceleration.z, 3);    // Accel_Z
  Serial.print(",");
  Serial.print(gyro.gyro.x * 57.2958, 2);  // Gyro_X (converted to deg/s)
  Serial.print(",");
  Serial.print(gyro.gyro.y * 57.2958, 2);  // Gyro_Y
  Serial.print(",");
  Serial.println(gyro.gyro.z * 57.2958, 2); // Gyro_Z
  
  delay(100); // 10Hz 
}