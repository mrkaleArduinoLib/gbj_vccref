/*
  NAME:
  Demonstration of calculations provided by gbjVccRef library

  DESCRIPTION:
  The sketch displays all values included in internal reference voltage
  measurements.
  - Setup of the library can be provided by separately obtained internal
    reference difference or by measured power supply voltage both in mV.
  - If power supply voltage is provided (e.g., with multimeter), i.e., it is not
    zero, the method calculates real internal reference voltage, which usually
    differs from 1100 mV.
  - If power supply voltage is not provided, i.e., it is zero, the internal
    reference difference should be provided to the constructor,
    else standard voltages are used.

  LICENSE:
  This program is free software; you can redistribute it and/or modify
  it under the terms of the MIT License (MIT).

  CREDENTIALS:
  Author: Libor Gabaj
*/
#include "gbj_vccref.h"
#define SKETCH "GBJ_VCCREF_DEMO 1.1.0"

#define UNIT_V " mV"
const int INPUT_DIFF = -35; // Typical internal reference difference in millivolts
const int INPUT_VCC = 5070; // Put here measured power supply voltage in millivolts
// const int INPUT_VCC = 0; // Suppres calculating internal reference voltage from supply voltage

// Measurement
gbj_vccref Vref = gbj_vccref(INPUT_DIFF);


void setup()
{
  Serial.begin(9600);
  Serial.println(SKETCH);
  Serial.println("Libraries:");
  Serial.println(GBJ_VCCREF_VERSION);
  Serial.println("---");
  // Initialization
  Vref.begin(INPUT_VCC);
  //
  Serial.print("Input Vcc: ");
  Serial.print(INPUT_VCC);
  Serial.println(UNIT_V);
  //
  Serial.print("Factor: ");
  Serial.println(Vref.getRefFactor());
  //
  Serial.print("Reference: ");
  Serial.print(Vref.getRefVoltage());
  Serial.println(UNIT_V);
  //
  Serial.print("Difference: ");
  Serial.print(Vref.getRefDiff());
  Serial.println(UNIT_V);
  //
  Serial.print("Measured Vcc: ");
  Serial.print(Vref.measureVcc());
  Serial.println(UNIT_V);
}


void loop() {}
