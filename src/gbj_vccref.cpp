#include "gbj_vccref.h"


// Constructor with previously figured out internal reference voltage difference
gbj_vccref::gbj_vccref(int8_t refDiff)
{
  _refDiff = constrain(refDiff, -1 * PRM_1V1_DIF, PRM_1V1_DIF);
}


// Measure current power supply voltage
uint16_t gbj_vccref::calcVoltage(uint16_t bitLevel)
{
  uint32_t voltage;
  // Calculate voltage with integer rounding
  voltage = (2 * getRef() * bitLevel + _refFactor) / (2 * _refFactor);
  return voltage;
}


// Calculate internal reference difference from measured power supply voltage
int8_t gbj_vccref::calcDiff(uint16_t inputVcc)
{
  uint32_t refVoltage;
  // Calculate internal reference factor
  _refFactor = readFactor();
  // Calculate internal reference voltage with integer rounding
  refVoltage = (2 * _refFactor * inputVcc + PRM_BIT_MAX) / (2 * PRM_BIT_MAX);
  _refDiff = refVoltage - PRM_1V1_REF;
  return _refDiff;
}


//-------------------------------------------------------------------------
// Private methods
//-------------------------------------------------------------------------
uint8_t gbj_vccref::readFactor()
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
