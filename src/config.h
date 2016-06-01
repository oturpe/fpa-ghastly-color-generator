// Cpu frequency for util/delay.h
#define F_CPU 6000000

// Half of delay in main execution loop, given in milliseconds
#define LOOP_DELAY 50

// Half length of indicator led on-off sequence. In other words, the time
// the indicator is lit and darkened. Given in units of LOOP_DELAY.
#define INDICATOR_HALF_PERIOD 10

#define COLOR_PERIOD 1

// Dmx settings
#define DMX_PRESCALER Attiny45::PSV_256
#define DMX_PORT PORTB
#define DMX_PIN PORTB1
#define DMX_CHANNELS 7

// Initial color
#define INITIAL_RED 0xff
#define INITIAL_GREEN 0x00
#define INITIAL_BLUE 0x00

// Delay during which initial color is shown. Given in milliseconds
//#define INITIAL_DELAY (30 * 1000)
#define INITIAL_DELAY (30 * 1000)
