#include <Arduino.h>
#include "led.h"
#include "timer.h"
#include "pin.cpp"
#include <FastLED.h>

enum leds_e : uint8_t
{
  GREEN = 0,
  YELLOW = 9,
  RED = 16
};

const uint8_t GREEN_LED_COUNT = 9;
const uint8_t YELLOW_LED_COUNT = 7;
const uint8_t RED_LED_COUNT = 6;
const uint8_t SHIFT_LED_COUNT = 3;

// Timer durations
utils::timer rev_timer(50);
utils::timer shift_timer(100);

void toggle_leds(CRGB *leds, uint8_t count, CRGB::HTMLColorCode color);
void set_leds(CRGB *leds, uint8_t count, CRGB::HTMLColorCode color);

CRGB leds[NUM_LEDS];

void setup()
{
  Serial.begin(9600);
  delay(1000);

  Serial.println("Program Start");
  rev_timer.start();

  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(100);
}

void loop()
{
  uint16_t reading = analogRead(POT_PIN_REV);
  uint8_t mapped = map(reading, 0, MAX_INPUT_VALUE, 0, MAX_OUTPUT_VALUE);

  if (rev_timer.check())
  {
    set_leds(leds, REV_LEDS, CRGB::Black);

    for (int i = 0; i < min(mapped, REV_LEDS); i++)
    {
      uint8_t green = map(i, 0, REV_LEDS - 1, 255, 0);
      uint8_t red = map(i, 0, REV_LEDS - 1, 0, 255);

      leds[i] = CRGB(red, green, 0);
    }

    if (mapped > REV_LEDS)
    {
      if (millis() % 200 < 100)
      {
        set_leds(leds + REV_LEDS, SHIFT_LEDS, CRGB::Blue);
      }
      else
      {
        set_leds(leds + REV_LEDS, SHIFT_LEDS, CRGB::Black);
      }
    }
    else if (reading <= 1000)
    {
      set_leds(leds + REV_LEDS, SHIFT_LEDS, CRGB::Black);
    }
  }

  FastLED.show();
}

// Enables blinking blue shift led
void toggle_leds(CRGB *leds, uint8_t count, CRGB::HTMLColorCode color)
{
  for (uint8_t i = 0; i < count; ++i)
    leds[i] = color;
}

// Enables rev leds
void set_leds(CRGB *leds, uint8_t count, CRGB::HTMLColorCode color)
{
  for (uint8_t i = 0; i < count; ++i)
    leds[i] = color;
}
