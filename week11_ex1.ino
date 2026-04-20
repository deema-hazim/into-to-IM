int trigPin=9;
int echoPin=7;

//to calculate the input from the ultrasonic sensor
int distanceThreshold = 10;  //in cm
float duration, distanceCm;

void setup() {
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
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

  //restricts values from 3 to 210 so that anything <=3 cm is considered 0 and >=210 is the max
  distanceCm = constrain(distanceCm, 3, 400);
  //convert distance into size of canvas 
  int mappedDistance = map(distanceCm, 3, 400, 0, 400);

  //print the value to Serial Monitor
  Serial.println(mappedDistance);

  //wait before the next reading, otherwise LED light constantly flickers
  delay(500);
}
