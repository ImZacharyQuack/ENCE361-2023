// ==========================================================
//
// display.c
//
// ENCE361 Helicopter Project
//
// Authors:
//    - Zachary Avis | 27167186  | zav10@uclive.ac.nz
//
// Description:
//    Main file for program
//
// Info:
//  This module contains code from by P.J. Bones from
//  Lab3 ENCE361:
//      pwmGen.c - Example code which generates a single PWM
//          output on J4-05 (M0PWM7) with duty cycle fixed and
//          the frequency controlled by UP and DOWN buttons in
//          the range 50 Hz to 400 Hz.
//
// ==========================================================



/* INCLUDES */
#include <stdint.h>
#include <string.h>

#include "utils/ustdlib.h"
#include "OrbitOLED/OrbitOLEDInterface.h"

#include "src/display/display.h"
#include "src/altitude/altitude.h"
#include "src/yaw/yaw.h"
#include "src/motors/motors.h"
#include "src/flightState/flightState.h"



/* CONSTANTS */
#define MAX_STR_LEN 16

#define DISPLAY_SYMBOL_DEGREES 0x60



/* VARIABLES */
void displayClear(void);
void displayDataTest(void);

/* FUNCTIONS */
// Init display
void initDisplay(void) {
    OLEDInitialise();
    displayClear();
}

void displayClear(void) {

    OLEDStringDraw("                ", 0, 0);
    OLEDStringDraw("                ", 0, 1);
    OLEDStringDraw("                ", 0, 2);
    OLEDStringDraw("                ", 0, 3);

}

void displayEasterEgg(void) {

    OLEDStringDraw(" DUCK ", 0, 0);
    OLEDStringDraw(" DUCK ", 0, 1);
    OLEDStringDraw(" DUCK ", 0, 2);
    OLEDStringDraw(" GOOSE ", 0, 3);

}

void updateDisplay(void) {
    displayData();
   // displayDataTest();
}

void displayData(void) {

    // String
    char string[MAX_STR_LEN + 1];

    // Gets yaw decidegrees & converts it
    //
    // Using absolute to make numbers positive & using a minus sign
    // to show negative numbers since cannot display floats & -0 isn't
    // a thing.
    int16_t yawDeciDegrees = getYawDeciDegrees();
    int16_t yawDegrees = abs(yawDeciDegrees / 10);
    int16_t yawSubDegrees = abs(yawDeciDegrees % 10);

    char minusSign = yawDeciDegrees >= 0 ? ' ' : '-';
    // For styling
    char *spaces = yawDegrees > 9 ? yawDegrees > 99 ? " " : "  " : "   ";

    // Print to screen
    // Prints Main duty cycle from 0-100%
    usnprintf(string, sizeof(string), "Main Duty: %4d%%", getMainDutyCycle());
    OLEDStringDraw(string, 0, 0);

    // Prints Tail duty cycle from 0-100%
    usnprintf(string, sizeof(string), "Tail Duty: %4d%%", getTailDutyCycle());
    OLEDStringDraw(string, 0, 1);

    // Prints altitude from 0-100%
    usnprintf(string, sizeof(string), "Altitude:  %4d%%", getAltitudePercent());
    OLEDStringDraw(string, 0, 2);

    // Prints Yaw degrees to 1dp
    usnprintf(string, sizeof(string), "Yaw:    %s%c%d.%d%c", spaces, minusSign, yawDegrees, yawSubDegrees, DISPLAY_SYMBOL_DEGREES);
    OLEDStringDraw(string, 0, 3);

}


void displayDataTest(void) {

    char string[MAX_STR_LEN + 1];

    usnprintf(string, sizeof(string), "Main Duty: %4d%%", getMainDutyCycle());
    OLEDStringDraw(string, 0, 0);

    usnprintf(string, sizeof(string), "DA: %4d", getDesiredAltitude());
    OLEDStringDraw(string, 0, 1);

    usnprintf(string, sizeof(string), "Altitude:  %4d%%", getAltitudePercent());
    OLEDStringDraw(string, 0, 2);

}

void displayError(void) {
    OLEDStringDraw(" Display Error ", 0, 1);
    OLEDStringDraw(" You fucked up! ", 0, 2);
}














