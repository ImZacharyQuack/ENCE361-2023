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
#include "src/motors/motors.h"
#include "src/yaw/yaw.h"
#include "src/switch/switch.h"
#include "src/flightState/flightState.h"
#include "src/desiredChanged/desiredChanged.h"
#include "src/display/display.h"


#include "input.h"


/* FUNCTIONS */
void initInput(void) {

    initButton();
    initSwitch();

}

void inputUpdate(void) {

    buttonUpdate();
    switchUpdate();

    // ALTITUDE
    if (getFlightState() == FLYING) {

        // Ascend
        if (buttonCheck(UP) == PUSHED) {
            //increaseMainDutyCycle(10);
            desiredIncrementAltitude();
        }

        // Descend
        if (buttonCheck(DOWN) == PUSHED) {
            desiredDecrementAltitude();
        }

    }

    // YAW
    if (getFlightState() == FLYING) {

        // Turn CCW
        if (buttonCheck(LEFT) == PUSHED) {
            desiredDecrementYaw();
        }

        // Turn CW
        if (buttonCheck(RIGHT) == PUSHED) {
            desiredIncrementYaw();
        }

    }

   // SWITCH
   if (switchCheck(SWITCH_SW1) == SWITCH_UP) {

       if (getFlightState() == LANDED) {
           setFlightState(FLYING);
       }

   }

   if (switchCheck(SWITCH_SW1) == SWITCH_DOWN) {

       if (getFlightState() == FLYING) {
           setFlightState(LANDED);
       }

   }

}











