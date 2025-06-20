#include <SPI.h>
#include <SD.h>



// sirius test
#define PIN_SPI_CS   15   // Chip Select pin
#define PIN_SPI_MOSI 12  // MOSI
#define PIN_SPI_MISO 13  // MISO
#define PIN_SPI_SCLK 14  // SCLK

int packetCount = 0;  // Declare globally so loop() can access it

void setup() {
    Serial.begin(115200);
    while (!Serial); // Wait for Serial Monitor (for debugging)

    Serial.println("Initializing SD card...");

    // Initialize SPI with custom pins
    SPI.begin(PIN_SPI_SCLK, PIN_SPI_MISO, PIN_SPI_MOSI, PIN_SPI_CS);

    // Try to initialize SD card
    if (!SD.begin(PIN_SPI_CS)) {
        Serial.println("SD card initialization failed!");
        Serial.println("Possible causes:");
        Serial.println("- Incorrect wiring (check CS, MOSI, MISO, SCLK)");
        Serial.println("- SD card not inserted properly");
        Serial.println("- SD card not formatted as FAT16/FAT32");
        return;
    }
    Serial.println("SD card initialized successfully!");

    // Check if file exists or create it
    if (SD.exists("/data.csv")) {
        Serial.println("data.csv already exists.");
    } else {
        Serial.println("Creating data.csv...");
        File file = SD.open("/data.csv", FILE_WRITE);
        if (file) {
            file.println("TEAM_ID,TIME,PACKET_COUNT,MODE,STATE,ALTITUDE,TEMPERATURE,PRESSURE,VOLTAGE," // Header row
                         "GYRO_R,GYRO_P,GYRO_Y,ACCEL_R,ACCEL_P,ACCEL_Y,MAG_R,MAG_P,MAG_Y,AUTO_GYRO_ROT_RATE,"
                         "GPS_TIME,GPS_ALTITUDE,GPS_LATITUDE,GPS_LONGITUDE,GPS_SATS,CMD_ECHO,RPM_ONE,RPM_TWO");
            file.close();
            Serial.println("Headers written to data.csv");
        } else {
            Serial.println("Error creating data.csv");
        }
    }
}

void loop() {
    // Simulating random sensor values
    int mode = random(0, 5);
    String currentState = "ACTIVE";
    float altitude = random(100, 500) / 10.0;
    float temperature = random(200, 350) / 10.0;
    float pressure = random(900, 1100);
    float voltage = random(30, 50) / 10.0;

    float gyro_r = random(-100, 100) / 10.0;
    float gyro_p = random(-100, 100) / 10.0;
    float gyro_y = random(-100, 100) / 10.0;

    float accel_r = random(-50, 50) / 10.0;
    float accel_p = random(-50, 50) / 10.0;
    float accel_y = random(-50, 50) / 10.0;

    float mag_r = random(-200, 200) / 10.0;
    float mag_p = random(-200, 200) / 10.0;
    float mag_y = random(-200, 200) / 10.0;

    float auto_gyro_rot_rate = random(10, 100) / 10.0;

    float gps_time = millis() / 1000.0;
    float gps_altitude = random(100, 500);
    float gps_latitude = random(-9000000, 9000000) / 100000.0;
    float gps_longitude = random(-18000000, 18000000) / 100000.0;
    int gps_sats = random(4, 12);
    int cmd_echo = random(0, 255);

    int rpm_one = random(1000, 5000);
    int rpm_two = random(1000, 5000);

    // Constructing CSV row
    String data = "TEAM_ID, " + String(millis()) + ", " + String(packetCount++) + ", " + String(mode) + ", " + currentState + ", " +
                  String(altitude) + ", " + String(temperature) + ", " + String(pressure) + ", " + String(voltage) + ", " +
                  String(gyro_r) + ", " + String(gyro_p) + ", " + String(gyro_y) + ", " +
                  String(accel_r) + ", " + String(accel_p) + ", " + String(accel_y) + ", " +
                  String(mag_r) + ", " + String(mag_p) + ", " + String(mag_y) + ", " + String(auto_gyro_rot_rate) + ", " +
                  String(gps_time) + ", " + String(gps_altitude) + ", " + String(gps_latitude) + ", " +
                  String(gps_longitude) + ", " + String(gps_sats) + ", " + String(cmd_echo) + ", " +
                  String(rpm_one) + ", " + String(rpm_two);

    // Open CSV file and write data (append new line)
    File file = SD.open("/data.csv", FILE_APPEND);
    if (file) {
        file.println(data);  // Make sure each entry is on a new line
        file.close();
        Serial.println("Data written to SD card: " + data);
    } else {
        Serial.println("Error opening CSV file for writing.");
    }

    delay(2000); // Log data every 2 seconds
}
