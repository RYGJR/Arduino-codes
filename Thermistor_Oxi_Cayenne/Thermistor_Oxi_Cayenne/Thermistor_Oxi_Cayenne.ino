#include <SoftwareSerial.h>
#include <ArduinoJson.h>
#define CAYENNE_PRINT Serial
#include <CayenneMQTTESP8266.h>
#include <ESP8266WiFi.h>
//D6 = Rx & D5 = Tx
SoftwareSerial nodemcu(D6, D5);

char ssid[] = "";
char wifiPassword[] = "";
char username[] = "";
char password[] = "";
char clientID[] = "";

const double VCC = 3.3;             // NodeMCU on board 3.3v vcc
const double R2 = 10000;            // 10k ohm series resistor
const double adc_resolution = 1023; // 10-bit adc

const double A = 0.001129148;   // thermistor equation parameters
const double B = 0.000234125;
const double C = 0.0000000876741; 


void setup() {
  // Initialize Serial port
  Serial.begin(115200);
  nodemcu.begin(115200);
  while (!Serial) continue;
  Cayenne.begin(username, password, clientID, ssid, wifiPassword);

}

void loop() {
 StaticJsonBuffer<100> jsonBuffer;
 JsonObject& data = jsonBuffer.parseObject(nodemcu);
 double Vout, Rth, temperature, adc_value; 
  

  
 adc_value = analogRead(A0);
 Vout = (adc_value * VCC) / adc_resolution;
 Rth = (VCC * R2 / Vout) - R2;
 float oxi = data["spo2"];
 
if(oxi != 0.0){ // the serial reading is invalid then send ping

Serial.println(oxi);

   temperature = (1 / (A + (B * log(Rth)) + (C * pow((log(Rth)),3))));   // Temperature in kelvin
   
   temperature = temperature - 273.15; // Temperature in degree celsius
  
   Serial.print("Temperature = ");
   Serial.print(temperature);


    Cayenne.virtualWrite(0, temperature, "Temp","C" );
    delay(2000);
   Serial.println(" degree celsius");
    Cayenne.virtualWrite(1, oxi, "Oximeter","SpO2" );
    delay(2000);
   

   
   
  
  
}else{
   int ping = 1;
   Cayenne.virtualWrite(2, ping, "ping","ping" );
   delay(2000);
 }

  
}
