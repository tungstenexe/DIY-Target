/********************************************************************************************************
* Nerf - DIY TARGET
*
* Description
* program for a DIY Target
*
* created  03 Jun 2019
* modified 12 Jun 2019
* by TungstenEXE
* 
* For non commercial use
* 
* If you find my code useful, do support me by subscribing my YouTube Channel, thanks.
*
* My YouTube Channel Link - Nerf related
* https://www.youtube.com/tungstenexe
* 
********************************************************************************************************/
#define PIN_VIBRATION        A5
#define PIN_LIGHT_R          11
#define PIN_LIGHT_G          10
#define PIN_LIGHT_B           9

#define PIN_RESET_COUNT       2
#define PIN_DISPLAY_DATA      5    
#define PIN_DISPLAY_CLOCK     6    
#define PIN_DISPLAY_LATCH     7    

#define DELAY_HIT_ON          2000
#define THRESHOLD_HIT         2000

// For 7 Segment LED display
byte num_digits []            = {0b11000000,0b11111001,0b10100100,0b10110000,0b10011001,0b10010010,0b10000010,0b11111000,0b10000000,0b10010000}; // digit 0 to 9

boolean hitDetected           = false;
int     hitCount              = 0;

void setup() {
  pinMode(PIN_VIBRATION    , INPUT);
  
  pinMode(PIN_LIGHT_R      , OUTPUT);
  pinMode(PIN_LIGHT_G      , OUTPUT);
  pinMode(PIN_LIGHT_B      , OUTPUT);

  pinMode(PIN_DISPLAY_DATA , OUTPUT);
  pinMode(PIN_DISPLAY_CLOCK, OUTPUT);
  pinMode(PIN_DISPLAY_LATCH, OUTPUT);

  pinMode(PIN_RESET_COUNT  , INPUT_PULLUP);   // PULLUP
  
  RGB_color(0, 255, 255);
  updateSettingDisplay();
}

void loop() {
  if (digitalRead(PIN_RESET_COUNT) == LOW) {
    hitCount = 0;
    updateSettingDisplay();    
  } else {
    if (!hitDetected) {
      hitDetected = (getReading() >= THRESHOLD_HIT); 
    } else {
      RGB_color(255, 0, 255);
      hitCount++;
      updateSettingDisplay();
      delay(DELAY_HIT_ON);
      do {
        hitDetected = (getReading() >= THRESHOLD_HIT);
      } while (hitDetected);
      RGB_color(0, 255, 255);
    }
  }
}

long getReading() {
  return pulseIn(PIN_VIBRATION, HIGH);
}

void RGB_color(int rValue, int gValue, int bValue) {
  analogWrite(PIN_LIGHT_R, rValue);
  analogWrite(PIN_LIGHT_G, gValue);
  analogWrite(PIN_LIGHT_B, bValue);
}

void updateSettingDisplay() {
  digitalWrite(PIN_DISPLAY_LATCH, LOW);
  shiftOut(PIN_DISPLAY_DATA, PIN_DISPLAY_CLOCK, MSBFIRST, num_digits[hitCount % 10]); 
  shiftOut(PIN_DISPLAY_DATA, PIN_DISPLAY_CLOCK, MSBFIRST, num_digits[hitCount / 10]); 
  digitalWrite(PIN_DISPLAY_LATCH, HIGH);    
}
