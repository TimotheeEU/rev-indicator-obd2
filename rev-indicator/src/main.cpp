#include <Arduino.h>
#include "led.h"
#include "timer.h"
#include "pin.cpp"
#include <FastLED.h>

enum leds_e : uint8_t
{
  GREEN = 0,
  YELLOW = 4,
  RED = 8
};

const uint8_t REV_LED_COUNT = 10;
const uint8_t GREEN_LED_COUNT = 4;
const uint8_t YELLOW_LED_COUNT = 3;
const uint8_t RED_LED_COUNT = 3;

led rev_led[] = {
  led(LED_GREEN1),
  led(LED_GREEN2),
  led(LED_GREEN3),
  led(LED_GREEN4),
  led(LED_YELLOW1),
  led(LED_YELLOW2),
  led(LED_YELLOW3),
  led(LED_RED1),
  led(LED_RED2),
  led(LED_RED3),
};

const uint8_t SHIFT_LED_COUNT = 2;

led shift_leds[] = {
  led(LED_BLUE1),
  led(LED_BLUE2),
};

// Timer durations
utils::timer rev_timer(50);
utils::timer shift_timer(100);

void toggle_leds(led *leds, uint8_t count);
void set_leds(led *leds, uint8_t count, bool on);

CRGB leds[NUM_LEDS];

void setup()
{
  Serial.begin(9600);
  delay(1000);

  Serial.println("Program Start");
  rev_timer.start();

  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(30);
}

void loop()
{
  uint16_t reading = analogRead(POT_PIN_REV);
  uint8_t mapped = map(reading, 0, MAX_INPUT_VALUE, 0, MAX_OUTPUT_VALUE);

  if (rev_timer.check())
  {
    set_leds(rev_led, REV_LED_COUNT, false);
    set_leds(rev_led, mapped, true);
    // fill_solid(leds, mapped, CRGB::Green);
    FastLED.show();
  }

  if (shift_timer.check())
  {
    if (reading > 1000)
    {
      toggle_leds(shift_leds, SHIFT_LED_COUNT);
      FastLED.show();
    }
    else
    {
      set_leds(shift_leds, SHIFT_LED_COUNT, false);
      FastLED.show();
    }
  }
}

// Enables blinking blue shift led
void toggle_leds(led *leds, uint8_t count)
{
  for (uint8_t i = 0; i < count; ++i)
  {
    leds[i].toggle();
  }
}

// Enables rev leds
void set_leds(led *leds, uint8_t count, bool on)
{
  for (uint8_t i = 0; i < count; ++i)
  {
    if (on)
    {
      leds[i].on();
    }
    else
    {
      leds[i].off();
    }
  }
}