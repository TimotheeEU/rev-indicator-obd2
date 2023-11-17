#ifndef REV_INDICATOR_LED_H
#define REV_INDICATOR_LED_H

#include <stdint.h>

class led
{
    public:
        led(uint8_t pin);
        ~led() = default;
    public:
        void on();
        void off();
        void toggle();
    private:
        uint8_t m_pin;
        uint8_t m_state;
};

#endif // REV_INDICATOR_LED_H