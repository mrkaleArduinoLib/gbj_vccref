/*
  NAME:
  Calculate Internal reference difference by gbjVccRef library

  DESCRIPTION:
  The sketch calculates internal reference difference of the microcontroller.
  - Measure power supply voltage with a multimeter and put it to the constant.
  - Utilize calculated difference in other production sketches and/or store it
  in microcontroller's EEPROM as a configuration parameter.

  LICENSE:
  This program is free software; you can redistribute it and/or modify it under
  the terms of the MIT License (MIT).

  CREDENTIALS:
  Author: Libor Gabaj
*/
#include "gbj_vccref.h"

const char UNIT_V[] = " mV";
// Power supply voltage in millivolts
const int INPUT_VCC = 5030;

gbj_vccref vref = gbj_vccref();

void setup()
{
  Serial.begin(9600);
  Serial.println("---");
  vref.begin(INPUT_VCC);
  //
  Serial.print("Input Vcc: ");
  Serial.print(INPUT_VCC);
  Serial.println(UNIT_V);
  //
  Serial.print("Internal Reference Difference: ");
  Serial.print(vref.getRefDiff());
  Serial.println(UNIT_V);
  //
  Serial.print("Internal Reference Voltage: ");
  Serial.print(vref.getRefVoltage());
  Serial.println(UNIT_V);
  //
  Serial.print("Reference Factor: ");
  Serial.print(vref.getRefFactor());
  Serial.println(" bits");
  Serial.println("---");
}

void loop() {}
