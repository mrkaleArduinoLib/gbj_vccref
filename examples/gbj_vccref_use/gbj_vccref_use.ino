/*
  NAME:
  Basic usage of the gbjVccRef library

  DESCRIPTION:
  The sketch measures floating analog input pin at current internal
  reference voltage.
  - The sketch relies on internal reference difference obtained separately
    and used for initialization of the library.

  LICENSE:
  This program is free software; you can redistribute it and/or modify
  it under the terms of the MIT License (MIT).

  CREDENTIALS:
  Author: Libor Gabaj
*/
#include "gbj_vccref.h"
#define SKETCH "GBJ_VCCREF_USE 1.1.0"

#define UNIT_V " mV"
const int INPUT_DIFF = -16;                 // Internal reference difference in mV
const unsigned int PERIOD_MEASURE = 3000;   // Time in miliseconds between measurements

// Hardware configuration
const unsigned char PIN_TEST = A0;          // Pin for analog reading

// Measurement
unsigned int level, voltage, reference;
gbj_vccref Vref = gbj_vccref(INPUT_DIFF);


void setup()
{
  analogReference(DEFAULT);
  //
  Serial.begin(9600);
  Serial.println(SKETCH);
  Serial.println("Libraries:");
  Serial.println(GBJ_VCCREF_VERSION);
  Serial.println("---");
  Serial.println("Bits\tInput(mV)\tReference(mV)");
}


void loop()
{
  level = analogRead(PIN_TEST);
  reference = Vref.measureVcc();
  voltage = Vref.calcVoltage(level);
  Serial.print(level);
  Serial.print("\t");
  Serial.print(voltage);
  Serial.print("\t\t");
  Serial.print(reference);
  Serial.println();
  delay(PERIOD_MEASURE);
}
