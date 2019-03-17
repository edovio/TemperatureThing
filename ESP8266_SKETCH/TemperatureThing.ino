/**********************************************************************************************************************
 * Sketch created by Edoardo Viola - GitHub Account: @edovio - Email: edoardo@edovio.in - http://edovio.in            *
 *            This code is under Creative Commons - Attribution 4.0 International (CC BY 4.0)                         *
 *              For any information about the project or to contribute follow it on GitHub                            *                 
 * *******************************************************************************************************************/


//Library for the DHT11 sensor
#include <Adafruit_Sensor.h>
#include "DHT.h"

//Library for Mozilla IOT Gateway
#include <Arduino.h>
#include "Thing.h"
#include "WebThingAdapter.h"

//Pin Conversion for ESP8266
#define D0 16
#define D1 5
#define D2 4
#define D3 0
#define D4 2
#define D5 14
#define D6 12
#define D7 13
#define D8 15
#define D9 3
#define D10 1

//Define about the type and PIN Connection for the Temperature Sensor
#define DHTPIN D1
#define DHTTYPE DHT11

//Temperature Sensor Inizialization
DHT dht(DHTPIN, DHTTYPE);

//Led Status PIN
#define ledStat D2

//Your wifi credentials here (and keep it private)
const char* ssid = "SSID_NAME";
const char* password = "PASSWORD";



WebThingAdapter adapter("w25");

const char* tempTypes[] = {"Temperatura", "Umidita"};
ThingDevice temp("Temp", "Temperatura Salotto", "Umidita Salotto");
ThingProperty tempValue("TEMP", "", NUMBER);
ThingProperty humValue("HUM", "", NUMBER);

bool lastOn = false;

void setup(void){
  pinMode(DHTPIN, INPUT);
  pinMode(ledStat,OUTPUT);
  Serial.begin(115200);
  Serial.println("");
  Serial.print("Connecting to \"");
  Serial.print(ssid);
  Serial.println("\"");
#if defined(ESP8266) || defined(ESP32)
  WiFi.mode(WIFI_STA);
#endif
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  bool blink = true;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    digitalWrite(ledStat, blink ? LOW : HIGH); // active low led
    blink = !blink;
  }
  digitalWrite(ledStat, HIGH); // active low led

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  temp.addProperty(&tempValue);
  temp.addProperty(&humValue);
  adapter.addDevice(&temp);
  adapter.begin();
  Serial.println("HTTP server started");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.print("/things/");
  Serial.println(temp.id);
}

void loop(void){
  adapter.update();
  
  double temperatura = dht.readTemperature();
  double umidita = dht.readHumidity();
  ThingPropertyValue tempCurrent;
  tempCurrent.number = temperatura;
  ThingPropertyValue humCurrent;
  humCurrent.number = umidita;

  tempValue.setValue(tempCurrent);
  humValue.setValue(humCurrent);
}
