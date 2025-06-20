#include "MS5611.h"

MS5611 MS5611(0x77);

void setup() {
  Serial.begin(115200);
  Wire.begin(2, 15);  // sda,scl   4,5 (Sirius)
  while(!Serial);

  if (!MS5611.begin()) {
    Serial.println("MS5611 not found, check wiring!");
    while (1);
  }
}

void loop() {
  MS5611.read();
  Serial.print("Temperature: ");
  Serial.print(MS5611.getTemperature(), 2);
  Serial.print("\tPressure: ");
  Serial.print(MS5611.getPressure(), 2);
  Serial.println();
  delay(1000);
}