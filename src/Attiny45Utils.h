// Helpers for working with Attiny45

#ifndef ATTINY_45_UTILS_H
#define ATTINY_45_UTILS_H

// Cleaner setting of bits
#define BV(x) (1<<x)

namespace Attiny45 {

/// Possible prescaler values for timers.
enum TimerPrescalerValue {
    PSV_1,
    PSV_2,
    PSV_4,
    PSV_8,
    PSV_16,
    PSV_32,
    PSV_64,
    PSV_128,
    PSV_256,
    PSV_512,
    PSV_1024,
    PSV_2048,
    PSV_4096,
    PSV_8192,
    PSV_16384
};

/// Sets timer 0 prescaler to requested value.
///
/// This function assumes that Clock Select bits have not been touched yet. Note
/// that timer 0 does not support all Timer0PrescalerValue values. If illegal
/// value is entered, prescaler is not set to any value.
///
/// \param value
///    Requested prescaler value
void setTimer0Prescaler(TimerPrescalerValue value);

/// Sets timer 1 prescaler to requested value.
///
/// This function assumes that Clock Select bits have not been touched yet.
/// Timer 1 support all listed prescaler values.
///
/// \param value
///    Requested prescaler value
void setTimer1Prescaler(TimerPrescalerValue value);

}

#endif
