// ==========================================================
//
// clock.c
//
// ENCE361 Helicopter Project
//
// Authors:
//    - Zachary Avis | 27167186 | zav10@uclive.ac.nz
//    - fname lname  | 00000000 | aaa00@uclive.ac.nz
//
// Description:
//    Header file for clock.c
//
// ==========================================================

#include "stdbool.h"

#ifndef CONTROL_H_
#define CONTROL_H_

struct controlGain {
    float Kp;
    float Ki;
    float Kd;
    uint8_t duty;
    uint16_t prevError;
    uint16_t accumulativeError;
};
typedef struct controlGain ControlGain;

/* FUNCTIONS */
void initControl(void);
void updateControl(void);

void controlEnableAltitude(bool enable);
void controlEnableYaw(bool enable);

uint16_t getPWM(void);

#endif // CONTROL_H_
