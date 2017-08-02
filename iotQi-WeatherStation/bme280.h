// Copyright (c) Microsoft. All rights reserved.
// Copyright (c) LooUQ Incorporated. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#ifndef __BME280_H
#define __BME280_H

#define UNITS_TYPE_METRIC        (0) 
#define UNITS_TYPE_IMPERIAL      (1)

extern float temperature;
extern float humidity;
extern float barPressure;

#ifdef __cplusplus
extern "C" {
#endif

void initBme(int unitsType);
void readBmeValues();


#ifdef __cplusplus
}
#endif


#endif//__BME280_H
