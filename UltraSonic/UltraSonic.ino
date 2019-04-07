/**
  CSE-341 Micro-Controller Project
  Name: BikeBlindSpot Doc
  Purpose: Code to detect the proximity of nearby vehicle and warn driver.

  @author Adyel Ullahil Mamun
  @version 0.5.5 17/03/2019
*/

#include "LedControl.h"


// Configuration

constexpr auto CLK_PIN = 10;
constexpr auto CS_PIN = 11;
constexpr auto DIN_PIN = 12;

constexpr auto BUZZER_PIN = 4;

// Ultrasonic Sensor Trigger Pin
// This must be set to use readUltrasonicDistance(int echoPin)
// function
constexpr int COMMON_TRIG_PIN = 2;

LedControl display = LedControl(DIN_PIN, CLK_PIN, CS_PIN);

// IMAGE Data
// Check here https://xantorohara.github.io/led-matrix-editor/
const uint64_t LEFT_ARROW = 0x0f03050910200000;
const uint64_t RIGHT_ARROW = 0xf0c0a09008040000;
const uint64_t FORWARD_ARROW = 0x0004040404150e04;
const uint64_t BACKWARD_ARROW = 0x040e150404040400;


// GLOBAL Variable
unsigned int MIN_DISTANCE = 360;
unsigned int CLOSEST_PING;

// All Functions
void displayImage(uint64_t image);
long readUltrasonicDistance(int triggerPin, int echoPin);
long readUltrasonicDistance(int echoPin);
void ledCondition(int sensor);
void updateClosestPing(long distance, int sensor);
void detectDistance(int sensor, long distance);
void buzz();

void setup() {
  Serial.begin(9600);

  display.clearDisplay(0);
  display.shutdown(0, false);
  display.setIntensity(0, 10);
}

bool status = false;
int buttonSum = 0;

void loop() {


  int button = digitalRead(14);
  delay(150);
  Serial.println(button);

  buttonSum += button;

  if (button == 1 && buttonSum < 5){
    status = true;
    display.clearDisplay(0);
    display.shutdown(0, false);
  }else if (buttonSum > 5){
    status = false;
    display.clearDisplay(0);
    display.shutdown(0, true);
    delay(1500);
    buttonSum = 0;
  }
  

  if (status) {

    MIN_DISTANCE = 360;

    long distance_one = 0.01723 * readUltrasonicDistance(3);
    detectDistance(1, distance_one);

    long distance_two = 0.01723 * readUltrasonicDistance(5);
    detectDistance(2, distance_two);

    long distance_three = 0.01723 * readUltrasonicDistance(7);
    detectDistance(3, distance_three);

    long distance_four = 0.01723 * readUltrasonicDistance(9);
    detectDistance(4, distance_four);

    ledCondition(CLOSEST_PING);
//    delay(1500);

  }
}

void ledCondition(int sensor) {

  switch (sensor) {
    case 1:
      displayImage(FORWARD_ARROW);
      break;
    case 2:
      displayImage(BACKWARD_ARROW);
      break;
    case 3:
      displayImage(LEFT_ARROW);
      break;
    case 4:
      displayImage(RIGHT_ARROW);
      break;
    default:
      break;
  }
}

void detectDistance(int sensor, long distance) {
  if (distance >= 330 || distance == 0) {
    Serial.print(sensor);
    Serial.println(": is Out of range");
  } else {
    Serial.print(sensor);
    Serial.print(": ");
    Serial.print(distance);
    Serial.println("cm");
    updateClosestPing(distance, sensor);
  }
}

void updateClosestPing(long distance, int sensor) {
  if (MIN_DISTANCE > distance) {
    CLOSEST_PING = sensor;
    MIN_DISTANCE = distance;
  }
}

long readUltrasonicDistance(int triggerPin, int echoPin) {

  pinMode(triggerPin, OUTPUT); // Clear the trigger
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);

  // Sets the trigger pin to HIGH state for 10 microseconds
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  pinMode(echoPin, INPUT);
  // Reads the echo pin, and returns the sound wave travel time in microseconds
  return pulseIn(echoPin, HIGH);
}


// Common Trigger must be defined to use this
long readUltrasonicDistance(int echoPin) {
  return readUltrasonicDistance(COMMON_TRIG_PIN, echoPin);
}

void buzz() {
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);
  delay(50);
  digitalWrite(BUZZER_PIN, HIGH);
}

void displayImage(uint64_t image) {

  buzz();
  display.clearDisplay(0);

  for (int i = 0; i < 8; i++) {
    byte row = (image >> i * 8) & 0xFF;
    for (int j = 0; j < 8; j++) {
      display.setLed(0, i, j, bitRead(row, j));
    }
  }
}
