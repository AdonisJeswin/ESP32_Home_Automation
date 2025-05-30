#include <WiFi.h>
#include <WebServer.h>
#include <DHT.h>
#include <math.h>  // Needed for sin(), cos() and PI

// ----- DHT Sensor Setup -----
#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// ----- Relay Pin Definitions -----
#define RELAY_FAN 5
#define RELAY_BULB 12

// ----- WiFi Credentials -----
// REPLACE with your WiFi SSID and PASSWORD
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

// ----- Create a Web Server -----
WebServer server(80);

// ----- Global Variables -----
float currentTemp = 0.0;
float currentHumidity = 0.0;
bool manualFanState = false;
bool manualBulbState = false;

unsigned long sensorInterval = 2000;
unsigned long lastSensorRead = 0;

String generateWebPage() {
  float tempAngle = PI * (1 - (currentTemp / 50.0));
  float r = 90.0;
  float centerX = 100.0;
  float centerY = 100.0;
  float tempX = centerX + r * cos(tempAngle);
  float tempY = centerY - r * sin(tempAngle);

  float humAngle = PI * (1 - (currentHumidity / 100.0));
  float humX = centerX + r * cos(humAngle);
  float humY = centerY - r * sin(humAngle);

  String page = "<!DOCTYPE html><html><head><meta charset='UTF-8'>";
  page += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
  page += "<meta http-equiv='refresh' content='1'>";
  page += "<title>ESP32 Home Automation</title>";
  page += "<style>";
  page += "body { font-family: Arial, sans-serif; background: #222; color: #f4f4f4; text-align: center; margin: 0; padding: 0; }";
  page += ".container { width: 90%; max-width: 600px; margin: 20px auto; padding: 20px; background: #333; border-radius: 10px; box-shadow: 0px 4px 10px rgba(0,0,0,0.5); }";
  page += "h1 { color: #f1c40f; }";
  page += ".card { background: #444; padding: 15px; border-radius: 8px; margin: 10px; font-size: 18px; }";
  page += "button { padding: 12px 20px; margin: 10px; font-size: 18px; border: none; border-radius: 5px; cursor: pointer; transition: 0.3s; }";
  page += ".on { background: #27ae60; color: white; }";
  page += ".off { background: #e74c3c; color: white; }";
  page += "button:hover { opacity: 0.8; }";
  page += "</style></head><body>";

  page += "<div class='container'>";
  page += "<h1>ESP32 Home Automation</h1>";

  page += "<div class='card'><strong>Temperature:</strong><br>";
  page += "<svg width='200' height='120'>";
  page += "<path d='M10,100 A90,90 0 0,1 190,100' fill='none' stroke='#ccc' stroke-width='20' />";
  page += "<path d='M10,100 A90,90 0 0,1 " + String(tempX,2) + "," + String(tempY,2) + "' fill='none' stroke='#27ae60' stroke-width='20' />";
  page += "<text x='100' y='105' text-anchor='middle' font-size='30' fill='#fff'>" + String(currentTemp, 2) + "°C</text>";
  page += "</svg></div>";

  page += "<div class='card'><strong>Humidity:</strong><br>";
  page += "<svg width='200' height='120'>";
  page += "<path d='M10,100 A90,90 0 0,1 190,100' fill='none' stroke='#ccc' stroke-width='20' />";
  page += "<path d='M10,100 A90,90 0 0,1 " + String(humX,2) + "," + String(humY,2) + "' fill='none' stroke='#2980b9' stroke-width='20' />";
  page += "<text x='100' y='105' text-anchor='middle' font-size='30' fill='#fff'>" + String(currentHumidity, 2) + "%</text>";
  page += "</svg></div>";

  page += "<div class='card'><strong>Fan:</strong> " + String((digitalRead(RELAY_FAN) == LOW) ? "ON" : "OFF") + "</div>";
  page += "<div class='card'><strong>Bulb:</strong> " + String((digitalRead(RELAY_BULB) == LOW) ? "ON" : "OFF") + "</div>";

  page += "<p><a href='/fan/on'><button class='on'>Fan ON</button></a>";
  page += "<a href='/fan/off'><button class='off'>Fan OFF</button></a></p>";
  page += "<p><a href='/bulb/on'><button class='on'>Bulb ON</button></a>";
  page += "<a href='/bulb/off'><button class='off'>Bulb OFF</button></a></p>";
  page += "<p><em>Note: Fan auto-turns ON above 35°C.</em></p>";
  page += "</div></body></html>";

  return page;
}

void handleRoot() {
  server.send(200, "text/html", generateWebPage());
}

void handleFanOn() {
  manualFanState = true;
  server.sendHeader("Location", "/");
  server.send(303);
}

void handleFanOff() {
  manualFanState = false;
  server.sendHeader("Location", "/");
  server.send(303);
}

void handleBulbOn() {
  manualBulbState = true;
  server.sendHeader("Location", "/");
  server.send(303);
}

void handleBulbOff() {
  manualBulbState = false;
  server.sendHeader("Location", "/");
  server.send(303);
}

void setup() {
  Serial.begin(115200);
  dht.begin();

  pinMode(RELAY_FAN, OUTPUT);
  pinMode(RELAY_BULB, OUTPUT);
  digitalWrite(RELAY_FAN, HIGH);
  digitalWrite(RELAY_BULB, HIGH);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Connected! IP Address: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/fan/on", handleFanOn);
  server.on("/fan/off", handleFanOff);
  server.on("/bulb/on", handleBulbOn);
  server.on("/bulb/off", handleBulbOff);

  server.begin();
}

void loop() {
  server.handleClient();

  unsigned long currentMillis = millis();
  if (currentMillis - lastSensorRead >= sensorInterval) {
    lastSensorRead = currentMillis;
    float temp = dht.readTemperature();
    float hum = dht.readHumidity();
    if (!isnan(temp) && !isnan(hum)) {
      currentTemp = temp;
      currentHumidity = hum;
    }
  }

  if (currentTemp > 35) {
    digitalWrite(RELAY_FAN, LOW);
  } else {
    digitalWrite(RELAY_FAN, manualFanState ? LOW : HIGH);
  }

  digitalWrite(RELAY_BULB, manualBulbState ? LOW : HIGH);

  delay(500);
}