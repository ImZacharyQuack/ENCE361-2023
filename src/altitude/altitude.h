// ==========================================================
//
// altitude.h
//
// ENCE361 Helicopter Project
//
// Authors:
//    - Zachary Avis | 27167186  | zav10@uclive.ac.nz
//
// Description:
//    Header file for altitude.c
//
// ==========================================================

#ifndef ALTITUDE_H_
#define ALTITUDE_H_

#include <stdbool.h>

/* FUNCTIONS */
void initAltitude(void);

void updateAltitude(void);

int32_t getMeanADC(void);
int16_t getAltitudePercent(void);

void altitudeCalibrate(void);
void setAltitudeReference(void);

bool isBufferFull(void);
void processADC(void);

int32_t getRefADC(void);
bool isAltitudeCalibrated(void);

#endif // ALTITUDE_H_
