#include <Arduino.h>
#include <WiFi.h>
#include <SPIFFS.h>
#include <ESPAsyncWebServer.h>
#include <ESP32Servo.h>
#include <DHT.h>
#include <DHT_U.h>

const char *ssid = "EduSense";
const char *password = "12345678";

AsyncWebServer server(80);

const int triggerPin1 = 13;
const int echoPin1 = 12;

const int triggerPin2 = 14;
const int echoPin2 = 27;

const int triggerPin3 = 26;
const int echoPin3 = 25;

const int triggerPin4 = 33;
const int echoPin4 = 32;

#define SOUND_SPEED 0.034
#define CM_TO_INCH 0.393701

const float DISTANCE_THRESHOLD = 20.0;
const unsigned long DETECTION_TIME = 500;

unsigned long startTime1 = 0;
unsigned long startTime2 = 0;
unsigned long startTime3 = 0;
unsigned long startTime4 = 0;

bool handDetected1 = false;
bool handDetected2 = false;
bool handDetected3 = false;
bool handDetected4 = false;

String detectedAnswer = "0"; // Initialize detectedAnswer to "0"

void initSPIFFS()
{
  if (!SPIFFS.begin())
  {
    Serial.println("An error has occurred while mounting SPIFFS");
  }
  else
  {
    Serial.println("SPIFFS mounted successfully");
    File root = SPIFFS.open("/");
    File file = root.openNextFile();

    Serial.println("SPIFFS Files:");
    while (file)
    {
      Serial.print("FILE: ");
      Serial.println(file.name());
      file = root.openNextFile();
    }
  }
}

void initServer()
{
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/index.html", "text/html"); });

  server.on("/index.css", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/index.css", "text/css"); });

  // Serve JS file
  server.on("/index.js", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/index.js", "application/javascript"); });

  server.on("/star.png", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/star.png", "image/png"); });
  server.on("/triangle.png", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/triangle.png", "image/png"); });
  server.on("/square.png", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/square.png", "image/png"); });
  server.on("/circle.png", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/circle.png", "image/png"); });
  server.on("/logo.png", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/logo.png", "image/png"); });

  server.on("/answer", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(200, "text/plain", detectedAnswer); });

  // Endpoint to reset the detected answer
  server.on("/reset", HTTP_GET, [](AsyncWebServerRequest *request)
            {
              detectedAnswer = "0"; // Reset detectedAnswer to "0"
              request->send(200, "text/plain", "Answer reset");
            });

  server.begin();
}

void getDistance(int triggerPin, int echoPin, float &distanceCm, float &distanceInch)
{
  long duration;
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distanceCm = duration * SOUND_SPEED / 2;
  distanceInch = distanceCm * CM_TO_INCH;
}

void setup()
{
  Serial.begin(115200);
  pinMode(triggerPin1, OUTPUT);
  pinMode(echoPin1, INPUT);

  pinMode(triggerPin2, OUTPUT);
  pinMode(echoPin2, INPUT);

  pinMode(triggerPin3, OUTPUT);
  pinMode(echoPin3, INPUT);

  pinMode(triggerPin4, OUTPUT);
  pinMode(echoPin4, INPUT);

  initSPIFFS();
  WiFi.softAP(ssid, password);

  Serial.println("Access Point Started");
  Serial.print("IP Address: ");
  Serial.println(WiFi.softAPIP());
  initServer();
}

void loop()
{
  float distanceCm1, distanceInch1;
  float distanceCm2, distanceInch2;
  float distanceCm3, distanceInch3;
  float distanceCm4, distanceInch4;

  getDistance(triggerPin1, echoPin1, distanceCm1, distanceInch1);
  getDistance(triggerPin2, echoPin2, distanceCm2, distanceInch2);
  getDistance(triggerPin3, echoPin3, distanceCm3, distanceInch3);
  getDistance(triggerPin4, echoPin4, distanceCm4, distanceInch4);

  if (distanceCm1 <= DISTANCE_THRESHOLD)
  {
    if (!handDetected1)
    {
      startTime1 = millis();
      handDetected1 = true;
    }
    else if (millis() - startTime1 >= DETECTION_TIME)
    {
      Serial.println("Answered 1");
      detectedAnswer = "1"; // Store the detected answer
      handDetected1 = false;
      startTime1 = 0;
    }
  }
  else
  {
    handDetected1 = false;
  }

  if (distanceCm2 <= DISTANCE_THRESHOLD)
  {
    if (!handDetected2)
    {
      startTime2 = millis();
      handDetected2 = true;
    }
    else if (millis() - startTime2 >= DETECTION_TIME)
    {
      Serial.println("Answered 2");
      detectedAnswer = "2"; // Store the detected answer
      handDetected2 = false;
      startTime2 = 0;
    }
  }
  else
  {
    handDetected2 = false;
  }

  if (distanceCm3 <= DISTANCE_THRESHOLD)
  {
    if (!handDetected3)
    {
      startTime3 = millis();
      handDetected3 = true;
    }
    else if (millis() - startTime3 >= DETECTION_TIME)
    {
      Serial.println("Answered 3");
      detectedAnswer = "3"; // Store the detected answer
      handDetected3 = false;
      startTime3 = 0;
    }
  }
  else
  {
    handDetected3 = false;
  }

  if (distanceCm4 <= DISTANCE_THRESHOLD)
  {
    if (!handDetected4)
    {
      startTime4 = millis();
      handDetected4 = true;
    }
    else if (millis() - startTime4 >= DETECTION_TIME)
    {
      Serial.println("Answered 4");
      detectedAnswer = "4"; // Store the detected answer
      handDetected4 = false;
      startTime4 = 0;
    }
  }
  else
  {
    handDetected4 = false;
  }

  delay(500);
}
