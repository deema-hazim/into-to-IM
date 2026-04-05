//variables for the analog input ultrasonic sensor switch
int trigPin = 10;
int echoPin = 9;
int ultrasonicLED = 3;
int distanceThreshold = 10;  //in cm
float duration, distanceCm;
int brightness = 0;

//variables for the digital input slide switch
int switchPin = 2;
int switchLED = 6;

void setup() {
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(ultrasonicLED, OUTPUT);
  pinMode(switchPin, INPUT);
  pinMode(switchLED, OUTPUT);
}

void loop() {
  //analog sensor
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  // calculate the distance
  distanceCm = 0.017 * duration;

  distanceCm = constrain(distanceCm, 3, 210);
  brightness = map(distanceCm, 3, 210, 0, 255);

  //store the state of the digital slide switch
  int slideSwitchState = digitalRead(switchPin);

  if (slideSwitchState == HIGH) {
    digitalWrite(switchLED, LOW);  //turn off the unused LED when the switch changes them
    analogWrite(ultrasonicLED, brightness); //set brightness 
  } else {
    digitalWrite(ultrasonicLED, LOW);  //turn off the unused LED when the switch changes them
    analogWrite(switchLED, brightness);
  }

  //print the value to Serial Monitor
  Serial.print("distance: ");
  Serial.print(distanceCm);
  Serial.println(" cm");

  delay(500);
}
