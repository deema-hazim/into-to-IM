int foilPin = 2;
int buzzer = 8;

void setup() {
  pinMode(foilPin, INPUT_PULLUP);
}

void loop() {
  int state = digitalRead(foilPin);

  if (state == HIGH){ //foils not touching (slouching)
    delay(1500); //wait 1.5 seconds in case person is just moving
    if(digitalRead(foilPin)==HIGH) { //read input again, if person is still slouched
      tone(buzzer, 1000); //tone
    }
  }

  else //foils touching (sitting straight)
    noTone(buzzer);
}