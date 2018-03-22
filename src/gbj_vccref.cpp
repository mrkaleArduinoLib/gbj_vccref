#include "gbj_vccref.h"


gbj_vccref::gbj_vccref(int8_t refDiff)
{
  refDiff = constrain(refDiff, -1 * PRM_1V1_DIF, PRM_1V1_DIF);
  _refVoltage = refDiff + PRM_1V1_REF;
}


void gbj_vccref::begin(uint16_t measuredVcc)
{
  _refFactor = calcRefFactor();
  if (measuredVcc > 0) _refVoltage = (2UL * _refFactor * measuredVcc + PRM_BIT_MAX) / (2 * PRM_BIT_MAX);
}


uint8_t gbj_vccref::calcRefFactor()
{
  // Read 1.1V reference against AVcc
  #if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
    ADMUX = _BV(REFS0) | _BV(MUX4) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  #elif defined (__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__)
    ADMUX = _BV(MUX5) | _BV(MUX0);
  #elif defined (__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
    ADMUX = _BV(MUX3) | _BV(MUX2);
  #else
    ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  #endif

  delay(2); // Wait for Vref to settle
  ADCSRA |= _BV(ADSC); // Start conversion
  while (bit_is_set(ADCSRA,ADSC)); // measuring

  uint8_t low  = ADCL; // must read ADCL first - it then locks ADCH
  uint8_t high = ADCH; // unlocks both
  uint8_t factor = (high<<8) | low;

  return factor;
}
