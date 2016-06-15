// Ghastly color generator
//
// A dmx transmitter that produces dmx signal suitable for controlling a BeamZ
// led Flatpar-186x10mm rgbw ir dmx light. Generates random flashes that look
// great projected through a ḿodified fan.

// Also fenerates a color sequence that is suitable for Cameio Flat Pro Outdoor
// Par Can in 3 channel mode 2. Generates a color sequence that is suitable for
// the "colorful appearing and disappearing ghost" effect.
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
#define COLOR_SEQUENCE_LENGTH 14
uint8_t colorSequence[COLOR_SEQUENCE_LENGTH][4] = {
    {0xff, 0x00, 0x00, 60},
    {0xff, 0x00, 0x00, 240},
    {0x00, 0xff, 0x00, 60},
    {0x30, 0xd0, 0x00, 10},
    {0x00, 0xff, 0x00, 8},
    {0x20, 0xe0, 0x00, 15},
    {0x00, 0xff, 0x00, 10},
    {0x40, 0xc0, 0x00, 8},
    {0x00, 0xff, 0x00, 13},
    {0x30, 0xd0, 0x00, 10},
    {0x00, 0xff, 0x00, 7},
    {0x00, 0xff, 0x00, 20},
    {0xff, 0x00, 0x00, 60},
    {0xff, 0x00, 0x00, 240}
};

/// \brief
///    Sets color of the ghastly colorful apparition light.
///
/// \param current
///    The current color in color sequence
///
/// \param next
///    Next color in the color sequence
///
/// \param gradient
///    Gradient position
void setColor(uint8_t * current, uint8_t * next, uint8_t gradient) {
    int16_t nextRed = current[0] + (next[0] - current[0])*gradient/next[3];
    int16_t nextGreen = current[1] + (next[1] - current[1])*gradient/next[3];
    int16_t nextBlue = current[2] + (next[2] - current[2])*gradient/next[3];
    DmxSimple.write(DMX_COLORFUL_CHANNEL, nextRed);
    DmxSimple.write(DMX_COLORFUL_CHANNEL + 1, nextGreen);
    DmxSimple.write(DMX_COLORFUL_CHANNEL + 2, nextBlue);
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

    // White is not used
    DmxSimple.write(DMX_FAN_CHANNEL+4, 0x00);
    // Master brightness full always
    DmxSimple.write(DMX_FAN_CHANNEL, 0xff);
    // Set auto/sound mode and stroboscope off
    DmxSimple.write(DMX_FAN_CHANNEL+5, 0x00);
    DmxSimple.write(DMX_FAN_CHANNEL+6, 0x00);

    // Set initial color
    uint8_t initialColor[3] = {INITIAL_RED, INITIAL_GREEN, INITIAL_BLUE};
    setColor(initialColor, initialColor, 0);

    _delay_ms(INITIAL_DELAY);

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
