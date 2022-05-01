#include <ESP8266WiFi.h>
#define CAYENNE_PRINT Serial
#include <CayenneMQTTESP8266.h>

// Set these to run example.
char ssid[] = "";
char wifiPassword[] = "";
char username[] = "";
char password[] = "";
char clientID[] = "";

const int FLEX_PIN = A0; // Pin connected to voltage divider output

// Measure the voltage at 5V and the actual resistance of your
// 47k resistor, and enter them below:
const float VCC = 3.3; // Measured voltage of Ardunio 5V line
const float R_DIV = 4700.0; // Measured resistance of 3.3k resistor

// Upload the code, then try to adjust these values to more
// accurately calculate bend degree.
const float STRAIGHT_RESISTANCE = 30000.0; // resistance when straight
const float BEND_RESISTANCE = 90000.0; // resistance at 90 deg

void setup() 
{
  Serial.begin(115200);
  pinMode(FLEX_PIN, INPUT);
  Cayenne.begin(username, password, clientID, ssid, wifiPassword);
}



void loop() 
{
 // Read the ADC, and calculate voltage and resistance from it
 int flexADC = analogRead(FLEX_PIN);
 float flexV = flexADC * VCC / 1023.0;
 float flexR = R_DIV * (VCC / flexV - 1.0);
 Serial.println("Resistance: " + String(flexR) + " ohms");
 
 // Use the calculated resistance to estimate the sensor's
 // bend angle:
 float angle = map(flexR, STRAIGHT_RESISTANCE, BEND_RESISTANCE,
                  0, 90.0);
  delay(2000);

 Serial.println("Bend: " + String(angle) + " degrees");

  
  Cayenne.virtualWrite(0, angle, "Bend","degrees" );

}
