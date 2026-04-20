int led = 9;
int pot = A0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(led, OUTPUT);
  pinMode(pot, INPUT);
}

void loop() {
  //read pot value
  int potValue = analogRead(pot);
  //map the 0-1023 value to a smaller range for the wind (-2 to 2)
  float windForce = map(potValue, 0, 1023, -100, 100) / 100.0; 
  //print value to the serial monitor to "send" it to p5
  Serial.println(windForce);

  //receive data from p5
  if (Serial.available() > 0) {
    char command = Serial.read();
    if (command == 'H') { //If p5 sends 'HIGH' turn LED on
      digitalWrite(led, HIGH);
    } else if (command == 'L') { // If p5 sends 'LOW' turn LED off
      digitalWrite(led, LOW);
    }
  }
}
