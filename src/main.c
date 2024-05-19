// ==========================================================
//
// main.c
//
// ENCE361 Helicopter Project
//
// Authors:
//    - Zachary Avis | 27167186 | zav10@uclive.ac.nz
//
// Description:m
//    Main file for program
//
// ==========================================================



/* INCLUDES */
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "src/clock/clock.h"
#include "src/motors/motors.h"
#include "src/utils/utils.h"
#include "src/display/display.h"
#include "src/input/input.h"
#include "src/altitude/altitude.h"
#include "src/utils/utils.h"
#include "src/scheduler/scheduler.h"
#include "src/utils/utils.h"
#include "src/yaw/yaw.h"
#include "src/switch/switch.h"
#include "src/flightState/flightState.h"
#include "src/desiredChanged/desiredChanged.h"
#include "src/control/control.h"
#include "src/motors/motors.h"
#include "src/uart/uart.h"

#include "inc/hw_memmap.h"
#include "inc/tm4c123gh6pm.h"

#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"



/* VARIABLES */ 
#define DUCKS_ARE_THE_BEST true


/* FUNCTIONS */
// Initialises all 
void initialise(void) {

    IntMasterDisable();

    initClock();
    initAltitude();
    initYaw();
    initMotors();
    initControl();
    initDesiredChange();
    initInput();
    initFlightState();
    initDisplay();
    initUART();

    startMainMotor();
    startTailMotor();

    initScheduler();
    // Add tasks to scheduler
    schedulerAddTask(processADC, 512, 1);
    schedulerAddTask(updateAltitude, 512, 1);
    schedulerAddTask(updateControl, 30, 5);
    schedulerAddTask(inputUpdate, 0, 10);
    schedulerAddTask(updateFlightState, 15, 20);
    schedulerAddTask(updateDisplay, 1, 100);
    schedulerAddTask(updateUARTFlightData, 4, 100);

    IntMasterEnable ();

    //displayData();

}

// Main functions
// Starts program
int main(void) {

    initialise();

    while (DUCKS_ARE_THE_BEST) {

        schedulerRun();

     }

}
