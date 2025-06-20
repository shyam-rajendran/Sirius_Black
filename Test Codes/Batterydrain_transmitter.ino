#include <SPI.h>
#include <LoRa.h>
#include <Wire.h>
#include <Adafruit_BMP280.h>
#include <MS5611.h>
#include <TinyGPSPlus.h>

// LoRa pins
#define LORA_SCK     14
#define LORA_MISO    13
#define LORA_MOSI    12
#define LORA_CS      10
#define LORA_RST     9
#define LORA_IRQ     8

#define LORA_FREQ    915E6

// I2C Pins
#define SDA_PIN 4
#define SCL_PIN 5

// Sensor Addresses
#define BMP280_ADDR 0x76
#define MS5611_ADDR 0x77

// GPS UART Pins
#define GPS_RX 18
#define GPS_TX 17

Adafruit_BMP280 bmp280;
MS5611 ms5611(MS5611_ADDR);
TinyGPSPlus gps;
HardwareSerial GPS_Serial(1);

void setup() {
  Serial.begin(115200);
  GPS_Serial.begin(9600, SERIAL_8N1, GPS_RX, GPS_TX);

  Wire.begin(SDA_PIN, SCL_PIN);
  bmp280.begin(BMP280_ADDR);
  ms5611.begin();
  ms5611.setOversampling(OSR_ULTRA_HIGH);

  SPI.begin(LORA_SCK, LORA_MISO, LORA_MOSI, LORA_CS);
  pinMode(LORA_RST, OUTPUT);
  digitalWrite(LORA_RST, LOW);
  delay(20);
  digitalWrite(LORA_RST, HIGH);
  delay(20);

  LoRa.setPins(LORA_CS, LORA_RST, LORA_IRQ);
  if (!LoRa.begin(LORA_FREQ)) {
    Serial.println("LoRa init failed");
    while (1);
  }

  LoRa.setTxPower(20, PA_OUTPUT_PA_BOOST_PIN);  // Max power
  Serial.println("✅ LoRa transmitter ready");
}

void loop() {
  // GPS update
  while (GPS_Serial.available()) {
    gps.encode(GPS_Serial.read());
  }

  // Read sensors
  float bmp_temp = bmp280.readTemperature();
  float bmp_press = bmp280.readPressure() / 100.0;
  float bmp_alt = bmp280.readAltitude(1013.25);

  ms5611.read();
  float ms_temp = ms5611.getTemperature();
  float ms_press = ms5611.getPressure();
  float ms_alt = 44330.0 * (1.0 - pow(ms_press / 1013.25, 0.1903));

  // Time for CSV
  char timestamp[16] = "NA";
  if (gps.time.isValid()) {
    snprintf(timestamp, sizeof(timestamp), "%02d:%02d:%02d",
             gps.time.hour(), gps.time.minute(), gps.time.second());
  }

  // Build CSV String
  String csv = String(timestamp) + "," +
               String(bmp_temp, 2) + "," + String(bmp_press, 2) + "," + String(bmp_alt, 2) + "," +
               String(ms_temp, 2) + "," + String(ms_press, 2) + "," + String(ms_alt, 2) + ",";

  if (gps.location.isValid()) {
    csv += String(gps.location.lat(), 6) + "," + String(gps.location.lng(), 6) + ",";
  } else {
    csv += "NA,NA,";
  }

  if (gps.altitude.isValid()) {
    csv += String(gps.altitude.meters(), 2);
  } else {
    csv += "NA";
  }

  // Transmit over LoRa
  LoRa.beginPacket();
  LoRa.print(csv);
  LoRa.endPacket();

  Serial.println("📡 Sent CSV:");
  Serial.println(csv);

  delay(1000);  // Or lower for higher data rate
}
