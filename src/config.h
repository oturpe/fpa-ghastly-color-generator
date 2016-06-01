// Cpu frequency for util/delay.h
#define F_CPU 6000000

// Half of delay in main execution loop, given in milliseconds
#define LOOP_DELAY 60

// Half length of indicator led on-off sequence. In other words, the time
// the indicator is lit and darkened. Given in units of LOOP_DELAY.
#define INDICATOR_HALF_PERIOD 8

#define COLOR_PERIOD 1

// Dmx settings
#define DMX_PRESCALER Attiny45::PSV_256
#define DMX_PORT PORTB
#define DMX_PIN PORTB1
#define DMX_CHANNELS 7