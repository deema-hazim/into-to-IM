int led = 9;

void setup() {
  Serial.begin(9600);
  pinMode(led, OUTPUT);
}

void loop() {
  if (Serial.available() > 0) {
    //read the incoming number from p5 until it sees a new line
    int brightness = Serial.parseInt(); 
    
    //constrain the value to the range 0-255
    brightness = constrain(brightness, 0, 255);
    
    //set the LED's brightness
    analogWrite(led, brightness);
  }
}
