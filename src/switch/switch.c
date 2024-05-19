 // ==========================================================
//
// motors.c
//
// ENCE361 Helicopter Project
//
// Authors:
//    - Zachary Avis | 27167186 | zav10@uclive.ac.nz
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
#include <stdbool.h>

#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"

#include "switch.h"
#include "OrbitOLED/OrbitOLEDInterface.h"


/* CONSTANTS */
#define SWITCH_1_PERIPH SYSCTL_PERIPH_GPIOA
#define SWITCH_1_PORT_BASE GPIO_PORTA_BASE
#define SWITCH_1_PIN GPIO_PIN_7

static bool previousSwitchState[SWITCH_SW1];
static bool currentSwitchState[SWITCH_SW1];



// Init switch [SW1]
void initSwitch(void) {

    SysCtlPeripheralEnable(SWITCH_1_PERIPH);
    GPIOPinTypeGPIOInput(SWITCH_1_PORT_BASE, SWITCH_1_PIN);
    GPIOPadConfigSet(SWITCH_1_PORT_BASE, SWITCH_1_PIN, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPD);
    previousSwitchState[SWITCH_SW1] = SWITCH_UP;
    currentSwitchState[SWITCH_SW1] = SWITCH_UP;

}

void switchUpdate(void) {

    previousSwitchState[SWITCH_SW1] = currentSwitchState[SWITCH_SW1];
    currentSwitchState[SWITCH_SW1] = GPIOPinRead(SWITCH_1_PORT_BASE, SWITCH_1_PIN) == SWITCH_1_PIN;

}

switchState switchCheck(switchSlider switchType) {

    return currentSwitchState[switchType] ? SWITCH_UP : SWITCH_DOWN;

}

bool switchChanged(switchSlider switchType) {
    return previousSwitchState[switchType] != currentSwitchState[switchType];
}














