#include <Adafruit_GFX.h>
#include <Arduino.h>
#include <Wire.h>

#include "Adafruit_LEDBackpack.h"
#include "Adafruit_NeoPixel.h"
#include "pitches.h"

#define ARCADE_N (34)
#define ARCADE_S (35)
#define ARCADE_W (33)
#define ARCADE_E (32)

#define SPEAKER (10)

#define NEOPIXEL_RING (22)
#define N_LEDS (16)

#define LED_RED_1 (5)
#define LED_RED_2 (6)
#define LED_RED_3 (7)
#define LED_BLUE_1 (2)
#define LED_BLUE_2 (3)
#define LED_BLUE_3 (4)

#define BUTTON_TOGGLE_1 (23)
#define BUTTON_TOGGLE_2 (17)
#define BUTTON_RED_1 (28)
#define BUTTON_RED_2 (29)
#define BUTTON_RED_3 (30)
#define BUTTON_BLUE_1 (25)
#define BUTTON_BLUE_2 (26)
#define BUTTON_BLUE_3 (27)

#define DEBUG 1

#define TXD3 (14)
#define RXD3 (15)

// Train control
const uint8_t TRAIN_FORWARD = 1u;    // U
const uint8_t TRAIN_BACKWARD = 2u;   // D
const uint8_t TRAIN_LIGHT = 3u;      // R1
const uint8_t TRAIN_BRAKE = 4u;      // R2
const uint8_t TRAIN_REFILL = 5u;     // B1
const uint8_t TRAIN_HORN = 6u;       // B2
const uint8_t TRAIN_DEPARTURE = 7u;  // R3
const uint8_t TRAIN_STEAM = 8u;      // B3
const uint8_t TRAIN_NOPE = 0u;

void chase(uint32_t c);

enum class Leds { Blue1, Blue2, Blue3, Red1, Red2, Red3, Toggles };

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
} ButtonArrray_t;

ButtonArrray_t buttonStates;

void SwitchLed(Leds led, uint8_t enabled);
void ReadButtons();
void IncreaseCount(void);
void HandleStick(void);
void SendEsp32(uint8_t cmd);

Adafruit_NeoPixel strip =
    Adafruit_NeoPixel(N_LEDS, NEOPIXEL_RING, NEO_GRB + NEO_KHZ800);
Adafruit_7segment matrix = Adafruit_7segment();

void setup() {
  Serial.begin(115200);  // Serial uses TX and RX Pins 0 + 1

  Serial3.begin(115200);

  matrix.begin(0x70);      // Init I2C Display
  strip.begin();           // Init LED Strip
  strip.setBrightness(1);  // lower brightness for toddlers
  strip.show();            // Initialize all pixels to 'off'

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
  delay(200);
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
  matrix.print(0000);
  matrix.writeDisplay();
  Serial.println("ButtonBox Initialized\r\n");
}

void loop() {
  ReadButtons();
  HandleStick();
}

void ReadButtons() {
  if (digitalRead(ARCADE_N) == 0) {
    if (!buttonStates.up) {
      IncreaseCount();
      SendEsp32(TRAIN_FORWARD);
      Serial.println("Up pressed");
    }
    buttonStates.up = true;
  } else {
    buttonStates.up = false;
  }
  if (digitalRead(ARCADE_S) == 0) {
    if (!buttonStates.down) {
      IncreaseCount();
      SendEsp32(TRAIN_BACKWARD);
      Serial.println("Down pressed");
    }
    buttonStates.down = true;
  } else {
    buttonStates.down = false;
  }
  if (digitalRead(ARCADE_W) == 0) {
    if (!buttonStates.left) {
      IncreaseCount();
      Serial.println("Left pressed");
    }
    buttonStates.left = true;
  } else {
    buttonStates.left = false;
  }
  if (digitalRead(ARCADE_E) == 0) {
    if (!buttonStates.right) {
      IncreaseCount();
      Serial.println("Right pressed");
    }
    buttonStates.right = true;
    delay(20);
  } else {
    buttonStates.right = false;
  }
  if (digitalRead(BUTTON_RED_1) == 0) {
    if (!buttonStates.red1) {
      IncreaseCount();
      SendEsp32(TRAIN_LIGHT);
      Serial.println("Red1 pressed");
    }
    buttonStates.red1 = true;
    digitalWrite(LED_RED_1, HIGH);
  } else {
    buttonStates.red1 = false;
    digitalWrite(LED_RED_1, LOW);
  }
  if (digitalRead(BUTTON_RED_2) == 0) {
    if (!buttonStates.red2) {
      IncreaseCount();
      SendEsp32(TRAIN_BRAKE);
      Serial.println("Red2 pressed");
    }
    buttonStates.red2 = true;
    digitalWrite(LED_RED_2, HIGH);
  } else {
    buttonStates.red2 = false;
    digitalWrite(LED_RED_2, LOW);
  }
  if (digitalRead(BUTTON_RED_3) == 0) {
    if (!buttonStates.red3) {
      IncreaseCount();
      SendEsp32(TRAIN_DEPARTURE);
      Serial.println("Red3 pressed");
    }
    buttonStates.red3 = true;
    digitalWrite(LED_RED_3, HIGH);
  } else {
    buttonStates.red3 = false;
    digitalWrite(LED_RED_3, LOW);
  }
  if (digitalRead(BUTTON_BLUE_1) == 0) {
    if (!buttonStates.blue1) {
      IncreaseCount();
      SendEsp32(TRAIN_REFILL);
      Serial.println("Blue1 pressed");
    }
    buttonStates.blue1 = true;
    digitalWrite(LED_BLUE_1, HIGH);
  } else {
    buttonStates.blue1 = false;
    digitalWrite(LED_BLUE_1, LOW);
  }
  if (digitalRead(BUTTON_BLUE_2) == 0) {
    if (!buttonStates.blue2) {
      IncreaseCount();
      SendEsp32(TRAIN_HORN);
      Serial.println("Blue2 pressed");
    }
    buttonStates.blue2 = true;
    digitalWrite(LED_BLUE_2, HIGH);
  } else {
    buttonStates.blue2 = false;
    digitalWrite(LED_BLUE_2, LOW);
  }
  if (digitalRead(BUTTON_BLUE_3) == 0) {
    if (!buttonStates.blue3) {
      IncreaseCount();
      SendEsp32(TRAIN_STEAM);
      Serial.println("Blue3 pressed");
    }
    buttonStates.blue3 = true;
    digitalWrite(LED_BLUE_3, HIGH);
  } else {
    buttonStates.blue3 = false;
    digitalWrite(LED_BLUE_3, LOW);
  }
}

void IncreaseCount(void) {
  static uint16_t count = 0u;
  count++;
  if (count >= 9999) {
    count = 0u;
  }
  matrix.print(count, DEC);
  matrix.writeDisplay();
}

void SendEsp32(uint8_t cmd) {
  Serial3.println(cmd);
  delay(2);
}

void HandleStick(void) {
  if (buttonStates.up) {
    strip.fill(strip.Color(0, 255, 0), 0, N_LEDS);
    strip.show();
  } else if (buttonStates.down) {
    strip.fill(strip.Color(255, 0, 0), 0, N_LEDS);
    strip.show();
  } else if (buttonStates.left) {
    strip.fill(strip.Color(0, 0, 255), 0, N_LEDS);
    strip.show();
  } else if (buttonStates.right) {
    strip.fill(strip.Color(150, 150, 150), 0, N_LEDS);
    strip.show();
  }
}

// TODO(ph) Add Uart receeiv method
// TODO(ph) Handle received Train Speed

void chase(uint32_t c) {
  for (uint16_t i = 0; i < strip.numPixels() + 4; i++) {
    strip.setPixelColor(i, c);      // Draw new pixel
    strip.setPixelColor(i - 4, 0);  // Erase pixel a few steps back
    strip.show();
    delay(10);
  }
}
