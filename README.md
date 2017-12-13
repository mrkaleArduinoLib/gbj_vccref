<a id="library"></a>
# gbjVccRef
The library measures internal reference voltage (1.1V) against AVcc voltage of the internal analog-digital converter and calculates default reference voltage (5V) for analog readings.


<a id="dependency"></a>
## Dependency
- **Arduino.h**: Main include file for the Arduino SDK version greater or equal to 100.
- **WProgram.h**: Main include file for the Arduino SDK version less than 100.
- **inttypes.h**: Integer type conversions. This header file includes the exact-width integer definitions and extends them with additional facilities provided by the implementation.


<a id="Constants"></a>
## Constants
- **GBJ\_VCCREF\_VERSION**: Name and semantic version of the library.
- **GBJ\_VCCREF\_1V1\_DEF**: Default (standard) internal reference voltage in millivolts.
- **GBJ\_VCCREF\_1V1\_MIN**: Minimal valid internal reference voltage.
- **GBJ\_VCCREF\_1V1\_MAX**: Maximal valid internal reference voltage.
- **GBJ\_VCCREF\_5V0\_DEF**: Standard default reference voltage in millivolts.
- **GBJ\_VCCREF\_5V0\_MIN**: Minimal valid supply voltage in millivolts.
- **GBJ\_VCCREF\_5V0\_MAX**: Maximal valid supply voltage in millivolts.
- **GBJ\_VCCREF\_VAL\_MAX**: Measuring intervals at 10 bit resolution.


<a id="interface"></a>
## Interface
- [gbj_vccref()](#gbj_vccref)
- [begin()](#begin)
- [measureVcc()](#measureVcc)

#### Getters
- [getRef()](#getRef)
- [getFactor()](#getFactor)
- [getDiff()](#getDiff)


<a id="gbj_vccref"></a>
## gbj_vccref()
#### Description
The constructor stores the difference of internal reference voltage from standard reference voltage 1.1V.
- The difference is a specific value for individual microcontroller.
- The difference should be obtained separately, e.g., with help of the [begin](#begin) method and measuring the power supply voltage.  
- The difference can be stored in one byte of EEPROM memory of the microcontroller as its configuration value.

#### Syntax
    gbj_vccref(int8_t refDiff);

#### Parameters
<a id="prm_refDiff"></a>
- **refDiff**: Difference of the internal reference voltage against standard 1.1V in millivolts.
  - *Valid values*: integer -100 ~ 100 ([GBJ\_VCCREF\_1V1\_MIN-GBJ\_VCCREF\_1V1\_DEF, GBJ\_VCCREF\_1V1\_MAX-GBJ\_VCCREF\_1V1\_DEF](#constants))
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

[Back to interface](#interface)


<a id="begin"></a>
## begin()
#### Description
The method calculates difference of internal reference voltage against standard reference voltage 1.1V from measured power supply voltage.
- The default Vcc voltage is standard 5.0 V.
- The internal reference difference is considered as a specific feature of the individual microcontroller.
- The method always recalculates and rewrites the difference set by the constructor parameter [refDiff](#prm_refDiff).

#### Syntax
	int8_t begin(uint16_t inputVcc);

#### Parameters
<a id="prm_inputVcc"></a>
- **inputVcc**: Real measured input power supply voltage in millivolts.
  - *Valid values*: integer 4500 ~ 5500 ([GBJ\_VCCREF\_5V0\_MIN ~ GBJ\_VCCREF\_5V0\_MAX](#constants))
  - *Default value*: 5000 ([GBJ\_VCCREF\_5V0\_DEF](#constants))

#### Returns
Internal reference voltage difference in millivolts.

#### See also
[gbj_vccref()](#gbj_vccref)

[Back to interface](#interface)


<a id="measureVcc"></a>
## measureVcc()
#### Description
The method calculates current power supply voltage from internally measured reference factor and stored internal reference voltage difference typical for the microcontroller.
- The method saves the current reference factor to the instance object.

#### Syntax
    uint16_t measureVcc();

#### Parameters
None

#### Returns
Current power supply voltage in millivolts.

[Back to interface](#interface)


<a id="getRef"></a>
## getRef()
#### Description
The method returns calculated internal reference voltage in millivolts as specific value of the microcontroller, which is standardly 1100 mV.

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
Recently measured internal reference factor.

[Back to interface](#interface)


<a id="getDiff"></a>
## getDiff()
#### Description
The method returns *internal reference voltage difference* in millivolts against standard internal reference voltage 1100 mV, which the [constructor](#gbj_vccref) has set or the method [begin](#begin()) has calculated.
- The internal reference difference is considered as a specific feature of the individual microcontroller.

#### Syntax
    int8_t getDiff();

#### Parameters
None

#### Returns
Internal reference difference of the microcontroller.

#### See also
[measureVcc()](#measureVcc)

[Back to interface](#interface)
