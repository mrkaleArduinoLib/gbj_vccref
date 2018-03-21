/*
  NAME:
  gbj_vccref

  DESCRIPTION:
  The library measures internal reference voltage (1.1V) against AVcc voltage
  of the internal analog-digital converter and calculates current power supply
  voltage utilized as a reference voltage for analog readings.
  - As a feature of the microcontroller the library instance object keeps
    the millivolts difference from the standard internal reference voltage
    1.1V (1100 mV), hereinafter referred as "internal reference difference".
  - The internal reference difference is kept as a signe byte data type, so
    that its value space is -127 ~ +127 mV, which is +/-11.5%, and should
    be sufficient for regular and functional microcontrollers.
  - The the range of internal reference voltage with expected internal
    reference difference (1100 +/-127 mV = 973 ~ 1227) expressed in bits
    for 10-bit resolution of microcontroller's ADC and nominal power supply
    5000 mV related to maximal bit ADC value 1023 is 199 ~ 251
    (Internal reference voltage / Nominal power supply * 1023), for which
    unsigned byte is sufficient.
  - The internal reference volatage 1100mV at nominal power supply 5000mV
    corresponds to 225 bits.
  - The question is: if maximal ADC value 1023 corresponds to nominal power
    supply 5000mV, what ADC value corresponds to internal reference voltage
    1100 mV?
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
  SCOTT: Secret Arduino Voltmeter - Measure Battery Voltage
    (http://provideyourown.com/2012/secret-arduino-voltmeter-measure-battery-voltage/)
  tinkerit: SecretVoltmeter.wiki
    (https://code.google.com/archive/p/tinkerit/wikis/SecretVoltmeter.wiki)
  Roberto Lo Giacco: VoltageReference
    (https://github.com/rlogiacco/VoltageReference.git)

 */
#ifndef GBJ_VCCREF_H
#define GBJ_VCCREF_H
#define GBJ_VCCREF_VERSION "GBJ_VCCREF 1.1.0"

#if defined(__AVR__)
  #if ARDUINO >= 100
    #include "Arduino.h"
  #else
    #include "WProgram.h"
  #endif
  #include <inttypes.h>
#endif


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
  - The difference should be obtained separatelly
    - call constructor without parameters at first
    - call calcDiff() with real power supply voltage measured by a multimeter
    - call constructor again with internal reference voltage difference
  - The difference can be stored in one byte of EEPROM memory of the
    microcontroller as its configuration value.

  PARAMETERS:
  refDiff - Difference of the internal reference voltage against standard
            1.1V in millivolts.
            - Data type: integer
            - Default value: 0
            - Limited range: -127 ~ 127 (+/-PRM_1V1_DIF)

  RETURN:
  Library instance object performing the voltage supply adjustment.
*/
  gbj_vccref(int8_t refDiff = 0);


/*
  Calculate voltage for current internal reference difference

  DESCRIPTION:
  The method calculates voltage from provided bit level value gained from
  analog reading with respect to current internal reference difference.
  - Internal reference difference is kept in the library instance object.

  PARAMETERS:
  bitLevel - Bit representation of the analog reading
             - Data type: positive integer
             - Default value: none
             - Limited range: 0 ~ 1023 (PRM_BIT_MAX)

  RETURN:
  Voltage in millivolts.
*/
  uint16_t calcVoltage(uint16_t bitLevel);


/*
  Calculate internal reference difference from measured Vcc

  DESCRIPTION:
  The method calculates difference of internal reference voltage against
  standard reference voltage 1100mV from measured power supply voltage.
  - The standard power supply Vcc voltage is 5000mV.

  PARAMETERS:
  inputVcc - Real measured input power supply voltage in millivolts.
             - Data type: positive integer
             - Default value: 5000 (PRM_5V0_REF)
             - Limited range: 4500 ~ 5500 (PRM_5V0_REF +/- PRM_5V0_DIF)

  RETURN:
  Internal reference voltage difference in millivolts.
*/
  int8_t calcDiff(uint16_t inputVcc = PRM_5V0_REF);


/*
  Measure current power supply voltage in millivolts

  DESCRIPTION:
  The method calculates current power supply voltage from typical internal
  reference voltage defined by the internal reference difference provided to
  the constructor.

  PARAMETERS: none

  RETURN:
  Current power supply voltage in millivolts.
*/
  inline uint16_t measureVcc() { return calcVoltage(PRM_BIT_MAX); };


//------------------------------------------------------------------------------
// Public getters
//------------------------------------------------------------------------------
  /* Internal reference voltage factor.
   * It is the number of bits representing the internal reference voltage
   * as a fraction of power supply voltage expressed as 1023 bits at
   * 10-bit resolution.
   */
  inline uint8_t getFactor() { return _refFactor; };
  inline int8_t getDiff() { return _refDiff; }; // Reference voltage difference of the microcontroller
  inline uint16_t getRef() { return PRM_1V1_REF + _refDiff; }; // Reference voltage in millivolt of the microcontroller


private:
//------------------------------------------------------------------------------
// Private constants
//------------------------------------------------------------------------------
enum Parameters
{
  PRM_BIT_MAX = 1023  // Maximal bit value of the AD converter
  PRM_1V1_REF = 1100, // Standard internal reference voltage in millivolts
  PRM_1V1_DIF = 127,  // Reasonable tolerance of internal reference voltage
  PRM_5V0_REF = 5000, // Standard supply voltage in millivolts
  PRM_5V0_DIF = 500,  // Reasonable tolerance of standard supply voltage  
};


//------------------------------------------------------------------------------
// Private attributes
//------------------------------------------------------------------------------
  int8_t _refDiff;  // Millivolt difference from standard 1.1V reference
  uint8_t _refFactor; // Reference factor (bits of internal reference voltage)


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
  - At stantard voltages the factor is int(1100 mV / 5000 mV * 1024) = 225.
  - The ratio of standard 1100mV reference voltage and real reference number
    is voltage per bit resolution, e.g., 1100/225 = 4.89mV/bit.

  PARAMETERS: none

  RETURN:
  Actual reference factor.
*/
  uint8_t readFactor();
};

#endif
