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
    digitalWrite(10, !led_status);
    break;
  case 2:
    digitalWrite(11, !led_status);
    break;
  case 3:
    digitalWrite(12, !led_status);
    break;
  case 4:
    digitalWrite(13, !led_status);
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
}

void loop() {
  
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
