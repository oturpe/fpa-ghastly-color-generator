#include "Attiny45Utils.h"

#include <avr/io.h>

namespace Attiny45 {

void setTimer0Prescaler(TimerPrescalerValue value) {
  switch (value) {
  case PSV_1:
    TCCR0B |= BV(CS00);
    break;
  case PSV_8:
    TCCR0B |= BV(CS01);
    break;
  case PSV_64:
    TCCR0B |= BV(CS01) | BV(CS00);
    break;
  case PSV_256:
    TCCR0B |= BV(CS02);
    break;
  case PSV_1024:
    TCCR0B |= BV(CS02) | BV(CS00);
    break;
  default:
    break;
  }
}

void setTimer1Prescaler(TimerPrescalerValue value) {
  switch (value) {
  case PSV_1:
    TCCR1 |= BV(CS10);
    break;
  case PSV_2:
    TCCR1 |= BV(CS11);
    break;
  case PSV_4:
    TCCR1 |= BV(CS11) | BV(CS10);
    break;
  case PSV_8:
    TCCR1 |= BV(CS12);
    break;
  case PSV_16:
    TCCR1 |= BV(CS12) | BV(CS10);
    break;
  case PSV_32:
    TCCR1 |= BV(CS12) | BV(CS11);
    break;
  case PSV_64:
    TCCR1 |= BV(CS12) | BV(CS11) | BV(CS10);
    break;
  case PSV_128:
    TCCR1 |= BV(CS13);
    break;
  case PSV_256:
    TCCR1 |= BV(CS13) | BV(CS10);
    break;
  case PSV_512:
    TCCR1 |= BV(CS13) | BV(CS11);
    break;
  case PSV_1024:
    TCCR1 |= BV(CS13) | BV(CS11) | BV(CS10);
    break;
  case PSV_2048:
    TCCR1 |= BV(CS13) | BV(CS12);
    break;
  case PSV_4096:
    TCCR1 |= BV(CS13) | BV(CS12) | BV(CS10);
    break;
  case PSV_8192:
    TCCR1 |= BV(CS13) | BV(CS12) | BV(CS11);
    break;
  case PSV_16384:
    TCCR1 |= BV(CS13) | BV(CS12) | BV(CS11) | BV(CS10);
    break;
  }
}

}
