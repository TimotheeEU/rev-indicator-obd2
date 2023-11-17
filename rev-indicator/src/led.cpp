#include "led.h"
#include <Arduino.h>

led::led(uint8_t pin)
    : m_pin(pin),
      m_state(0)
{
    pinMode(m_pin, OUTPUT);
}
void led::on()
{
    m_state = HIGH;
    digitalWrite(m_pin, m_state);
}
void led::off()
{
    m_state = LOW;
    digitalWrite(m_pin, m_state);
}
void led::toggle()
{
    m_state = (m_state == LOW) ? HIGH : LOW;
    digitalWrite(m_pin, m_state);
}