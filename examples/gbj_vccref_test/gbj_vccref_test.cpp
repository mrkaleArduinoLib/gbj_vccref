/*
  NAME:
  Demonstration of calculations provided by gbjVccRef library

  DESCRIPTION:
  The sketch displays all values included in internal reference voltage
  measurements.
  - Internal
  reference difference should be obtained separatelly, e.g., with appropriate
  example sketch.

  LICENSE:
  This program is free software; you can redistribute it and/or modify it under
  the terms of the MIT License (MIT).

  CREDENTIALS:
  Author: Libor Gabaj
*/
#include "gbj_vccref.h"

const char UNIT_V[] = " mV";
// Internal reference difference in millivolts
const int INPUT_DIFF = -39;

gbj_vccref vref = gbj_vccref(INPUT_DIFF);

void setup()
{
  Serial.begin(9600);
  Serial.println("---");
  vref.begin();
  //
  Serial.print("Measured Vcc: ");
  Serial.print(vref.measureVcc());
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
