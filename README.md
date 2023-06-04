# gbjVccRef

The library measures internal reference voltage (1.1V) against AVcc voltage of the internal analog-digital converter and calculates current power supply voltage utilized as a reference voltage for analog readings.

* The library is aimed for Arduino microcontrollers with Atmel microprocessors only.
* As a feature of the microcontroller the library instance object keeps the millivolts difference from the standard internal reference voltage 1.1V (1100 mV), hereinafter referred as "internal reference difference".
* The internal reference difference is kept as a signed byte data type, so that its value space is `-127 ~ +127 mV`, which is `±11.5%`, and should be sufficient for regular and functional microcontrollers.
* The range of internal reference voltage with expected internal reference difference (1100 ± 127 mV = 973 ~ 1227 mV) expressed in bits for 10-bit resolution of microcontroller's ADC and nominal power supply (Internal reference voltage / Nominal power supply * 1024):
  * **5000 mV**: 199 ~ 251 bits, nominally 225 bits
  * **3300 mV**: 302 ~ 381 bits, nominally 341 bits


<a id="dependency"></a>

## Dependency
* **Arduino.h**: Main include file for the Arduino SDK version greater or equal to 100.
* **inttypes.h**: Integer type conversions. This header file includes the exact-width integer definitions and extends them with additional facilities provided by the implementation.


<a id="interface"></a>

## Interface
* [gbj_vccref()](#gbj_vccref)
* [begin()](#begin)
* [calcVoltage()](#calcVoltage)
* [calcRefFactor()](#calcRefFactor)
* [measureVcc()](#measureVcc)

#### Getters
* [getRefVoltage()](#getRefVoltage)
* [getRefFactor()](#getRefFactor)
* [getRefDiff()](#getRefDiff)


<a id="gbj_vccref"></a>

## gbj_vccref()

#### Description
Constructor stores the difference of internal reference voltage from standard reference voltage 1100 mV.
* The difference is a specific value for individual microcontroller.
* The difference should be obtained separatelly:
  1. Call constructor without parameters at first.
  2. Provide method [begin()](#begin) with real power supply voltage measured by a multimeter.
  3. Call method [getRefDiff()](#getRefDiff) to obtain internal reference difference.
  4. Call constructor again with internal reference difference.
* The difference can be stored in one byte of <abbr title='Electrically Erasable Programmable Read-Only Memory'>EEPROM</abbr> memory of the microcontroller as its configuration value.

#### Syntax
    gbj_vccref(int8_t refDiff)

#### Parameters
* **refDiff**: Difference of the internal reference voltage against standard 1.1V in millivolts.
  * *Valid values*: integer -127 ~ 127
  * *Default value*: 0

#### Returns
Object preforming the default reference voltage measurement.

#### Example
```cpp
gbj_vccref Vref = gbj_vccref();     // Microcontroller is ideal with exact standard internal reference voltage
gbj_vccref Vref = gbj_vccref(-35);  // Internal reference difference figured out separately
```

#### See also
[begin()](#begin)

[measureVcc()](#measureVcc)

[Back to interface](#interface)


<a id="begin"></a>

## begin()

#### Description
The method executes initailizaton actions and flows of the library.
* The method calculates internal reference factor, which is the bit value of the internal reference voltage 1.1V at 10-bit ADC resolution.
* If measured power supply voltage is provided, i.e., it is not zero, the method calculates real internal reference voltage, which usually differs from 1100 mV.
* If measured power supply voltage is not provided, i.e., it is zero, the internal reference difference should be provided to the constructor, else standard voltages are used.
* The standard (ideal) internal reference voltage is 1100 mV.
* The standard (ideal) power supply Vcc voltage is 5000 or 3300 mV.

#### Syntax
	void begin(uint16_t measuredVcc)

#### Parameters
* **measuredVcc**: Real measured power supply voltage in millivolts.
  * *Valid values*: non-negative integer 0 ~ 65535
  * *Default value*: 0

#### Returns
None

#### See also
[gbj_vccref()](#gbj_vccref)

[Back to interface](#interface)


<a id="calcVoltage"></a>

## calcVoltage()

#### Description
The method calculates voltage from provided bit level value gained from analog reading with respect to current internal reference difference.
* Internal reference difference is kept in the library instance object as the [constructor's](#gbj_vccref) parameter _refDiff_.

#### Syntax
	uint16_t calcVoltage(uint16_t bitLevel)

#### Parameters
* **bitLevel**: Bit representation of the analog reading.
  * *Valid values*: non-negative integer 0 ~ 1023
  * *Default value*: none

#### Returns
Voltage in millivolts.

#### See also
[gbj_vccref()](#gbj_vccref)

[measureVcc()](#measureVcc)

[Back to interface](#interface)


<a id="calcRefFactor"></a>

## calcRefFactor()

#### Description
The method reads internal 1.1V reference voltage against AVcc reference voltage of the analog-digital converter.
* Reference factor is internal reference voltage as a fraction of AVcc default reference voltage expressed in bits at 10-bit resolution.
* At stantard power supply voltage 5V the factor is int(1100 mV / 5000 mV * 1024) = 225.
* At stantard power supply voltage 3.3V the factor is int(1100 mV / 3300 mV * 1024) = 341.
* The ratio of standard 1100 mV reference voltage and real reference number is voltage per bit resolution, e.g., 1100/225 = 4.89 mV/bit, resp. 1100/341 = 3.23 mV/bit.

#### Syntax
	uint8_t calcRefFactor()

#### Parameters
None

#### Returns
Actual reference factor.

#### See also
[begin()](#begin)

[Back to interface](#interface)


<a id="measureVcc"></a>

## measureVcc()

#### Description
The method calculates current power supply voltage from internal reference voltage defined by the internal reference difference provided to the the [constructor's](#gbj_vccref) by its parameter _refDiff_.

#### Syntax
    uint16_t measureVcc()

#### Parameters
None

#### Returns
Current power supply voltage in millivolts.

[Back to interface](#interface)


<a id="getRefVoltage"></a>

## getRefVoltage()

#### Description
The method returns calculated internal reference voltage in millivolts as specific value of the microcontroller, which is ideally 1100 mV.

#### Syntax
    uint16_t getRefVoltage()

#### Parameters
None

#### Returns
Internal reference voltage in millivolts.

[Back to interface](#interface)


<a id="getRefFactor"></a>

## getRefFactor()

#### Description
The method returns _internal reference voltage factor_, which is the number of bits representing the internal reference voltage as a fraction of power supply voltage expressed in bits at 10-bit resolution.

#### Syntax
    uint8_t getRefFactor()

#### Parameters
None

#### Returns
Internal reference factor in bits of the microcontroller.

[Back to interface](#interface)


<a id="getRefDiff"></a>

## getRefDiff()

#### Description
The method returns _internal reference difference_ in millivolts against standard internal reference voltage 1100 mV.
* The internal reference difference is considered as a specific feature of the individual microcontroller.

#### Syntax
    int8_t getRefDiff()

#### Parameters
None

#### Returns
Internal reference difference of the microcontroller.

[Back to interface](#interface)
