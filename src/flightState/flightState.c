// ==========================================================
//
// input.c
//
// ENCE361 Helicopter Project
//
// Authors:
//    - Zachary Avis | 27167186 | zav10@uclive.ac.nz
//    - fname lname  | 00000000 | aaa00@uclive.ac.nz
//
// Description:
//    Main file for program
//
// ==========================================================



/* INCLUDES */
#include <stdint.h>

#include "stdlib.h"

#include "src/button/button.h"
#include "src/circBufT/circBufT.h"
#include "src/motors/motors.h"
#include "src/scheduler/scheduler.h"
#include "src/yaw/yaw.h"
#include "src/switch/switch.h"
#include "src/display/display.h"
#include "src/altitude/altitude.h"
#include "src/control/control.h"
#include "src/desiredChanged/desiredChanged.h"

#include "flightState.h"

#include "utils/ustdlib.h"
#include "OrbitOLED/OrbitOLEDInterface.h"


/* CONSTANT */
#define TAIL_REF_DUTY 10
#define ALT_HOVER_PERC 10


/* VARIABLES */
static FlightStates flightState;



/* FUNCTIONS */
void initFlightState(void) {
    flightState = LANDED;
}

FlightStates getFlightState(void) {
    return flightState;
}

void setFlightState(FlightStates state) {
    flightState = state;
}

char* getFlightStateString(void) {

    switch (flightState) {
        case LANDED: return "Landed";
        case TAKING_OFF: return "Taking Off";
        case FLYING: return "Flying";
        case LANDING: return "Landing";
        default: return "";
    }

}

void updateFlightState(void) {

    int16_t settledYaw = getYawDeciDegrees();
    int16_t settledAltitude = getAltitudePercent();

    switch(flightState) {

        case TAKING_OFF:

            if (isYawCalibrated() && isAltitudeCalibrated()) {

                setFlightState(FLYING);
                controlEnableAltitude(true);
                controlEnableYaw(true);

            // Alt & Yaw ref not done
            } else {

                setMainDutyCycle(0);
                setTailDutyCycle(TAIL_REF_DUTY);

                if (isBufferFull()) {
                    altitudeCalibrate();
                    // Yaw calibrate done by interrupt
                }

            }

            break;

        case LANDING:

            if (settledYaw <= 20 && settledYaw >= -20) {

                if (settledAltitude <= 2 && settledAltitude >= -2) {

                    controlEnableAltitude(false);
                    controlEnableYaw(false);

                    setFlightState(LANDED);

                } else {

                    if (settledAltitude == ALT_HOVER_PERC) {
                        setDesiredAltitude(0);
                    } else if (getDesiredAltitude() != 0) {
                        setDesiredAltitude(ALT_HOVER_PERC);
                    }

                }

            } else {
                setDesiredYaw(0);
            }

            break;

    }

}











