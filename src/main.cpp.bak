#include <Arduino.h>
#include <WiFi.h>
#include <SPIFFS.h>
#include <ESPAsyncWebServer.h>
#include <ESP32Servo.h>
#include <DHT.h>
#include <DHT_U.h>

const char *ssid = "ESP32-AP";
const char *password = "12345678";

AsyncWebServer server(80);

#define window1Pin 23
#define lightSensor 34
#define DHTPIN 4
#define DHTTYPE DHT11
#define moisturePin 32

Servo window1;
DHT dht(DHTPIN, DHTTYPE);

#define TRIGGER_PIN 12
#define ECHO_PIN 13

void setup()
{
    pinMode(moisturePin, INPUT);
    window1.attach(window1Pin);
    dht.begin();
    Serial.begin(115200);

    pinMode(TRIGGER_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);

    if (!SPIFFS.begin(true))
    {
        Serial.println("An Error has occurred while mounting SPIFFS");
        return;
    }

    File root = SPIFFS.open("/");
    File file = root.openNextFile();
    while (file)
    {
        Serial.print("FILE: ");
        Serial.println(file.name());
        file = root.openNextFile();
    }

    WiFi.softAP(ssid, password);

    Serial.println("Access Point Started");
    Serial.print("IP address: ");
    Serial.println(WiFi.softAPIP());

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send(SPIFFS, "/index.html", "text/html"); });

    server.on("/index.css", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send(SPIFFS, "/index.css", "text/css"); });

    server.on("/open/1", HTTP_GET, [](AsyncWebServerRequest *request)
              {
                  Serial.println("Window 1 is opening");
                  window1.write(90);
                  request->send(200, "text/plain", "Window 1 is opening"); });

    server.on("/open/2", HTTP_GET, [](AsyncWebServerRequest *request)
              {
                  Serial.println("Window 2 is opening");
                  request->send(200, "text/plain", "Window 2 is opening"); });

    server.on("/open/3", HTTP_GET, [](AsyncWebServerRequest *request)
              {
                  Serial.println("Window 3 is opening");
                  request->send(200, "text/plain", "Window 3 is opening"); });

    server.on("/open/4", HTTP_GET, [](AsyncWebServerRequest *request)
              {
                  Serial.println("Window 4 is opening");
                  request->send(200, "text/plain", "Window 4 is opening"); });

    server.on("/close/1", HTTP_GET, [](AsyncWebServerRequest *request)
              {
                  Serial.println("Window 1 is closing");
                  window1.write(0);
                  request->send(200, "text/plain", "Window 1 is closing"); });

    server.on("/close/2", HTTP_GET, [](AsyncWebServerRequest *request)
              {
                  Serial.println("Window 2 is closing");
                  request->send(200, "text/plain", "Window 2 is closing"); });

    server.on("/close/3", HTTP_GET, [](AsyncWebServerRequest *request)
              {
                  Serial.println("Window 3 is closing");
                  request->send(200, "text/plain", "Window 3 is closing"); });

    server.on("/close/4", HTTP_GET, [](AsyncWebServerRequest *request)
              {
                  Serial.println("Window 4 is closing");
                  request->send(200, "text/plain", "Window 4 is closing"); });

    server.onNotFound([](AsyncWebServerRequest *request)
                      { request->send(404, "text/plain", "Not Found"); });

    server.on("/light", HTTP_GET, [](AsyncWebServerRequest *request)
              {
                  int lightValue = analogRead(lightSensor);
                  request->send(200, "text/plain", String(lightValue)); });

    server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request)
              {
                  float temperature = dht.readTemperature();
                  request->send(200, "text/plain", String(temperature)); });

    server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest *request)
              {
                  float humidity = dht.readHumidity();
                  request->send(200, "text/plain", String(humidity)); });

    server.begin();
}

long measureDistance()
{
    // Clear the trigger pin
    digitalWrite(TRIGGER_PIN, LOW);
    delayMicroseconds(2);

    // Set the trigger pin to HIGH for 10 microseconds
    digitalWrite(TRIGGER_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIGGER_PIN, LOW);

    // Read the echo pin, the pulse length in microseconds
    long duration = pulseIn(ECHO_PIN, HIGH);

    // Calculate the distance
    long distance = duration * 0.034 / 2;

    return distance;
}

void loop()
{
    long distance = measureDistance();


    Serial.print("Distance: ");
    Serial.println(distance);

    delay(50); // Delay for 1 second
}
