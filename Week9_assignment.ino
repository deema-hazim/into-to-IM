//declaring for ultrasonic sensor
int trigPin = 10;
int echoPin = 9;

//LED
int redLED = 3;
int greenLED = 6;
int brightness = 0;

//to calculate the input from the ultrasonic sensor
int distanceThreshold = 10;  //in cm
float duration, distanceCm;

//pin number for the slide switch
int switchPin = 2;


void setup() {
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(redLED, OUTPUT);
  pinMode(switchPin, INPUT);
  pinMode(greenLED, OUTPUT);
}

void loop() {
  //sends a 10 microsecond pulse
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  //waits until the echo comes back and measures how long it took
  duration = pulseIn(echoPin, HIGH);

  //Sound travels at 0.034 cm per microsecond
  //Divide by 2 for the go and return trip = 0.017
  distanceCm = 0.017 * duration;

  //restricts values from 3 to 210 so that light completely turns off at <=3 cm and is brightest at >=210
  distanceCm = constrain(distanceCm, 3, 210);
  //convert distance into LED brightness
  brightness = map(distanceCm, 3, 210, 0, 255);

  //read and store the state of the digital slide switch
  int slideSwitchState = digitalRead(switchPin);

  //if the switch is ON
  if (slideSwitchState == HIGH) {
    digitalWrite(greenLED, LOW);      //turn off the green LED
    analogWrite(redLED, brightness);  //control red LED's brightness
  } else {
    digitalWrite(redLED, LOW);          //turn off the red LED
    analogWrite(greenLED, brightness);  //control green LED's brightness
  }

  //print the value to Serial Monitor
  Serial.print("distance: ");
  Serial.print(distanceCm);
  Serial.println(" cm");

  //wait before the next reading, otherwise LED light constantly flickers
  delay(500);
}
