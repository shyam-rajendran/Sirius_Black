#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_HMC5883_U.h>

Adafruit_HMC5883_Unified mag = Adafruit_HMC5883_Unified(12345);

//smoothing globals
float smoothedHeading = 0;
const float alpha = 0.1;

void displaySensorDetails(void)
{
  sensor_t sensor;
  mag.getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" uT");
  Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" uT");
  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" uT");  
  Serial.println("------------------------------------");
  Serial.println("");
  delay(500);
}

void setup(void) 
{
  Serial.begin(115200);
  Wire.begin(4,5);  // sda, scl

  Serial.println("HMC5883 Magnetometer Test"); Serial.println("");
  
  if(!mag.begin())
  {
    Serial.println("no HMC5883 detected");
    while(1);
  }
  
  displaySensorDetails();
  delay(5000);
}

void loop(void) 
{
  sensors_event_t event; 
  mag.getEvent(&event);

  Serial.print("X: "); Serial.print(event.magnetic.x); Serial.print("  ");
  Serial.print("Y: "); Serial.print(event.magnetic.y); Serial.print("  ");
  Serial.print("Z: "); Serial.print(event.magnetic.z); Serial.print("  "); Serial.println("uT");

  float heading = atan2(event.magnetic.y, -event.magnetic.x);
  float declinationAngle = 6.30 * PI / 180.0;  // Vellore=6.30 deg
  heading += declinationAngle;

  if(heading < 0)
    heading += 2*PI;
  if(heading > 2*PI)
    heading -= 2*PI;

  float headingDegrees = heading * 180/M_PI;

  // ----------- Smoothing -----------
  float delta = headingDegrees - smoothedHeading;
  if (delta > 180) delta -= 360;
  else if (delta < -180) delta += 360;

  smoothedHeading += alpha * delta;

  if (smoothedHeading < 0) smoothedHeading += 360;
  if (smoothedHeading >= 360) smoothedHeading -= 360;
  // ---------------------------------

  Serial.print("Heading (degrees): "); Serial.println(smoothedHeading);

  delay(100);
}
 