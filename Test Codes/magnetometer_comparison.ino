#include <Wire.h>
#include <Adafruit_LIS3MDL.h>
#include <Adafruit_HMC5883_U.h>
#include <math.h>


#define SDA_PIN 4
#define SCL_PIN 5


#define LIS3MDL_ADDR 0x1C

Adafruit_LIS3MDL lis3mdl;
Adafruit_HMC5883_Unified hmc5883 = Adafruit_HMC5883_Unified(12345);

// Smoothing globals for HMC5883L
float smoothedHeadingHMC = 0;
const float alpha = 0.1;

void setup() {
  Serial.begin(115200);
  delay(1000);

  Wire.begin(SDA_PIN, SCL_PIN);

  // Initialize LIS3MDL
  if (!lis3mdl.begin_I2C(LIS3MDL_ADDR, &Wire)) {
    Serial.println("❌ LIS3MDL not found.");
  } else {
    Serial.println("✅ LIS3MDL initialized.");
    lis3mdl.setPerformanceMode(LIS3MDL_MEDIUMMODE);
    lis3mdl.setOperationMode(LIS3MDL_CONTINUOUSMODE);
    lis3mdl.setDataRate(LIS3MDL_DATARATE_155_HZ);
    lis3mdl.setRange(LIS3MDL_RANGE_4_GAUSS);
  }

  // Initialize HMC5883L
  if (!hmc5883.begin()) {
    Serial.println("❌ HMC5883L not found.");
  } else {
    Serial.println("✅ HMC5883L initialized.");
  }
}

float calculateHeading(float x, float y) {
  float heading = atan2(y, x) * 180 / PI;
  if (heading < 0) heading += 360;
  return heading;
}

float smoothHeading(float current, float previous) {
  float delta = current - previous;
  if (delta > 180) delta -= 360;
  else if (delta < -180) delta += 360;

  float result = previous + alpha * delta;

  if (result < 0) result += 360;
  if (result >= 360) result -= 360;

  return result;
}

void loop() {
  sensors_event_t lis_event, hmc_event;
  lis3mdl.getEvent(&lis_event);
  hmc5883.getEvent(&hmc_event);

  float heading_lis = calculateHeading(lis_event.magnetic.x, lis_event.magnetic.y);
  float heading_hmc = calculateHeading(hmc_event.magnetic.y, -hmc_event.magnetic.x); 

  smoothedHeadingHMC = smoothHeading(heading_hmc, smoothedHeadingHMC);

  Serial.println("\n🧭 Magnetometer Heading Comparison:");
  Serial.printf("LIS3MDL   → Heading: %6.2f° | X: %6.2f | Y: %6.2f | Z: %6.2f\n",heading_lis, lis_event.magnetic.x, lis_event.magnetic.y, lis_event.magnetic.z);
  Serial.printf("HMC5883L  → Raw: %6.2f° | Smoothed: %6.2f° | X: %6.2f | Y: %6.2f | Z: %6.2f\n", heading_hmc, smoothedHeadingHMC, hmc_event.magnetic.x, hmc_event.magnetic.y, hmc_event.magnetic.z);

  delay(100);
}
