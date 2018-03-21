<a id="library"></a>
# gbjVccRef
The library measures internal reference voltage (1.1V) against AVcc voltage of the internal analog-digital converter and calculates current power supply voltage (5V) utilized as a reference voltage for analog readings.
- As a feature of the microcontroller the library instance object keeps the millivolts difference from the standard internal reference voltage 1.1V (1100 mV), hereinafter referred as "internal reference difference".
- The internal reference difference is kept as a signe byte data type, so that its value space is **-127 ~ +127 mV, which is +/-11.5%**, and should be sufficient for regular and functional microcontrollers.
- The the range of internal reference voltage with expected internal reference difference **(1100 +/-127 mV = 973 ~ 1227 bits**) expressed in bits for 10-bit resolution of microcontroller's ADC and nominal power supply 5000 mV related to maximal bit ADC value 1023 leads to **reference factor range 199 ~ 251**
		Internal reference voltage / Nominal power supply * 1023
for which unsigned byte is sufficient.
- The internal reference volatage 1100mV at nominal power supply 5000mV corresponds to **225 bits**.
- The question is: *if maximal ADC value 1023 corresponds to nominal power supply 5000mV, what ADC value corresponds to internal reference voltage 1100 mV?*


<a id="dependency"></a>
## Dependency
- **Arduino.h**: Main include file for the Arduino SDK version greater or equal to 100.
- **WProgram.h**: Main include file for the Arduino SDK version less than 100.
- **inttypes.h**: Integer type conversions. This header file includes the exact-width integer definitions and extends them with additional facilities provided by the implementation.


<a id="Constants"></a>
## Constants
- **GBJ\_VCCREF\_VERSION**: Name and semantic version of the library.


<a id="interface"></a>
## Interface
- [gbj_vccref()](#gbj_vccref)
- [calcVoltage()](#calcVoltage)
- [calcDiff()](#calcDiff)
- [measureVcc()](#measureVcc)

#### Getters
- [getRef()](#getRef)
- [getFactor()](#getFactor)
- [getDiff()](#getDiff)


<a id="gbj_vccref"></a>
## gbj_vccref()
#### Description
The constructor stores the difference of internal reference voltage from standard reference voltage 1100 mV.
- The difference is a specific value for individual microcontroller.
- The difference should be obtained separatelly
	- call constructor without parameters at first
	- call [calcDiff()](#calcDiff) with real power supply voltage measured by a multimeter
	- call constructor again with internal reference voltage difference
- The difference can be stored in one byte of EEPROM memory of the microcontroller as its configuration value.

#### Syntax
    gbj_vccref(int8_t refDiff);

#### Parameters
<a id="prm_refDiff"></a>
- **refDiff**: Difference of the internal reference voltage against standard 1.1V in millivolts.
  - *Valid values*: integer -127 ~ 127
  - *Default value*: 0

#### Returns
Object preforming the default reference voltage measurement.

#### Example
``` cpp
gbj_vccref Vref = gbj_vccref();     // Microcontroller is ideal with exact standard internal reference voltage
gbj_vccref Vref = gbj_vccref(-16);  // Internal reference difference figured out separately
```

#### See also
[measureVcc()](#measureVcc)

[calcDiff()](#calcDiff)

[Back to interface](#interface)


<a id="calcVoltage"></a>
## calcVoltage()
#### Description
The method calculates voltage from provided bit level value gained from analog reading with respect to current internal reference difference.
- Internal reference difference is kept in the library instance object as as constructor parameter [refDiff](#prm_refDiff).

#### Syntax
	uint16_t calcVoltage(uint16_t bitLevel);

#### Parameters
- **bitLevel**: Bit representation of the analog reading.
  - *Valid values*: non-negative integer 0 ~ 1023
  - *Default value*: none

#### Returns
Voltage in millivolts.

#### See also
[gbj_vccref()](#gbj_vccref)

[Back to interface](#interface)


<a id="calcDiff"></a>
## calcDiff()
#### Description
The method calculates difference of internal reference voltage against standard internal reference voltage 1100 mV from measured power supply voltage.
- The standard power supply Vcc voltage is 5000 mV.
- The internal reference difference is considered as a specific feature of the individual microcontroller.
- The method always recalculates and rewrites the internal refereence difference set by the constructor parameter [refDiff](#prm_refDiff).

#### Syntax
	int8_t calcDiff(uint16_t inputVcc);

#### Parameters
- **inputVcc**: Real measured input power supply voltage in millivolts.
  - *Valid values*: integer 4500 ~ 5500
  - *Default value*: 5000

#### Returns
Internal reference voltage difference in millivolts.

#### See also
[gbj_vccref()](#gbj_vccref)

[calcVoltage()](#calcVoltage)

[Back to interface](#interface)


<a id="measureVcc"></a>
## measureVcc()
#### Description
The method calculates current power supply voltage from typical internal reference voltage defined by the internal reference difference provided to the constructor by its parameter [refDiff](#prm_refDiff). 

#### Syntax
    uint16_t measureVcc();

#### Parameters
None

#### Returns
Current power supply voltage in millivolts.

#### See also
[gbj_vccref()](#gbj_vccref)

[Back to interface](#interface)


<a id="getRef"></a>
## getRef()
#### Description
The method returns calculated internal reference voltage in millivolts as specific value of the microcontroller, which is ideally 1100 mV.

#### Syntax
    uint16_t getRef();

#### Parameters
None

#### Returns
Internal reference voltage in millivolts.

#### See also
[measureVcc()](#measureVcc)

[Back to interface](#interface)


<a id="getFactor"></a>
## getFactor()
#### Description
The method returns *internal reference voltage factor*, which is the number of bits representing the internal reference voltage as a fraction of power supply voltage expressed as 1023 bits at 10-bit resolution.

#### Syntax
    uint8_t getFactor();

#### Parameters
None

#### Returns
Internal reference factor in bits of the microcontroller.

[Back to interface](#interface)


<a id="getDiff"></a>
## getDiff()
#### Description
The method returns *internal reference difference* in millivolts against standard internal reference voltage 1100 mV, which the [constructor](#gbj_vccref) has set or the method [calcDiff()](#calcDiff) has calculated.
- The internal reference difference is considered as a specific feature of the individual microcontroller.

#### Syntax
    int8_t getDiff();

#### Parameters
None

#### Returns
Internal reference difference of the microcontroller.

#### See also
[measureVcc()](#measureVcc)

[calcDiff()](#calcDiff)

[Back to interface](#interface)
