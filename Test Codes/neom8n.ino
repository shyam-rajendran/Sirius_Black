#include <TinyGPS++.h>
#include <HardwareSerial.h>

// GPS object
TinyGPSPlus gps;

// Create a hardware serial port on pins 17 (TX) and 18 (RX)
HardwareSerial gpsSerial(1);

// Pins
#define RXPin 18  //sirius rx 18
#define TXPin 17  //sirius tx 17

void setup() {
  Serial.begin(115200);
  gpsSerial.begin(9600, SERIAL_8N1, RXPin, TXPin);  // GPS module baud rate and pins

  Serial.println("Initializing GPS...");
}

unsigned long lastDisplay = 0;

void loop() {
  // Consume all incoming GPS characters quickly
  while (gpsSerial.available()) {
    gps.encode(gpsSerial.read());
  }

  // Print data once per second
  if (millis() - lastDisplay > 1000) {
    lastDisplay = millis();

    if (gps.location.isValid()) {
      Serial.println("------- GPS Data -------");
      Serial.print("LAT: "); Serial.println(gps.location.lat(), 6);
      Serial.print("LONG: "); Serial.println(gps.location.lng(), 6);
      Serial.print("SPEED (km/h): "); Serial.println(gps.speed.kmph());
      Serial.print("ALT (m): "); Serial.println(gps.altitude.meters());
      Serial.print("HDOP: "); Serial.println(gps.hdop.hdop());
      Serial.print("Satellites: "); Serial.println(gps.satellites.value());
      Serial.print("Time in UTC: ");
      Serial.print(gps.date.year()); Serial.print("/");
      Serial.print(gps.date.month()); Serial.print("/");
      Serial.print(gps.date.day()); Serial.print(", ");
      Serial.print(gps.time.hour()); Serial.print(":");
      Serial.print(gps.time.minute()); Serial.print(":");
      Serial.println(gps.time.second());
      Serial.println("------------------------");
    } else {
      Serial.println("Waiting for GPS fix...");
    }
  }
}
