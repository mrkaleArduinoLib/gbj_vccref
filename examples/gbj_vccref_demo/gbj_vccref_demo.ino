/*
  NAME:
  Demonstration of calculations provided by gbjVccRef library

  DESCRIPTION:
  The sketch displays all values included in internal reference voltage
  measurements.
  - Setup of the library can be provided by separately obtained internal
    reference difference or by measured power supply voltage both in mV.
  - The way of initialition is determined by boolean constant just for
    demonstration purposes.
  - The method always recalculates and rewrites the internal reference
    difference set by the constructor.

  LICENSE:
  This program is free software; you can redistribute it and/or modify
  it under the terms of the MIT License (MIT).

  CREDENTIALS:
  Author: Libor Gabaj
*/
#include "gbj_vccref.h"
#define SKETCH "GBJ_VCCREF_DEMO 1.0.0"

#define UNIT_V " mV"
const int INPUT_DIFF = -16;
const int INPUT_VCC = 5040;
const bool SET_VCC = true;  // Change it to display another initialization

// Measurement
gbj_vccref Vref = gbj_vccref(INPUT_DIFF);


void setup()
{
  Serial.begin(9600);
  Serial.println(SKETCH);
  Serial.println("Libraries:");
  Serial.println(GBJ_VCCREF_VERSION);
  Serial.println("---");
  // Print header
  if (SET_VCC)
  {
    Vref.begin(INPUT_VCC); // Recalculate reference difference
    Serial.print("Input Vcc: ");
    Serial.print(INPUT_VCC);
    Serial.println(UNIT_V);
    //
    Serial.print("Factor: ");
    Serial.println(Vref.getFactor());
    //
    Serial.print("Difference: ");
    Serial.print(Vref.getDiff());
    Serial.println(UNIT_V);
  }
  else
  {
    Serial.print("Input difference: ");
    Serial.print(INPUT_DIFF);
    Serial.println(UNIT_V);
  }
  //
  Serial.print("Reference: ");
  Serial.print(Vref.getRef());
  Serial.println(UNIT_V);
  //
  Serial.print("Current Vcc: ");
  Serial.print(Vref.measureVcc());
  Serial.print(UNIT_V);
  Serial.print(" at factor: ");
  Serial.println(Vref.getFactor());
}


void loop() {}
