// ==========================================================
//
// uart.c
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
//
// ==========================================================



/* INCLUDES */
#include <stdint.h>
#include <stdbool.h>

#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"

#include "utils/ustdlib.h"



/* FUNCTIONS */
int32_t max(int32_t a, int32_t b) {
    return a > b ? a : b;
}

int32_t min (int32_t a, int32_t b) {
    return a < b ? a : b;
}

int32_t clamp(int32_t number, int32_t minNumber, int32_t maxNumber) {
    return max(min(number, maxNumber), minNumber);
}
















