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
//    This module contains the clock initialisation function
//    for setting the system clock to run at 40 MHz
//
// ==========================================================

#include <stdint.h>
#include <stdbool.h>

#include "clock.h"

#include "driverlib/sysctl.h"

void initClock(void) {

    // Sets clock to 40 MHz
    SysCtlClockSet(SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);

}
