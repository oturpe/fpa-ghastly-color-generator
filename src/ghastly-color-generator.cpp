// Ghastly color generator
//
// A dmx transmitter that produces dmx signal suitable for controlling a BeamZ
// led Flatpar-186x10mm rgbw ir dmx light. Generates a color sequence that is
// suitable for the "colorful appearing and disappearing ghost" effect.
//
// Author: Otto Urpelainen

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

// Temp: some color sequence
#define COLOR_SEQUENCE_LENGTH 6
uint8_t colorSequence[COLOR_SEQUENCE_LENGTH][4] = {
    {0xff, 0x00, 0x00, 50},
    {0xff, 0x00, 0x00, 200},
    {0x00, 0xff, 0x00, 50},
    {0xff, 0x00, 0x00, 50},
    {0xff, 0x00, 0x00, 200},
    {0x00, 0x00, 0xff, 50}
};

void setColor(uint8_t * current, uint8_t * next, uint8_t gradient) {
    int16_t nextRed = current[0] + (next[0] - current[0])*gradient/next[3];
    int16_t nextGreen = current[1] + (next[1] - current[1])*gradient/next[3];
    int16_t nextBlue = current[2] + (next[2] - current[2])*gradient/next[3];
    DmxSimple.write(2, nextRed);
    DmxSimple.write(3, nextGreen);
    DmxSimple.write(4, nextBlue);
}

int main() {

    // Set output pins
    //    B0 (indicator)
    //    dmx output is set by library
    DDRB |= BV(DDB0);

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
    uint8_t colorSequenceCounter = 0;
    uint8_t gradient = 1;

    while(true) {
        counter += 1;
        _delay_ms(LOOP_DELAY);

        if(counter % INDICATOR_HALF_PERIOD == 0) {
            indicatorLit = !indicatorLit;
            setIndicator(indicatorLit);
        }

        if(counter % COLOR_PERIOD == 0) {
            uint8_t currentPosition = colorSequenceCounter % COLOR_SEQUENCE_LENGTH;
            uint8_t nextPosition = (colorSequenceCounter + 1) % COLOR_SEQUENCE_LENGTH;

            uint8_t * current = colorSequence[currentPosition];
            uint8_t * next = colorSequence[nextPosition];

            setColor(current, next, gradient);

            if (gradient == next[3]) {
                colorSequenceCounter++;
                gradient = 0;
            }
            else {
                gradient++;
            }
        }
    }
}
