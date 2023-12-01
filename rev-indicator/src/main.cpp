#include <Arduino.h>
#include <CAN.h>
#include <OBD2.h>
#include <FastLED.h>
#include "timer.h"
#include "config.h"

#define LED_STRIP 7
#define NUM_LEDS 25
// array of PID's to print values of
const int PIDS[] = {
    ENGINE_COOLANT_TEMPERATURE,
    ENGINE_RPM,
    VEHICLE_SPEED,
};

utils::timer rev_timer(50);

const int NUM_PIDS = sizeof(PIDS) / sizeof(PIDS[0]);

CRGB leds[NUM_LEDS];

void set_leds(CRGB *leds, uint8_t count, CRGB::HTMLColorCode color);

void setup()
{
  Serial.begin(9600);
  while (!Serial)
    ;

  Serial.println(F("OBD2 Key Stats"));

  while (true)
  {
    Serial.print(F("Attempting to connect to OBD2 CAN bus ... "));

    if (!OBD2.begin())
    {
      Serial.println(F("failed!"));

      delay(500);
    }
    else
    {
      Serial.println(F("success"));
      break;
    }
  }

  rev_timer.start();

  FastLED.addLeds<WS2812B, LED_STRIP, GRB>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(50);

  Serial.println();
}

void printPID(int pid);

void loop()
{
  float pidValue = OBD2.pidRead(ENGINE_RPM);

  uint32_t reading = static_cast<uint32_t>(pidValue);
  uint8_t mapped = map(reading, MIN_INPUT_VALUE, MAX_INPUT_VALUE, 0, MAX_OUTPUT_VALUE);

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
    else if (mapped <= MIN_INPUT_VALUE)
    {
      set_leds(leds + REV_LEDS, SHIFT_LEDS, CRGB::Black);
    }
  }

  FastLED.show();
}

// Enables rev leds
void set_leds(CRGB *leds, uint8_t count, CRGB::HTMLColorCode color)
{
  for (uint8_t i = 0; i < count; ++i)
    leds[i] = color;
}

void printPID(int pid)
{
  // print PID name
  Serial.print(OBD2.pidName(pid));
  Serial.print(F(" = "));

  // read the PID value
  float pidValue = OBD2.pidRead(pid);

  if (isnan(pidValue))
  {
    Serial.print("error");
  }
  else
  {
    // print value with units
    Serial.print(pidValue);
    Serial.print(F(" "));
    Serial.print(OBD2.pidUnits(pid));
  }

  Serial.println();
}