// ==========================================================
//
// yaw.h
//
// ENCE361 Helicopter Project
//
// Authors:
//    - Zachary Avis | 27167186  | zav10@uclive.ac.nz
//
// Description:
//    Header file for yaw.c
//
// ==========================================================

#ifndef YAW_H_
#define YAW_H_

#include <stdint.h>

/* FUNCTIONS */
void initYaw(void);

void yawIntHandler(void);
void yawUpdateState(bool phaseA, bool phaseB);

int16_t getYawDeciDegrees(void);
int16_t getSlotCount(void);
bool isYawCalibrated(void);

#endif // YAW_H_
