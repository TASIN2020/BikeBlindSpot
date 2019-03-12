#include "LedControl.h"

#define CLK_PIN 10
#define CS_PIN 11
#define DIN_PIN 12
LedControl display = LedControl(DIN_PIN, CLK_PIN, CS_PIN);

const uint64_t LEFT_ARROW = 0x000004027f020400;
const uint64_t RIGHT_ARROW = 0x0404040404150e04;


void displayImage(uint64_t image);


long readUltrasonicDistance(int triggerPin, int echoPin)
{
  pinMode(triggerPin, OUTPUT);  // Clear the trigger
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

void ledCondition(int sensor, bool led_status){
  
  // Here 10, 11, 13, 14 are led annode
  
  switch (sensor) {
  case 1:
//    digitalWrite(10, !led_status);
    displayImage(LEFT_ARROW);
    break;
  case 2:
//    digitalWrite(11, !led_status);
    break;
  case 3:
//    digitalWrite(12, !led_status);
    break;
  case 4:
//    digitalWrite(13, !led_status);
    break;
  default:
    break;
  }
}

void detectDistance(int sensor, int distance)
{
  if(distance >= 330 || distance == 0){
    Serial.print (sensor);
    Serial.println(": is Out of range");
    ledCondition(sensor, 0);
  }else{
    Serial.print ( sensor );
    Serial.print ( ": " );
    Serial.print ( distance);
    Serial.println("cm");
    ledCondition(sensor, 1);
  }
}

void setup() {
  Serial.begin (9600);
  pinMode(10, OUTPUT);
//  pinMode(11, OUTPUT);
//  pinMode(12, OUTPUT);
//  pinMode(13, OUTPUT);


  display.clearDisplay(0);
  display.shutdown(0, false);
  display.setIntensity(0, 10);

}

void loop() {

  display.clearDisplay(0);
  long distance_one = 0.01723 * readUltrasonicDistance(2,3);
  detectDistance(1, distance_one);

/*
  
  long distance_two = 0.01723 * readUltrasonicDistance(4,5);
  detectDistance(2, distance_two);
  
  long distance_three = 0.01723 * readUltrasonicDistance(6,7);
  detectDistance(3, distance_three);
  
  long distance_four = 0.01723 * readUltrasonicDistance(8,9);
  detectDistance(4, distance_four);

  */  
}


void displayImage(uint64_t image) {
  for (int i = 0; i < 8; i++) {
    byte row = (image >> i * 8) & 0xFF;
    for (int j = 0; j < 8; j++) {
      display.setLed(0, i, j, bitRead(row, j));
    }
  }
}
