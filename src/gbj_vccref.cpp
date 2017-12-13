#include "gbj_vccref.h"


// Constructor with previously figured out internal reference voltage difference
gbj_vccref::gbj_vccref(int8_t refDiff)
{
  _refDiff = constrain(refDiff, GBJ_VCCREF_1V1_MIN - GBJ_VCCREF_1V1_DEF, GBJ_VCCREF_1V1_MAX - GBJ_VCCREF_1V1_DEF);
}


// Setup by measured power supply voltage
int8_t gbj_vccref::begin(uint16_t inputVcc)
{
  _refFactor = readFactor(); // Store recent (current) reference factor
  _refDiff = ((long)inputVcc * (long)_refFactor + (GBJ_VCCREF_VAL_MAX / 2)) / GBJ_VCCREF_VAL_MAX - GBJ_VCCREF_1V1_DEF;
  return _refDiff;
}


// Internal reference voltage in millivolts
uint16_t gbj_vccref::getRef()
{
  return GBJ_VCCREF_1V1_DEF + _refDiff;
}


// Measure current power supply voltage
uint16_t gbj_vccref::measureVcc()
{
  _refFactor = readFactor(); // Store recent (current) reference factor
  return ((long)getRef() * GBJ_VCCREF_VAL_MAX + ((long)_refFactor / 2)) / (long)_refFactor;
}


//-------------------------------------------------------------------------
// Getters
//-------------------------------------------------------------------------
int8_t  gbj_vccref::getDiff() { return _refDiff; }
uint8_t gbj_vccref::getFactor() { return _refFactor; }


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
