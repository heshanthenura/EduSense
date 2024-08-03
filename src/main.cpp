#include <Arduino.h>
#include <WiFi.h>
#include <SPIFFS.h>
#include <ESPAsyncWebServer.h>
#include <ESP32Servo.h>
#include <DHT.h>
#include <DHT_U.h>

const int triggerPin1 = 13;  
const int echoPin1 = 12;     

const int triggerPin2 = 14;  
const int echoPin2 = 27;    

#define SOUND_SPEED 0.034
#define CM_TO_INCH 0.393701

const float DISTANCE_THRESHOLD = 20.0; 
const unsigned long DETECTION_TIME = 500;

unsigned long startTime1 = 0;
unsigned long startTime2 = 0;

bool handDetected1 = false;
bool handDetected2 = false;


void getDistance(int triggerPin, int echoPin, float &distanceCm, float &distanceInch) {
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


void setup() {
  Serial.begin(115200); 
  pinMode(triggerPin1, OUTPUT); 
  pinMode(echoPin1, INPUT); 
  
  pinMode(triggerPin2, OUTPUT);
  pinMode(echoPin2, INPUT); 
}

void loop() {
  float distanceCm1, distanceInch1;
  float distanceCm2, distanceInch2;


  getDistance(triggerPin1, echoPin1, distanceCm1, distanceInch1);
  getDistance(triggerPin2, echoPin2, distanceCm2, distanceInch2);

  if (distanceCm1 <= DISTANCE_THRESHOLD) {
    if (!handDetected1) {
      startTime1 = millis();
      handDetected1 = true;
    } else if (millis() - startTime1 >= DETECTION_TIME) {
      Serial.println("Answered 1");
      handDetected1 = false;
      startTime1=0;
    }
  } else {
    handDetected1 = false;
  }

  if (distanceCm2 <= DISTANCE_THRESHOLD) {
    if (!handDetected2) {
      startTime2 = millis();
      handDetected2 = true;
    } else if (millis() - startTime2 >= DETECTION_TIME) {
      Serial.println("Answered 2");
      handDetected2 = false; 
      startTime2=0;
    }
  } else {
    handDetected2 = false; 
  }
  delay(100); 
}

