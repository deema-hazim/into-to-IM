#include "pitches.h"

#define BUZZER_PIN 2
#define PIANO_YELLOW 8
#define PIANO_BLUE 9
#define PIANO_RED 10
#define POT_PIN A0

int melody[] = {
  NOTE_E4, NOTE_F4, NOTE_G4, NOTE_E5, NOTE_C5, NOTE_D5, NOTE_C5, NOTE_C5, NOTE_B4, NOTE_B4, NOTE_D4,
  NOTE_E4, NOTE_F4, NOTE_D5, NOTE_B4, NOTE_C5, NOTE_B4, NOTE_A4, NOTE_G4, NOTE_G4, NOTE_E4, NOTE_F4,
  NOTE_G4, NOTE_C5, NOTE_D5, NOTE_E5, NOTE_D5, NOTE_C5, NOTE_A4, NOTE_D5, NOTE_E5, NOTE_F5, NOTE_E5,
  NOTE_D5, NOTE_G4, NOTE_F5, NOTE_E5, NOTE_D5, NOTE_C5, NOTE_C5, REST, NOTE_C5, REST, NOTE_C5, NOTE_E5,
  NOTE_C5, NOTE_D5, REST, NOTE_D5, NOTE_D5, NOTE_D5, REST, NOTE_D5, NOTE_F5, NOTE_D5, NOTE_E5, REST, NOTE_E5,
  NOTE_E5, NOTE_E5, REST, NOTE_E5, NOTE_G5, NOTE_E5, NOTE_F5, REST, NOTE_F5, NOTE_F5, NOTE_E5, NOTE_D5, NOTE_G4,
  NOTE_B4, NOTE_C5, NOTE_C5, REST
};

int durations[] = {
  8, 8, 4, 4, 4, 8, 8, 4, 4, 4, 8, 8, 4, 4, 4, 8, 8, 4, 4, 4, 8, 8, 4, 8, 8, 4, 8, 8, 4,
  8, 8, 4, 8, 8, 4, 4, 4, 4, 2, 4, 4, 4, 8, 8, 4, 4, 4, 8, 8, 2, 4, 8, 8, 4, 4, 4, 8, 8,
  2, 4, 8, 8, 4, 4, 4, 8, 8, 4, 8, 8, 2, 2, 2, 4, 4
};

//tracks which note in the melody to play next
int note = 0;

//store what each button's state was during the previous loop
int prev_yellow = HIGH;
int prev_blue = HIGH;
int prev_red = HIGH;

void setup() {
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(POT_PIN, INPUT);

  //INPUT_PULLUP sets the pin as an input and activates an internal resistor that pulls the pin to HIGH by default
  pinMode(PIANO_YELLOW, INPUT_PULLUP);
  pinMode(PIANO_BLUE, INPUT_PULLUP);
  pinMode(PIANO_RED, INPUT_PULLUP);
}

void loop() {
  int size = sizeof(durations) / sizeof(int);
  int state_yellow = digitalRead(PIANO_YELLOW);
  int state_blue = digitalRead(PIANO_BLUE);
  int state_red = digitalRead(PIANO_RED);

  int pot_value = analogRead(POT_PIN); //0-1023 value

  //rescales the 0–1023 range to 50–200, and dividing by 100.0 gives the float range 0.50–2.00 for the pitch multiplier
  float pitchMultiplier = map(pot_value, 0, 1023, 50, 200) / 100.0;

  //checks if the button is currently pressed and wasn't pressed last loop so only the exact moment of transition triggers justPressed
  bool justPressed = (state_yellow == LOW && prev_yellow == HIGH) 
                      || (state_blue == LOW && prev_blue == HIGH) 
                      || (state_red == LOW && prev_red == HIGH);

  if (justPressed) {
    int duration = 1000 / durations[note];

    int shiftedNote = (int)(melody[note] * pitchMultiplier);

    tone(BUZZER_PIN, shiftedNote, duration);

    int pauseBetweenNotes = duration * 1.30;
    //waits slightly longer than the note itself for smooth sound
    delay(pauseBetweenNotes);

    noTone(BUZZER_PIN);
    //advances to the next note and % size wraps back to 0 once the whole melody has played
    note = (note + 1) % size;
  }

  //Save current states for next loop iteration
  prev_yellow = state_yellow;
  prev_blue = state_blue;
  prev_red = state_red;
}
