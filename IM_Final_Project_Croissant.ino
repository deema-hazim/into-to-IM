//defining variables
#define ENC_A 2 //encoder A for rotary encoder
#define ENC_B 3 //encoder B for rotary encoder

#define BTN 9 //pin for button
#define LED_R 12 //pin for red LED light
#define LED_Y 11 //pin for yellow LED light
#define LED_G 10 //pin for green LED light

//timestamps that track when the encoder last moved clockwise or counter-clockwise, used for speed detection
unsigned long _lastIncReadTime = micros();
unsigned long _lastDecReadTime = micros();
int _pauseLength = 25000; //if 2 clicks happen within 0.025 seconds, it counts as "fast turning"
int _fastIncrement = 3; //jump by 3 instead of 1 if it's a fast turn

volatile int counter = 0;
int lastBtnState = HIGH;

void setup() {
  Serial.begin(9600);

  pinMode(ENC_A, INPUT_PULLUP);
  pinMode(ENC_B, INPUT_PULLUP);
  //attachInterrupt tells Arduino to immediately run the read_encoder function whenever pin ENC_A or ENC_B change
  //this is so that it immediately changes the rotator value when it is used and doesn't have to wait for loop to check
  attachInterrupt(digitalPinToInterrupt(ENC_A), read_encoder, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENC_B), read_encoder, CHANGE);

  pinMode(BTN, INPUT_PULLUP);
  pinMode(LED_Y, OUTPUT);
  pinMode(LED_R, OUTPUT);
  pinMode(LED_G, OUTPUT);

  //turn yellow LED on by default at initial startup
  setLED('Y');
}

void loop() {
  //reading information from p5
  while (Serial.available() > 0) {
    char cmd = (char)Serial.read();
    if (cmd == 'X') counter = 0;    // reset encoder value to zero. sent from p5 at the beginning of a new round of the game
    else setLED(cmd);    //turn on led lights depending on value sent from p5 (either "Y", "G", or "R")
  }

  //bind the encoder to these values so no matter how much the user turns it, it can only go from 0-100
  if (counter > 100) { counter = 100; }
  if (counter < 0) { counter = 0; }

  int btnState = digitalRead(BTN);   //read the state of the button
  //to prevent holding the button counting as multiple buttons, this flag only turns true when button goes from unpressed to pressed (HIGH to LOW)
  bool justPressed = (lastBtnState == HIGH && btnState == LOW);
  lastBtnState = btnState;   //read button state

  //printing to the serial monitor
  // this prints "counter value,button state\n" (e.g "75,0" -> counter=75 and button was not clicked)
  Serial.print(counter);
  Serial.print(',');

  if (justPressed) {
    Serial.println(1); //button pressed -> print 1
  } else {
    Serial.println(0); //print 0 as long as the button has not been pressed
  }

  delay(50);
}

void read_encoder() {
  //static means these variables remember their value between calls and do not reset each time the function runs
  static uint8_t old_AB = 3;  //remembers the previous state of both encoders. starts at 3 because in binary that is (00000011)
  static int8_t encval = 0;   //accumulates encoder movement. When it reaches +4 or -4, that counts as one full click

  //enc_states is a list of 16 possible combinations for encoder pin states
  //given the old and new states of both pins, this tells us if the encoder moved forward (+1), backward (-1), or neither (0)
  static const int8_t enc_states[] = { 0, -1, 1, 0, 1, 0, 0, -1, -1, 0, 0, 1, 0, 1, -1, 0 };

  //the << symbol is used to binary shift to the left
  old_AB <<= 2;    //binary shift the old encoder value by 2 bits to make room for new states
  if (digitalRead(ENC_A)) old_AB |= 0x02;    //if pin A is HIGH, set bit 1 of old_AB. So the current state of pin A is now stored
  if (digitalRead(ENC_B)) old_AB |= 0x01;    //if pin B is HIGH, set bit 0 of old_AB. Now old_AB contains both the old and new states of both pins in 4 bits
  encval += enc_states[(old_AB & 0x0f)];    //Use the bottom 4 bits of old_AB as an index into the lookup table. Add whatever the table says (+1, -1, or 0) to encval

  //if we've accumulated more than 3 steps forward, that's one full physical click clockwise
  if (encval > 3) {
    //default change value is 1
    int changevalue = 1;

    //but if the last clockwise click happened less than 25000 microseconds ago, we're turning fast so multiply the change by 3
    if ((micros() - _lastIncReadTime) < _pauseLength) {
      changevalue = _fastIncrement * changevalue;
    }

    _lastIncReadTime = micros();  //record when this clockwise click happened
    counter = counter + changevalue;   //update counter by either 1 or 3, depending on how fast we're turning it
    encval = 0;   //reset the encoder accumulator
  } 
  
  //this follows the same logic as above but for counter-clockwise turns, meaning it just subtracts from the counter instead of adds
  else if (encval < -3) {
    int changevalue = -1;
    if ((micros() - _lastDecReadTime) < _pauseLength) {
      changevalue = _fastIncrement * changevalue;
    }
    _lastDecReadTime = micros();
    counter = counter + changevalue;
    encval = 0;
  }
}

void setLED(char cmd) {
  //when p5 sends a 'Y' signal, turn on the yellow LED and turn off the green and red
  if (cmd == 'Y') {
    digitalWrite(LED_Y, HIGH);
    digitalWrite(LED_R, LOW);
    digitalWrite(LED_G, LOW);
  } 
  //when p5 sends a 'R' signal, turn on the red LED and turn off the green and yellow
  else if (cmd == 'R') {
    digitalWrite(LED_Y, LOW);
    digitalWrite(LED_R, HIGH);
    digitalWrite(LED_G, LOW);
  } 
  //when p5 sends a 'G' signal, turn on the green LED and turn off the yellow and red  
  else if (cmd == 'G') {
    digitalWrite(LED_Y, LOW);
    digitalWrite(LED_R, LOW);
    digitalWrite(LED_G, HIGH);
  }
}
