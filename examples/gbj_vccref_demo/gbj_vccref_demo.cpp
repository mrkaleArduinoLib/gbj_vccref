/*
  NAME:
  Basic usage of the gbjVccRef library

  DESCRIPTION:
  The sketch measures floating analog input pin at current internal reference
  voltage.
  - The sketch relies on internal reference difference obtained separately
    and used for initialization of the library.

  LICENSE:
  This program is free software; you can redistribute it and/or modify
  it under the terms of the MIT License (MIT).

  CREDENTIALS:
  Author: Libor Gabaj
*/
#include "gbj_vccref.h"

const char UNIT_V[] = " mV";
// Internal reference difference in mV
const int INPUT_DIFF = -39;
// Time in miliseconds between measurements
const unsigned int PERIOD_MEASURE = 3000;
// Pin for analog reading
const unsigned char PIN_TEST = A0;

unsigned int level, voltage;
gbj_vccref vref = gbj_vccref(INPUT_DIFF);

void setup()
{
  analogReference(DEFAULT);
  //
  Serial.begin(9600);
  Serial.println("---");
  vref.begin();
  //
  Serial.print("Factor: ");
  Serial.println(vref.getRefFactor());
  //
  Serial.print("vref: ");
  Serial.print(vref.getRefVoltage());
  Serial.println(UNIT_V);
  //
  Serial.print("Vcc: ");
  Serial.print(vref.measureVcc());
  Serial.println(UNIT_V);
  //
  Serial.println("---");
  Serial.println("Bits\tInput(mV)");
}

void loop()
{
  level = analogRead(PIN_TEST);
  voltage = vref.calcVoltage(level);
  Serial.print(level);
  Serial.print("\t");
  Serial.println(voltage);
  delay(PERIOD_MEASURE);
}
