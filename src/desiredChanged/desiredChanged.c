// ==========================================================
//
// control.c
//
// ENCE361 Helicopter Project
//
// Authors:
//    - Zachary Avis | 27167186 | zav10@uclive.ac.nz
//
// Description:
//    Main file for program
//
// ==========================================================



/* INCLUDES */
#include <stdint.h>

#include "desiredChanged.h"

#include "src/utils/utils.h"

/* CONSTANTS */
#define YAW_DECIDEGREES              150
#define ALTITUDE_PERCENT              10

#define DECIDEGREES_IN_CIRCLE       3600
#define MAX_YAW_DECIDEGREES      1800
#define MIN_YAW_DECIDEGREES       -1800

#define MAX_ALTITUDE                     100
#define MIN_ALTITUDE                      0


/* VARIABLES */
static int16_t desiredYaw;
static uint8_t desiredAltitude;



/* FUNCTIONS */
void initDesiredChange(void) {
    desiredYaw = 0;
    desiredAltitude = 0;
}

void desiredIncrementYaw(void) {

    desiredYaw += YAW_DECIDEGREES;
    if (desiredYaw > MAX_YAW_DECIDEGREES) {
        desiredYaw -= DECIDEGREES_IN_CIRCLE;
    }

}

void desiredDecrementYaw(void) {

    desiredYaw -= YAW_DECIDEGREES;
    if (desiredYaw <= MIN_YAW_DECIDEGREES) {
        desiredYaw += DECIDEGREES_IN_CIRCLE;
    }

}

int16_t getDesiredYaw(void) {
    return desiredYaw;
}

void setDesiredYaw(int16_t yaw) {
    desiredYaw = yaw;
}


void desiredIncrementAltitude(void) {

    desiredAltitude = min(desiredAltitude + ALTITUDE_PERCENT, 100);

}

void desiredDecrementAltitude(void) {

    desiredAltitude = max(desiredAltitude - ALTITUDE_PERCENT, 0);

}

uint8_t getDesiredAltitude(void) {
    return desiredAltitude;
}

void setDesiredAltitude(uint8_t alt) {
    desiredAltitude = alt;
}







































