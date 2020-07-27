#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"
#include "Adafruit_NeoPixel.h"

// #define ARCADE_N (14)
// #define ARCADE_S (15)
// #define ARCADE_W (16)
// #define ARCADE_E (17)

#define SPEAKER (10)        // Disconnect while uploading
#define NEOPIXEL_RING (22)
// #define LED_COL_1 (2)
// #define LED_COL_2 (3)
// #define LED_COL_3 (4)
// #define LED_ROW_1 (5)
// #define LED_ROW_2 (6)

// #define BUTTON_COL_1 (7)
// #define BUTTON_COL_2 (8)
// #define BUTTON_COL_3 (9)
// #define BUTTON_ROW_1 (10)
// #define BUTTON_ROW_2 (11)
// #define BUTTON_ROW_3 (12)
// #define TOGGLE_LEDS (13)
#define N_LEDS (16)

#define DEBUG 0

#if (DEBUG == 0)
void chase(uint32_t c);
#endif

enum class Leds {
    Blue1,
    Blue2,
    Blue3,
    Red1,
    Red2,
    Red3,
    Toggles
};

typedef struct ButtonArray {
    bool blue1;
    bool blue2;
    bool blue3;
    bool red1;
    bool red2;
    bool red3;
    bool up;
    bool down;
    bool left;
    bool right;
    bool toggle1;
    bool toggle2;
    uint8_t numButtons = 12;
}ButtonArrray_t;

ButtonArrray_t buttonStates;

void SwitchLed(Leds led, uint8_t enabled);
void ReadButtons();
void HandleButtons();

#if (DEBUG == 0)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(N_LEDS, NEOPIXEL_RING, NEO_GRB + NEO_KHZ800);
#endif
Adafruit_7segment matrix = Adafruit_7segment();

void setup() {
#if (DEBUG == 1)
  Serial.begin(9600);             // Serial uses TX and RX Pins 0 + 1
  Serial.println("ButtonBox TestCode\r\n");
#else
  matrix.begin(0x70); // Init I2C Display
  strip.begin();      // Init LED Strip
  strip.setBrightness(1);    // lower brightness for toddlers
  strip.show(); // Initialize all pixels to 'off'
#endif

  // Init Arcade
  // pinMode(ARCADE_N, INPUT_PULLUP);
  // pinMode(ARCADE_S, INPUT_PULLUP);
  // pinMode(ARCADE_W, INPUT_PULLUP);
  // pinMode(ARCADE_E, INPUT_PULLUP);

  // // Init LEDs to off
  // pinMode(LED_ROW_1, OUTPUT);
  // pinMode(LED_ROW_2, OUTPUT);
  // pinMode(LED_COL_1, OUTPUT);   // TODO verify, might break something watch this.
  // pinMode(LED_COL_2, OUTPUT);
  // pinMode(LED_COL_3, OUTPUT);

  // // TODO(Add led control functions and defines for LED 1-6)
  // // ALL LEDS OFF
  // digitalWrite(LED_ROW_1, HIGH);
  // digitalWrite(LED_ROW_2, HIGH);
  // digitalWrite(LED_COL_1, LOW);
  // digitalWrite(LED_COL_2, LOW);
  // digitalWrite(LED_COL_3, LOW);
  // delay(1000);

  // SwitchLed(Leds::Blue1, 1);
  // delay(1000);
  // SwitchLed(Leds::Blue1, 0);
  // delay(1000);
  // SwitchLed(Leds::Blue2, 1);
  // delay(1000);
  // SwitchLed(Leds::Blue2, 0);
  // delay(1000);
  // SwitchLed(Leds::Blue3, 1);
  // delay(1000);
  // SwitchLed(Leds::Blue3, 0);
  // delay(1000);
  // SwitchLed(Leds::Red1, 1);
  // delay(1000);
  // SwitchLed(Leds::Red1, 0);
  // delay(1000);
  // SwitchLed(Leds::Red2, 1);
  // delay(1000);
  // SwitchLed(Leds::Red2, 0);
  // delay(1000);
  // SwitchLed(Leds::Red3, 1);
  // delay(1000);
  // SwitchLed(Leds::Red3, 0);
  // delay(1000);
}


void loop() {
  static uint16_t data = 0;

  matrix.print(data, DEC);
  matrix.writeDisplay();
  // SwitchLed(Leds::Blue1, 1);
  // SwitchLed(Leds::Blue2, 1);
  // SwitchLed(Leds::Blue3, 1);
  // SwitchLed(Leds::Red1, 1);
  // SwitchLed(Leds::Red2, 1);
  // SwitchLed(Leds::Red3, 1);
  #if (DEBUG == 0)
  chase(strip.Color(255, 0, 0)); // Red
  #endif
  // SwitchLed(Leds::Blue1, 0);
  // SwitchLed(Leds::Blue2, 0);
  // SwitchLed(Leds::Blue3, 0);
  // SwitchLed(Leds::Red1, 0);
  // SwitchLed(Leds::Red2, 0);
  // SwitchLed(Leds::Red3, 0);
    #if (DEBUG == 0)

  chase(strip.Color(0, 255, 0)); // Green
    #endif

  // SwitchLed(Leds::Blue1, 1);
  // SwitchLed(Leds::Blue2, 1);
  // SwitchLed(Leds::Blue3, 1);
  // SwitchLed(Leds::Red1, 1);
  // SwitchLed(Leds::Red2, 1);
  // SwitchLed(Leds::Red3, 1);
    #if (DEBUG == 0)

  chase(strip.Color(0, 0, 255)); // Blue
    #endif

  // SwitchLed(Leds::Blue1, 0);
  // SwitchLed(Leds::Blue2, 0);
  // SwitchLed(Leds::Blue3, 0);
  // SwitchLed(Leds::Red1, 0);
  // SwitchLed(Leds::Red2, 0);
  // SwitchLed(Leds::Red3, 0);
  data++;
  // ReadButtons();
  // HandleButtons();
}

#if (DEBUG == 0)

void chase(uint32_t c) {
  for(uint16_t i=0; i<strip.numPixels()+4; i++) {
      strip.setPixelColor(i  , c); // Draw new pixel
      strip.setPixelColor(i-4, 0); // Erase pixel a few steps back
      strip.show();
      delay(30);
  }
}
#endif
/*
void SwitchLed(Leds led, uint8_t enabled){
    uint8_t column, row = 0;
    uint8_t inverted = 0;
    if(enabled == 0) {
      inverted = 1;
    }

    switch (led) {
    case Leds::Blue1:
      row = LED_ROW_1;
      column = LED_COL_1;
    break;
    case Leds::Blue2:
    row = LED_ROW_1;
      column = LED_COL_2;
    break;
    case Leds::Blue3:
    row = LED_ROW_1;
      column = LED_COL_3;
    break;
    case Leds::Red1:
    row = LED_ROW_2;
      column = LED_COL_1;
    break;
    case Leds::Red2:
    row = LED_ROW_2;
      column = LED_COL_2;
    break;
    case Leds::Red3:
    row = LED_ROW_2;
      column = LED_COL_3;
    break;
    default:
    break;
  }

  if(led == Leds::Toggles) {
    digitalWrite(TOGGLE_LEDS, enabled);

  } else {
    digitalWrite(row, inverted);
    digitalWrite(column, enabled);
  }


}
*/


// TODO(ph) Button matrix failed....
/*
void ReadButtons() {
  if (digitalRead(BUTTON_ROW_1) == 0) {
    if(digitalRead(BUTTON_COL_1) == 0) {
      buttonStates.toggle1 = true;
      #if (DEBUG == 1)
      Serial.println("Blue1 pressed");
      #endif
    } else{ buttonStates.toggle1 = false;
          #if (DEBUG == 1)

      Serial.println("Blue1 released");
      #endif

}
    if (digitalRead(BUTTON_COL_2) == 0) {
      buttonStates.toggle2 = true;
    } else{ buttonStates.toggle2 = false; }
  } if (digitalRead(BUTTON_ROW_2) == 0) {
    if(digitalRead(BUTTON_COL_1) == 0) {
      buttonStates.blue1 = true;
    } else{ buttonStates.blue1 = false; }
    if (digitalRead(BUTTON_COL_2) == 0) {
      buttonStates.blue2 = true;
    } else{ buttonStates.blue2 = false; }
    if (digitalRead(BUTTON_COL_3) == 0) {
      buttonStates.blue3 = true;
    } else{ buttonStates.blue3 = false; }
  } if (digitalRead(BUTTON_ROW_3) == 0) {
    if(digitalRead(BUTTON_COL_1) == 0) {
      buttonStates.red1 = true;
    } else{ buttonStates.red1 = false; }
    if (digitalRead(BUTTON_COL_2) == 0) {
      buttonStates.red2 = true;
    } else{ buttonStates.red2 = false; }
    if (digitalRead(BUTTON_COL_3) == 0) {
      buttonStates.red3 = true;
    } else{ buttonStates.red3 = false; }
  }
  if (digitalRead(ARCADE_N) == 0) {
    buttonStates.up = true;
  } else {buttonStates.up = false;}
  if (digitalRead(ARCADE_S) == 0) {
    buttonStates.down = true;
  } else {buttonStates.down = false;}
  if (digitalRead(ARCADE_W) == 0) {
    buttonStates.left = true;
  } else {buttonStates.left = false;}
  if (digitalRead(ARCADE_E) == 0) {
    buttonStates.right = true;
  } else {buttonStates.right = false;}
}

void HandleButtons() {
  if(buttonStates.blue1) {
    SwitchLed(Leds::Blue1, 1);
  } else { SwitchLed(Leds::Blue1, 0); }
  if(buttonStates.blue2) {
    SwitchLed(Leds::Blue2, 1);
  } else { SwitchLed(Leds::Blue2, 0); }
  if(buttonStates.blue3) {
    SwitchLed(Leds::Blue3, 1);
  } else { SwitchLed(Leds::Blue3, 0); }
  if(buttonStates.red1) {
    SwitchLed(Leds::Red1, 1);
  } else { SwitchLed(Leds::Red1, 0); }
  if(buttonStates.red2) {
    SwitchLed(Leds::Red2, 1);
  } else { SwitchLed(Leds::Red2, 0); }

}
*/