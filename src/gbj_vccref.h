/*
  NAME:
  gbjVccRef

  DESCRIPTION:
  The library measures internal reference voltage (1.1V) against AVcc voltage
  of the internal analog-digital converter and calculates current power supply
  voltage utilized as a reference voltage for analog readings.

  - As a feature of the microcontroller the library instance object keeps the
  millivolts difference from the standard internal reference voltage 1.1V (1100
  mV), hereinafter referred as "internal reference difference".

  - The internal reference difference is kept as a signed byte data type, so
  that its value space is -127 ~ +127 mV, which is +/-11.5%, and should be
  sufficient for regular and functional microcontrollers.

  - The range of internal reference voltage with expected internal reference
  difference (1100 +/-127 mV = 973 ~ 1227) expressed in bits for 10-bit
  resolution of microcontroller's ADC and nominal power supply (Internal
  reference voltage / Nominal power supply * 1024)
    - 5000 mV is 199 ~ 251 bits, nominally 225 bits
    - 3300 mV is 302 ~ 381 bits, nominally 341 bits

  - Library is inspired by the Roberto Lo Giacco's library VoltageReference, but
  totally rewritten.

  LICENSE:
  This program is free software; you can redistribute it and/or modify
  it under the terms of the license GNU GPL v3
  http://www.gnu.org/licenses/gpl-3.0.html (related to original code) and MIT
  License (MIT) for added code.

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

#if defined(__AVR__)
  #include <Arduino.h>
  #include <inttypes.h>
#else
  #error !!! Only Arduino family is supported !!!
#endif

class gbj_vccref
{
public:
  /*
    Constructor

    DESCRIPTION:
    Constructor stores the difference of internal reference voltage from
    standard reference voltage 1100 mV.
    - The difference is a specific value for individual microcontroller.
    - The difference should be obtained separatelly
      - call constructor without parameters at first
      - provide begin() with real power supply voltage measured by a multimeter
      - call getRefDiff() to obtain internal reference difference
      - call constructor again with internal reference difference
    - The difference can be stored in one byte of EEPROM memory of the
      microcontroller as its configuration value.

    PARAMETERS:
    refDiff - Difference of the internal reference voltage against standard 1100
    mV in millivolts.
      - Data type: integer
      - Default value: 0
      - Limited range: -127 ~ 127 (+/-PRM_1V1_DIF)

    RETURN: Library instance object performing the voltage supply adjustment.
  */
  gbj_vccref(int8_t refDiff = 0)
  {
    refDiff = constrain(refDiff, -1 * Params::PRM_1V1_DIF, Params::PRM_1V1_DIF);
    refVoltage_ = refDiff + Params::PRM_1V1_REF;
  }

  /*
    Initialization activities

    DESCRIPTION:
    The method executes initailizaton actions and flows of the library.
    - The method calculates internal reference factor, which is the bit value
      of the internal reference voltage 1.1V at 10-bit ADC resolution.
    - If measured power supply voltage is provided, i.e., it is not zero,
      the method calculates real internal reference voltage, which usually
    differs from 1100 mV.
    - If measured power supply voltage is not provided, i.e., it is zero,
      the internal reference difference should be provided to the constructor,
      else standard voltages are used.
    - The standard (ideal) internal reference voltage is 1100 mV.
    - The standard (ideal) power supply Vcc voltage is 5000 or 3300 mV.

    PARAMETERS:
    measuredVcc - Real measured power supply voltage in millivolts.
      - Data type: non-negative integer
      - Default value: 0
      - Limited range: 0 ~ 65535

    RETURN: None
  */
  void begin(uint16_t measuredVcc = 0)
  {
    refFactor_ = calcRefFactor();
    if (measuredVcc > 0)
    {
      refVoltage_ = (2UL * refFactor_ * measuredVcc + Params::PRM_BIT_MAX) /
                    (2 * Params::PRM_BIT_MAX);
    }
  }

  /*
    Calculate voltage for current internal reference difference

    DESCRIPTION:
    The method calculates voltage from provided bit level value gained from
    analog reading with respect to current internal reference voltage calculated
    at initialization.

    PARAMETERS:
    bitValue - Bit value of the analog reading.
      - Data type: positive integer
      - Default value: none
      - Limited range: 0 ~ 1023 (PRM_BIT_MAX)

    RETURN: Voltage in millivolts.
  */
  inline uint16_t calcVoltage(uint16_t bitValue)
  {
    return (2UL * refVoltage_ * bitValue + refFactor_) / (2 * refFactor_);
  };

  /*
    Calculate internal reference factor

    DESCRIPTION:
    The method reads internal 1.1V reference voltage against AVcc reference
    voltage of the analog-digital converter.
    - Reference factor is internal reference voltage as a fraction of AVcc
    default reference voltage expressed in bits at 10-bit resolution.
    - At stantard power supply voltage 5V the factor is int(1100 mV / 5000 mV *
    1024) = 225.
    - At stantard power supply voltage 3.3V the factor is int(1100 mV / 3300 mV
    * 1024) = 341.
    - The ratio of standard 1100 mV reference voltage and real reference number
    is voltage per bit resolution, e.g., 1100/225 = 4.89 mV/bit, resp. 1100/341
    = 3.23 mV/bit.

    PARAMETERS: None

    RETURN: Actual reference factor.
  */
  uint8_t calcRefFactor()
  {
// Read 1.1V reference against AVcc
#if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega1280__) ||              \
  defined(__AVR_ATmega2560__)
    ADMUX = _BV(REFS0) | _BV(MUX4) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
#elif defined(__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) ||                \
  defined(__AVR_ATtiny84__)
    ADMUX = _BV(MUX5) | _BV(MUX0);
#elif defined(__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) ||                \
  defined(__AVR_ATtiny85__)
    ADMUX = _BV(MUX3) | _BV(MUX2);
#else
    ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
#endif
    // Wait for Vref to settle
    delay(2);
    // Start conversion
    ADCSRA |= _BV(ADSC);
    // Measuring
    while (bit_is_set(ADCSRA, ADSC))
    {
      ;
    }
    // Must read ADCL first - it then locks ADCH
    uint8_t low = ADCL;
    // Unlocks both
    uint8_t high = ADCH;
    uint8_t factor = (high << 8) | low;
    return factor;
  }

  /*
    Measure current power supply voltage in millivolts

    DESCRIPTION:
    The method calculates current power supply voltage from internal reference
    voltage calculated at initialization.

    PARAMETERS: None

    RETURN: Current power supply voltage in millivolts.
  */
  inline uint16_t measureVcc() { return calcVoltage(Params::PRM_BIT_MAX); }

  // Getters
  inline uint8_t getRefFactor() { return refFactor_; }
  inline uint16_t getRefVoltage() { return refVoltage_; }
  inline int8_t getRefDiff() { return refVoltage_ - Params::PRM_1V1_REF; }

private:
  enum Params : int16_t
  {
    // Bit value range of the AD converter
    PRM_BIT_MAX = 1024,
    // Standard internal reference voltage in millivolts
    PRM_1V1_REF = 1100,
    // Reasonable tolerance of internal reference voltage
    PRM_1V1_DIF = 127,
  };

  /*
    Internal reference voltage factor in bits
    - It is the number of bits representing the internal reference voltage as a
    fraction of power supply voltage expressed as 1023 bits value at 10-bit
    resolution. - This is a typical value for particular microcontroller.
   */
  uint8_t refFactor_;

  /*
    Internal reference voltage in millivolts
    - Ideally it is the 1100 mV, but in reality it has tolerance +/-10%.
    - If measured power supply is provided, the library calculates real voltage.
   */
  uint16_t refVoltage_;
};

#endif
