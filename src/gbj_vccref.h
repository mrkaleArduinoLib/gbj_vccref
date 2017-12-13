/*
  NAME:
  gbj_vccref

  DESCRIPTION:
  The library measures internal reference voltage (1.1V) against AVcc voltage
  of the internal analog-digital converter and calculates default reference
  voltage for analog readings.
  - Library is inspired by the Roberto Lo Giacco's library VoltageReference,
    but totally rewritten.

  LICENSE:
  This program is free software; you can redistribute it and/or modify
  it under the terms of the license GNU GPL v3 http://www.gnu.org/licenses/gpl-3.0.html
  (related to original code) and MIT License (MIT) for added code.

  CREDENTIALS:
  Author: Libor Gabaj
  GitHub: https://github.com/mrkaleArduinoLib/gbj_vccref.git

  CREDITS:
  SCOTT: Secret Arduino Voltmeter � Measure Battery Voltage
    (http://provideyourown.com/2012/secret-arduino-voltmeter-measure-battery-voltage/)
  tinkerit: SecretVoltmeter.wiki
    (https://code.google.com/archive/p/tinkerit/wikis/SecretVoltmeter.wiki)
  Roberto Lo Giacco: VoltageReference
    (https://github.com/rlogiacco/VoltageReference.git)
 */
#ifndef GBJ_VCCREF_H
#define GBJ_VCCREF_H
#define GBJ_VCCREF_VERSION "GBJ_VCCREF 1.0.0"

#if defined(__AVR__)
  #if ARDUINO >= 100
    #include "Arduino.h"
  #else
    #include "WProgram.h"
  #endif
  #include <inttypes.h>
#endif

#define GBJ_VCCREF_1V1_DEF  1100  // Default (standard) internal reference voltage in millivolts
#define GBJ_VCCREF_1V1_MIN  1000  // Minimal valid internal reference voltage
#define GBJ_VCCREF_1V1_MAX  1200  // Maximal valid internal reference voltage
#define GBJ_VCCREF_5V0_DEF  5000  // Standard default reference voltage in millivolts
#define GBJ_VCCREF_5V0_MIN  4500  // Minimal valid supply voltage in millivolts
#define GBJ_VCCREF_5V0_MAX  5500  // Maximal valid supply voltage in millivolts
#define GBJ_VCCREF_VAL_MAX  1023  // Measuring intervals at 10 bit resolution

class gbj_vccref
{
public:
//------------------------------------------------------------------------------
// Public methods
//------------------------------------------------------------------------------


/*
  Constructor

  DESCRIPTION:
  Constructor stores the difference of internal reference voltage from
  standard reference voltage 1.1V.
  - The difference is a specific value for individual microcontroller.
  - The difference should be obtained separatelly, e.g., with help of the
    'begin' method and measuring the power supply voltage.
  - The difference can be stored in one byte of EEPROM memory of the
    microcontroller as its configuration value.

  PARAMETERS:
  refDiff - Difference of the internal reference voltage against standard
            1.1V in millivolts.
            - Data type: integer
            - Default value: 0
            - Limited range: -100 ~ 100 (GBJ_VCCREF_1V1_MIN - GBJ_VCCREF_1V1_DEF,
                                         GBJ_VCCREF_1V1_MAX - GBJ_VCCREF_1V1_DEF)

  RETURN:
  Library instance object performing the voltage supply adjustment.
*/
  gbj_vccref(int8_t refDiff = 0);


/*
  Calculate reference difference from measured Vcc

  DESCRIPTION:
  The method calculates difference of internal reference voltage against
  standard reference voltage 1.1V from measured power supply voltage.
  - The default Vcc voltage is standard 5.0 V.

  PARAMETERS:
  inputVcc - Real measured input power supply voltage in millivolts.
             - Data type: positive integer
             - Default value: 5000 (GBJ_VCCREF_VCC_DEF)
             - Limited range: 4500 ~ 5500 (GBJ_VCCREF_5V0_MIN ~ GBJ_VCCREF_5V0_MAX)

  RETURN:
  Internal reference voltage difference in millivolts.
*/
  int8_t begin(uint16_t inputVcc = GBJ_VCCREF_5V0_DEF);


/*
  Measure current power supply voltage in millivolts

  DESCRIPTION:
  The method calculates current power supply voltage from internally measured
  reference factor and stored internal reference voltage difference typical for
  a microcontroller.
  - The method saves the current reference factor to the instance object.

  PARAMETERS: none

  RETURN:
  Current power supply voltage in millivolts.
*/
  uint16_t measureVcc();


//------------------------------------------------------------------------------
// Public getters
//------------------------------------------------------------------------------
  /* Internal reference voltage factor.
   * It is the number of bits representing the internal reference voltage
   * as a fraction of power supply voltage expressed as 1023 bits at
   * 10-bit resolution.
   */
  uint8_t  getFactor();
  int8_t   getDiff();  // Reference voltage difference of the microcontroller
  uint16_t getRef();   // Reference voltage in millivolt of the microcontroller


private:
//------------------------------------------------------------------------------
// Private attributes
//------------------------------------------------------------------------------
  int8_t  _refDiff;     // Millivolt difference from standard 1.1V reference
  uint8_t _refFactor;   // Reference factor (bits of internal reference voltage)


//------------------------------------------------------------------------------
// Private methods
//------------------------------------------------------------------------------


/*
  Read internal reference factor

  DESCRIPTION:
  The method reads internal 1.1V reference voltage against AVcc reference
  voltage of the analog-digital converter.
  - Reference factor is internal reference voltage as a fraction of AVcc
    default reference voltage expressed in bits at 10-bit resolution.
  - At stantard voltages the factor is 1100 mV / 5000 mV * 1023 = 225.

  PARAMETERS: none

  RETURN:
  Actual reference factor.
*/
  uint8_t readFactor();
};

#endif
