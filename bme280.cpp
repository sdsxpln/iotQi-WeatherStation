// Copyright (c) Microsoft. All rights reserved.
// Copyright (c) LooUQ Incorporated. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include "bme280.h"

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
//#include "iotQi_FeatherOLED.h"
//#include "iotQi_FeatherOLED_WiFi.h"
//
//extern iotQi_FeatherOLED_WiFi display;

const int bme280_cs_pin__i = 12;
const float mbToInches = 0.0295299830714;

Adafruit_BME280 bme(bme280_cs_pin__i);

bool bme_init_result = true;
int _unitsType = UNITS_TYPE_METRIC;   

//int _oledEnabled = 0;
//int _oledDisplayLabelSet = 0;


float temperature;
float humidity;
float barPressure; 
///////////////////////////////////////////////////////////////////////////////////////////////////

void initBme(int unitsType) // int displayOLED)
{
  if (unitsType != UNITS_TYPE_METRIC)
  {
    _unitsType = UNITS_TYPE_IMPERIAL;
  }
//  _oledEnabled = displayOLED;
//
//  display.writeLineAt(2, 0, " Temp=");
//display.writeLineAt(3, 0, "Humid=");

  Serial.println("Checking for the presence of the BME280 temp/humid/press module.");
  bme_init_result = bme.begin();
  if (bme_init_result)
  {
    Serial.println("Found and initialized BME280 module.");
  }
  else
  {
    Serial.println("Warning! BME280 module not found.");
  }
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void readBmeValues()
{
//  if(!_oledDisplayLabelSet)
//  {
//     _oledDisplayLabelSet = 1;
//    display.writeLineAt(2, 0, " Temp=");
//    display.writeLineAt(3, 0, "Humid=");
//  }

  
  char* isSim = "This Is Not a Drill";
//  if (bme_init_result)
  {
    temperature = bme.readTemperature(); // Deg C
    humidity = bme.readHumidity();
    barPressure = bme.readPressure();
  }
//  else
//  {
//    isSim = "** Simulated **";
//    float tr = (float)random(100) / 100;
//    float hr = (float)random(100) / 100;
//    float pr = (float)random(100) / 100;
    
    
//  if (_unitsType == UNITS_TYPE_IMPERIAL) 
//  {
//    *temperature = *temperature * 1.8 + 32.0;
//    *barPressure = *barPressure * mbToInches;
//  }
//  
//  if (_oledEnabled) 
//  {
//    printf("---- Output to OLED ---- \r\n");
//    char dispLine2[8]; 
//    sprintf(dispLine2, "%.2f", bme.readTemperature());
//    display.writeLineAt(2, 7, dispLine2);
//
//    char dispLine3[8];
//    sprintf(dispLine3, "%.2f", bme.readHumidity());
//    display.writeLineAt(3, 7, dispLine3);
//  }
  printf("Temp=%.2f *F, Humi=%.2f %, Pres=%.2f in   %s\r\n\r\n",temperature, humidity, barPressure, isSim);
}
