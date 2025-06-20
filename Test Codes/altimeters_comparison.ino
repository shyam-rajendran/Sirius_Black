#include <Wire.h>
#include <Adafruit_BMP280.h>
#include <MS5611.h>
#include <TinyGPSPlus.h>

// I2C Pins
#define SDA_PIN 4
#define SCL_PIN 5

// Sensor Addresses
#define BMP280_ADDR 0x76
#define MS5611_ADDR 0x77

// GPS UART Pins
#define GPS_RX 18  // RX (connect to GPS TX)
#define GPS_TX 17  // TX (connect to GPS RX)

// Sensor Instances
Adafruit_BMP280 bmp280;
MS5611 ms5611(MS5611_ADDR);
TinyGPSPlus gps;
HardwareSerial GPS_Serial(1);  // UART1

void setup() {
  Serial.begin(115200);
  delay(1000);

  // Setup I2C with custom pins
  Wire.begin(SDA_PIN, SCL_PIN);

  // Initialize BMP280
  if (!bmp280.begin(BMP280_ADDR)) {
    Serial.println("❌ BMP280 not found at 0x76.");
  } else {
    Serial.println("✅ BMP280 initialized.");
    bmp280.setSampling(Adafruit_BMP280::MODE_NORMAL,
                       Adafruit_BMP280::SAMPLING_X2,
                       Adafruit_BMP280::SAMPLING_X16,
                       Adafruit_BMP280::FILTER_X16,
                       Adafruit_BMP280::STANDBY_MS_500);
  }

  // Initialize MS5611
  if (!ms5611.begin()) {
    Serial.println("❌ MS5611 not found at 0x77.");
  } else {
    Serial.println("✅ MS5611 initialized.");
    ms5611.setOversampling(OSR_ULTRA_HIGH);
  }

  // Initialize GPS
  GPS_Serial.begin(9600, SERIAL_8N1, GPS_RX, GPS_TX);
  Serial.println("✅ GPS serial initialized on UART1.");
}

void loop() {
  // Read from GPS
  while (GPS_Serial.available() > 0) {
    gps.encode(GPS_Serial.read());
  }

  // BMP280 readings
  float bmp_temp = bmp280.readTemperature();
  float bmp_press = bmp280.readPressure() / 100.0;
  float bmp_alt = bmp280.readAltitude(1013.25);

  // MS5611 readings
  ms5611.read();
  float ms_temp = ms5611.getTemperature();
  float ms_press = ms5611.getPressure();
  float ms_alt = 44330.0 * (1.0 - pow(ms_press / 1013.25, 0.1903));

  // Display readings
  Serial.println("\n📊 Sensor Comparison:");
  Serial.printf("BMP280  → Temp: %.2f°C | Pressure: %.2f hPa | Altitude: %.2f m\n", bmp_temp, bmp_press, bmp_alt);
  Serial.printf("MS5611  → Temp: %.2f°C | Pressure: %.2f hPa | Altitude: %.2f m\n", ms_temp, ms_press, ms_alt);

  // GPS Data
  if (gps.location.isValid()) {
    Serial.printf("GPS     → Lat: %.6f | Lon: %.6f\n", gps.location.lat(), gps.location.lng());
    if (gps.altitude.isValid()) {
      Serial.printf("         Altitude: %.2f m\n", gps.altitude.meters());
    }
    if (gps.time.isValid()) {
      Serial.printf("         Time (UTC): %02d:%02d:%02d\n",
                    gps.time.hour(), gps.time.minute(), gps.time.second());
    }
  } else {
    Serial.println("GPS     → Waiting for fix...");
  }

  delay(1000);
}
