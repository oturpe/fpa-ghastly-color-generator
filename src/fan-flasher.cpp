// Fog mover tester

#include "Attiny45Utils.h"

#include "config.h"

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "lib/DmxSimple/DmxSimple.h"

// \brief
///    Turns th indicator led on of off
///
/// \param lit
///    If led is turned on. Otherwise it is turned off.
inline void setIndicator(bool lit) {
    if (lit) {
        PORTB |= BV(PORTB0);
    }
    else {
        PORTB &= ~BV(PORTB0);
    }
}


bool magnetFound = false;

ISR(INT0_vect,ISR_NOBLOCK) {
    if (magnetFound) {
        // External interrupt on falling edge.
        MCUCR |= BV(ISC01);
        MCUCR &= ~BV(ISC00);
        DmxSimple.write(1,0x00);
    }
    else {
        // External interrupt on rising edge.
        MCUCR |= BV(ISC01) | BV(ISC00);
        DmxSimple.write(1,0xff);
    }

    magnetFound = !magnetFound;
}




#define COLOR_SEQUENCE_LENGTH 25
uint8_t colorSequence[COLOR_SEQUENCE_LENGTH][3] = {
    {0xff, 0x00, 0x00},
    {0xf0, 0x00, 0x10},
    {0xe0, 0x00, 0x20},
    {0xd0, 0x00, 0x30},
    {0xc0, 0x00, 0x40},
    {0xb0, 0x00, 0x50},
    {0xa0, 0x00, 0x60},
    {0x90, 0x00, 0x70},
    {0x80, 0x00, 0x80},
    {0x70, 0x00, 0x90},
    {0x60, 0x00, 0xa0},
    {0x50, 0x00, 0xb0},
    {0x40, 0x00, 0xc0},
    {0x40, 0x00, 0xc0},
    {0x50, 0x00, 0xb0},
    {0x60, 0x00, 0xa0},
    {0x70, 0x00, 0x90},
    {0x80, 0x00, 0x80},
    {0x90, 0x00, 0x70},
    {0xa0, 0x00, 0x60},
    {0xb0, 0x00, 0x50},
    {0xc0, 0x00, 0x40},
    {0xd0, 0x00, 0x30},
    {0xe0, 0x00, 0x20},
    {0xf0, 0x00, 0x10},
};

void setColor(uint8_t * rbg) {
    DmxSimple.write(2, rbg[0]);
    DmxSimple.write(3, rbg[2]);
    DmxSimple.write(4, rbg[1]);
}

int main() {

    // Set output pins
    //    B0 (indicator)
    //    dmx output is set by library
    DDRB |= BV(DDB0);

    // Enable external interrupt
    //GIMSK |= BV(INT0);

    // Initialize dmx
    Attiny45::setTimer0Prescaler(DMX_PRESCALER);
    DmxSimple.maxChannel(DMX_CHANNELS);
    sei();

    // Set color balance: all full off initially
    DmxSimple.write(2, 0x00);
    DmxSimple.write(3, 0x00);
    DmxSimple.write(4, 0x00);
    DmxSimple.write(5, 0x00);

    // Master brightness full always
    DmxSimple.write(1, 0xff);

    // Set auto/sound mode and stroboscope off
    DmxSimple.write(6, 0x00);
    DmxSimple.write(7, 0x00);

    _delay_ms(500);

    bool indicatorLit = false;
    uint64_t counter = 0;
    uint16_t colorCounter = 0;

    while(true) {
        counter += 1;
        _delay_ms(LOOP_DELAY);

        if(counter % INDICATOR_HALF_PERIOD == 0) {
            indicatorLit = !indicatorLit;
            setIndicator(indicatorLit);
        }

        if(counter % COLOR_PERIOD == 0) {
            setColor(colorSequence[colorCounter % COLOR_SEQUENCE_LENGTH]);
            colorCounter++;
        }
    }
}
