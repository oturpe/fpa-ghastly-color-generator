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

int main() {

    // Set output pins
    //    B0 (indicator)
    //    dmx output is set by library
    DDRB |= BV(DDB0);

    // Enable external interrupt
    GIMSK |= BV(INT0);

    // Initialize dmx
    Attiny45::setTimer0Prescaler(DMX_PRESCALER);
    DmxSimple.maxChannel(DMX_CHANNELS);
    sei();

    // Set color balance: all full on (to be modulated with master dimming)
    DmxSimple.write(2, 0xff);
    DmxSimple.write(3, 0xff);
    DmxSimple.write(4, 0xff);
    DmxSimple.write(5, 0xff);

    // Start with light off
    DmxSimple.write(1, 0x00);

    // Set auto/sound mode and stroboscope off
    DmxSimple.write(6, 0x00);
    DmxSimple.write(7, 0x00);

    _delay_ms(500);

    bool indicatorLit = false;
    uint64_t counter = 0;

    while(true) {
        counter += 1;
        _delay_ms(LOOP_DELAY);

        if(counter % INDICATOR_HALF_PERIOD == 0) {
            indicatorLit = !indicatorLit;
            setIndicator(indicatorLit);
        }
    }
}
