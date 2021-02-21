#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"
#include "Adafruit_NeoPixel.h"

#define ARCADE_N (34)
#define ARCADE_S (35)
#define ARCADE_W (33)
#define ARCADE_E (32)

#define SPEAKER (10)

#define NEOPIXEL_RING (22)
#define N_LEDS (16)

#define LED_RED_1 (2)
#define LED_RED_2 (3)
#define LED_RED_3 (4)
#define LED_BLUE_1 (5)
#define LED_BLUE_2 (6)
#define LED_BLUE_3 (7)

#define BUTTON_TOGGLE_1 (23)
#define BUTTON_TOGGLE_2 (17)
#define BUTTON_RED_1 (28)
#define BUTTON_RED_2 (29)
#define BUTTON_RED_3 (30)
#define BUTTON_BLUE_1 (25)
#define BUTTON_BLUE_2 (26)
#define BUTTON_BLUE_3 (27)

#define NRF_IRQ (11)
#define NRF_MOSI (50)
#define NRF_MISO (51)
#define NRF_CS   (53)
#define NRF_SCK  (52)
#define NRF_CE (49)

#define DEBUG 1

#define TXD3 (14)
#define RXD3 (15)

// Train control
const char TRAIN_FORWARD = "F";     // U
const char TRAIN_BACKWARD = "B";    // D
const char TRAIN_STOP = "S";        // R1
const char TRAIN_BRAKE = "1";       // R2
const char TRAIN_REFILL = "2";      // B1
const char TRAIN_HORN = "3";        // B2
const char TRAIN_DEPARTURE = "4";   // R3
const char TRAIN_STEAM = "5";       // B3
const char TRAIN_LIGHT = "6";       // L

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
void IncreaseCount(void);
void SendEsp32(char cmd);

Adafruit_NeoPixel strip = Adafruit_NeoPixel(N_LEDS, NEOPIXEL_RING, NEO_GRB + NEO_KHZ800);
Adafruit_7segment matrix = Adafruit_7segment();


void setup() {
  Serial.begin(115200);             // Serial uses TX and RX Pins 0 + 1
  Serial3.begin(115200);
  Serial.println("ButtonBox TestCode\r\n");
  matrix.begin(0x70);   // Init I2C Display
  strip.begin();        // Init LED Strip
  strip.setBrightness(1);    // lower brightness for toddlers
  strip.show();   // Initialize all pixels to 'off'

  // Init Arcade
  pinMode(ARCADE_N, INPUT_PULLUP);
  pinMode(ARCADE_S, INPUT_PULLUP);
  pinMode(ARCADE_W, INPUT_PULLUP);
  pinMode(ARCADE_E, INPUT_PULLUP);

  // Init Buttons and Toggle
  pinMode(BUTTON_RED_1, INPUT_PULLUP);
  pinMode(BUTTON_RED_2, INPUT_PULLUP);
  pinMode(BUTTON_RED_3, INPUT_PULLUP);
  pinMode(BUTTON_BLUE_1, INPUT_PULLUP);
  pinMode(BUTTON_BLUE_2, INPUT_PULLUP);
  pinMode(BUTTON_BLUE_3, INPUT_PULLUP);
  pinMode(BUTTON_TOGGLE_1, OUTPUT);
  pinMode(BUTTON_TOGGLE_2, OUTPUT);

  // Init LEDs to output
  pinMode(LED_RED_1, OUTPUT);
  pinMode(LED_RED_2, OUTPUT);
  pinMode(LED_RED_3, OUTPUT);
  pinMode(LED_BLUE_1, OUTPUT);
  pinMode(LED_BLUE_2, OUTPUT);
  pinMode(LED_BLUE_3, OUTPUT);


  digitalWrite(LED_RED_1, HIGH);
  digitalWrite(LED_RED_2, HIGH);
  digitalWrite(LED_RED_3, HIGH);
  digitalWrite(LED_BLUE_1, HIGH);
  digitalWrite(LED_BLUE_2, HIGH);
  digitalWrite(LED_BLUE_3, HIGH);
  delay(1000);
  // ALL LEDS OFF
  digitalWrite(LED_RED_1, LOW);
  digitalWrite(LED_RED_3, LOW);
  digitalWrite(LED_RED_2, LOW);
  digitalWrite(LED_BLUE_1, LOW);
  digitalWrite(LED_BLUE_2, LOW);
  digitalWrite(LED_BLUE_3, LOW);
  digitalWrite(BUTTON_TOGGLE_1, HIGH);
  digitalWrite(BUTTON_TOGGLE_2, HIGH);

  buttonStates.toggle1 = false;
  buttonStates.toggle2 = false;
  buttonStates.blue1 = false;
  buttonStates.blue2 = false;
  buttonStates.blue3 = false;
  buttonStates.red1 = false;
  buttonStates.red2 = false;
  buttonStates.red3 = false;
  buttonStates.up = false;
  buttonStates.down = false;
  buttonStates.left = false;
  buttonStates.right = false;
}


void loop() {
  static uint16_t data = 0;

  matrix.print(data, DEC);
  matrix.writeDisplay();

  //chase(strip.Color(255, 0, 0));
  //chase(strip.Color(0, 255, 0));
  //chase(strip.Color(0, 0, 255));

  data++;
  ReadButtons();
}


void chase(uint32_t c) {
  for(uint16_t i=0; i<strip.numPixels()+4; i++) {
      strip.setPixelColor(i  , c); // Draw new pixel
      strip.setPixelColor(i-4, 0); // Erase pixel a few steps back
      strip.show();
      delay(30);
  }
}

void ReadButtons() {
  if (digitalRead(ARCADE_N) == 0) {
    if(!buttonStates.up) {
      IncreaseCount();
    }
    Serial.println("Up pressed");
    buttonStates.up = true;
    SendEsp32(TRAIN_FORWARD);
  } else {buttonStates.up = false;}
  if (digitalRead(ARCADE_S) == 0) {
    if(!buttonStates.down) {
      IncreaseCount();
    }
    Serial.println("Down pressed");
    buttonStates.down = true;
    SendEsp32(TRAIN_BACKWARD);
  } else {buttonStates.down = false;}
  if (digitalRead(ARCADE_W) == 0) {
    if(!buttonStates.left) {
      IncreaseCount();
    }
    Serial.println("Left pressed");
    buttonStates.left = true;
    SendEsp32(TRAIN_DEPARTURE);
  } else {buttonStates.left = false;}
  if (digitalRead(ARCADE_E) == 0) {
    if(!buttonStates.right) {
      IncreaseCount();
    }
    Serial.println("Right pressed");
    buttonStates.right = true;
  } else {buttonStates.right = false;}
  if (digitalRead(BUTTON_RED_1) == 0) {
    Serial.println("Red1 pressed");
    if(!buttonStates.red1) {
      IncreaseCount();
    }
    buttonStates.red1 = true;
    digitalWrite(LED_RED_1, HIGH);
    SendEsp32(TRAIN_STOP);
  } else {
    buttonStates.red1 = false;
    digitalWrite(LED_RED_1, LOW);
  }
  if (digitalRead(BUTTON_RED_2) == 0) {
    Serial.println("Red2 pressed");
    if(!buttonStates.red2) {
      IncreaseCount();
    }
    buttonStates.red2 = true;
    digitalWrite(LED_RED_2, HIGH);
    SendEsp32(TRAIN_BRAKE);
  } else {
    buttonStates.red2 = false;
    digitalWrite(LED_RED_2, LOW);
  }
  if (digitalRead(BUTTON_RED_3) == 0) {
    if(!buttonStates.red3) {
      IncreaseCount();
    }
    Serial.println("Red3 pressed");
    buttonStates.red3 = true;
    digitalWrite(LED_RED_3, HIGH);
    SendEsp32(TRAIN_LIGHT);
  } else {
    buttonStates.red3 = false;
    digitalWrite(LED_RED_3, LOW);
  }
  if (digitalRead(BUTTON_BLUE_1) == 0) {
    if(!buttonStates.blue1) {
      IncreaseCount();
    }
    Serial.println("Blue1 pressed");
    buttonStates.blue1 = true;
    digitalWrite(LED_BLUE_1, HIGH);
    SendEsp32(TRAIN_REFILL);
  } else {
    buttonStates.blue1 = false;
    digitalWrite(LED_BLUE_1, LOW);
  }
  if (digitalRead(BUTTON_BLUE_2) == 0) {
    if(!buttonStates.blue2) {
      IncreaseCount();
    }
    Serial.println("Blue2 pressed");
    buttonStates.blue2 = true;
    digitalWrite(LED_BLUE_2, HIGH);
    SendEsp32(TRAIN_HORN);
  } else {
    buttonStates.blue2 = false;
    digitalWrite(LED_BLUE_2, LOW);
    }
  if (digitalRead(BUTTON_BLUE_3) == 0) {
    if(!buttonStates.blue3) {
      IncreaseCount();
    }
    Serial.println("Blue3 pressed");
    buttonStates.blue3 = true;
    digitalWrite(LED_BLUE_3, HIGH);
    SendEsp32(TRAIN_STEAM);
  } else {
    buttonStates.blue3 = false;
    digitalWrite(LED_BLUE_3, LOW);
  }
}

void IncreaseCount(void) {
  static uint16_t count = 0u;

  count++;
  if(count >= 9999) {
    count = 0u;
  }
  matrix.print(count, DEC);
  matrix.writeDisplay();
}

void SendEsp32(char cmd) {
  Serial3.write(cmd);
}
