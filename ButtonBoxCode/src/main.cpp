#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"
#include "Adafruit_NeoPixel.h"

#define ARCADE_N (14)
#define ARCADE_S (15)
#define ARCADE_W (16)
#define ARCADE_E (17)

#define SPEAKER (0)        // Disconnect while uploading
#define NEOPIXEL_RING (1)  
#define LED_COL_1 (2)
#define LED_COL_2 (3)
#define LED_COL_3 (4)
#define LED_ROW_1 (5)
#define LED_ROW_2 (6)

#define BUTTON_COL_1 (7)
#define BUTTON_COL_2 (8)
#define BUTTON_COL_3 (9)
#define BUTTON_ROW_1 (10)
#define BUTTON_ROW_2 (11)
#define BUTTON_ROW_3 (12)
#define TOGGLE_LEDS (13)
#define N_LEDS (16)

#define DEBUG (0)

void chase(uint32_t c);

enum class Leds {
    Blue1,
    Blue2,
    Blue3,
    Red1,
    Red2,
    Red3,
    Toggles
};

struct ButtonArray {
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
};

void SwitchLed(Leds led, uint8_t enabled);
void HandleButtons();

Adafruit_NeoPixel strip = Adafruit_NeoPixel(N_LEDS, NEOPIXEL_RING, NEO_GRB + NEO_KHZ800);
Adafruit_7segment matrix = Adafruit_7segment();

void setup() {
#if (DEBUG == 1)
  Serial.begin(9600);             // Serial uses TX and RX Pins 0 + 1
  Serial.println("ButtonBox TestCode\r\n");
#endif

  matrix.begin(0x70); // Init I2C Display
  strip.begin();      // Init LED Strip
  strip.setBrightness(1);    // lower brightness for toddlers
  strip.show(); // Initialize all pixels to 'off'


  // Init Arcade
  pinMode(ARCADE_N, INPUT_PULLUP);
  pinMode(ARCADE_S, INPUT_PULLUP);
  pinMode(ARCADE_W, INPUT_PULLUP);
  pinMode(ARCADE_E, INPUT_PULLUP);
  
  // Init LEDs to off
  pinMode(LED_ROW_1, OUTPUT);
  pinMode(LED_ROW_2, OUTPUT);
  pinMode(LED_COL_1, OUTPUT);   // TODO verify, might break something watch this.
  pinMode(LED_COL_2, OUTPUT);   
  pinMode(LED_COL_3, OUTPUT);   
  
  // TODO(Add led control functions and defines for LED 1-6)
  // ALL LEDS OFF
  digitalWrite(LED_ROW_1, HIGH);
  digitalWrite(LED_ROW_2, HIGH);
  digitalWrite(LED_COL_1, LOW);
  digitalWrite(LED_COL_2, LOW);
  digitalWrite(LED_COL_3, LOW);
  delay(1000);

  SwitchLed(Leds::Blue1, 1);
  delay(1000);
  SwitchLed(Leds::Blue1, 0);
  delay(1000);
  SwitchLed(Leds::Blue2, 1);
  delay(1000);
  SwitchLed(Leds::Blue2, 0);
  delay(1000);
  SwitchLed(Leds::Blue3, 1);
  delay(1000);
  SwitchLed(Leds::Blue3, 0);
  delay(1000);
  SwitchLed(Leds::Red1, 1);
  delay(1000);
  SwitchLed(Leds::Red1, 0);
  delay(1000);
  SwitchLed(Leds::Red2, 1);
  delay(1000);
  SwitchLed(Leds::Red2, 0);
  delay(1000);
  SwitchLed(Leds::Red3, 1);
  delay(1000);
  SwitchLed(Leds::Red3, 0);
  delay(1000);
}


void loop() {
  static uint16_t data = 0;  
  
  
  matrix.print(data, DEC);
  matrix.writeDisplay();

  chase(strip.Color(255, 0, 0)); // Red
  chase(strip.Color(0, 255, 0)); // Green
  chase(strip.Color(0, 0, 255)); // Blue

  data++;
}


void chase(uint32_t c) {
  for(uint16_t i=0; i<strip.numPixels()+4; i++) {
      strip.setPixelColor(i  , c); // Draw new pixel
      strip.setPixelColor(i-4, 0); // Erase pixel a few steps back
      strip.show();
      delay(30);
  }
}

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

void HandleButtons() {
  // digitalRead()

}