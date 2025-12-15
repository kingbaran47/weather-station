#include <Arduino.h>
#include <DHT.h>
#include <DHT_U.h>

#include <WiFi.h>
#include <WebServer.h>



const char* WIFI_SSID = "YOUR_WIFI_NAME";
const char *WIFI_PASSWORD = "YOUR_WIFI_PASSWORD";


//webserver details
WebServer server(80);
String header;
unsigned long currentTime = millis();
const long timeoutTime = 2000;
unsigned long previousTime;

//sensor details
#define DHTPIN 18
#define DHTTYPE DHT22

DHT_Unified dht(DHTPIN, DHTTYPE);

uint32_t delayMS;



void handleRoot() {
  sensors_event_t event;
  dht.temperature().getEvent(&event);
  float temp = event.temperature; //reads value from event
  dht.humidity().getEvent(&event);
  float hum = event.relative_humidity; //reads value from event

  String html = "<!DOCTYPE html><html><body>";
  html += "<h1>ESP32 Weatherstation</h1>";
  html += "<p>Temperature: " + String(temp) + " °C</p>";
  html += "<p>Humidity: " + String(hum) + " %</p>";
  html += "</body></html>";

  server.send(200, "text/html", html);
}

//Prepare response


void setup() {
  Serial.begin(115200);
  //Connect to WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while(WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(400);
  }

  Serial.println();
  Serial.println("WiFi connected successfully.");
  Serial.println("IP adress: ");
  Serial.println(WiFi.localIP());
  server.on("/", handleRoot);
  server.begin();
  dht.begin();  
  sensor_t sensor;


}

void loop() {
  server.handleClient();

  static unsigned long lastRead = 0;
  if(millis() - lastRead > 2000) {
lastRead = millis();
  sensors_event_t event;
  
  dht.temperature().getEvent(&event);
  if(isnan(event.temperature)) {
    Serial.println("Error while reading temperature.");
  } else {
    Serial.print("Temperature: ");
    Serial.print(event.temperature);
    Serial.print(" °C");
    Serial.println();
  }

    dht.humidity().getEvent(&event);
  if(isnan(event.relative_humidity)) {
    Serial.println("Error while reading humidity.");
  } else {
    Serial.print("Humidity: ");
    Serial.print(event.relative_humidity);
    Serial.print(" %");
    Serial.println();
  }

  }
  

}

