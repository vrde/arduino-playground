#include "FastLED.h"

// LED strip settings
#define DATA_PIN 11
#define CLOCK_PIN 13

const int DOTS = 5;
const float GRAVITY = 0.098;
const float FRICTION = 0.01;
const int NUM_LEDS = 60;

typedef struct tdot {
  float r; // radian
  float w; // angular velocity
  CRGB c;  // color
} Dot;

Dot dots[] = {
  PI * 3 / 4, 5, CRGB::Green,
  PI / 2, -3, CRGB::Red,
  PI, -3, CRGB::Blue,
  PI, 2, CRGB::Orange,
  PI, 10, CRGB::Purple
};

CRGB leds[NUM_LEDS];


void setup() {
  FastLED.addLeds<APA102, DATA_PIN, CLOCK_PIN, BGR>(leds, NUM_LEDS);
  Serial.begin(115200);
  randomSeed(analogRead(0));
}

void next(float dt, Dot *dot) {
  float g = cos(dot->r) * -GRAVITY;
  dot->w += g;

  if (dot->w > 0) dot->w -= FRICTION;
  else dot->w += FRICTION;

  dot->r += dot->w / dt;
  while (dot->r > 2 * PI) dot->r -= 2 * PI;
  while (dot->r < 0) dot->r = 2 * PI + dot->r;
}

void render(Dot *dot) {
  int n = (NUM_LEDS - 1) * (dot->r / (2 * PI));
  leds[n] = dot->c;

}

void loop() {
  FastLED.clear();

  for (int i = 0; i < DOTS; i++) {
    Dot *dot = &dots[i];
    render(dot);
    next(25, dot);
  }

  FastLED.show();
  delay(25);
}

