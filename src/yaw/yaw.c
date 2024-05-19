// ==========================================================
//
// altitude.c
//
// ENCE361 Helicopter Project
//
// Authors:
//    - Zachary Avis | 27167186  | zav10@uclive.ac.nz
//
// Description:
//      This module contains functions to check & sample the adc & average it, convert
//      it to a percent
//
// ==========================================================



/* INCLUDES */
#include <stdint.h>
#include <stdbool.h>

#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/adc.h"
#include "driverlib/pwm.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "driverlib/interrupt.h"
#include "driverlib/debug.h"
#include "utils/ustdlib.h"

#include "src/circBufT/circBufT.h"
#include "src/altitude/altitude.h"


/* CONSTANTS */
#define SLOTS 112
#define PHASES 4
// 112 slots over 4 phases is 448
#define MAX_SLOT_COUNT 448
#define DEGREES_IN_CRICLE 360
#define DECIDEGREES_IN_CIRCLE DEGREES_IN_CRICLE * 10 // 360 * 10

// Phase A pin => PB0
// Phase B pin => PB1
#define YAW_QUAD_PERIPH SYSCTL_PERIPH_GPIOB
#define YAW_QUAD_BASE GPIO_PORTB_BASE
#define YAW_QUAD_INT_PIN_1 GPIO_INT_PIN_0
#define YAW_QUAD_INT_PIN_2 GPIO_INT_PIN_1
#define YAW_QUAD_PIN_1 GPIO_PIN_0
#define YAW_QUAD_PIN_2 GPIO_PIN_1
#define YAW_QUAD_PIN_TYPE GPIO_PIN_TYPE_STD_WPD
#define YAW_QUAD_STRENGTH GPIO_STRENGTH_2MA
#define YAW_QUAD_EDGE_TYPE GPIO_BOTH_EDGES
#define YAW_QUAD_DDR GPIO_DIR_MODE_IN

// REFERENCE PIN => PC4
#define YAW_REF_PERIPH SYSCTL_PERIPH_GPIOC
#define YAW_REF_BASE GPIO_PORTC_BASE
#define YAW_REF_INT_PIN GPIO_INT_PIN_4
#define YAW_REF_PIN GPIO_PIN_4
#define YAW_REF_STRENGTH GPIO_STRENGTH_2MA
#define YAW_REF_PIN_TYPE GPIO_PIN_TYPE_STD_WPU
#define YAW_REF_EDGE_TYPE GPIO_RISING_EDGE

enum quadrature_state {QUAD_STATE_CW, QUAD_STATE_CCW, QUAD_STATE_NC, QUAD_STATE_INVALID};
typedef enum quadrature_state QuadState;



/* VARIABLES */
static volatile QuadState quadState;
static volatile int16_t slot_count;

static uint8_t previousState;
static int8_t count;

static volatile bool yawCalibrated;



/* FUNCTIONS */
// Prototype functions
void yawIntHandler(void);
void yawRefIntHandler(void);
void yawUpdateState(bool phaseA, bool phaseB);

void initYaw(void) {

    quadState = QUAD_STATE_NC;
    previousState = 0;
    slot_count = 0;
    count = 0;
    yawCalibrated = false;

    // Setup the pins A & B
    SysCtlPeripheralEnable(YAW_QUAD_PERIPH);

    // Disable interrupts for safety
    GPIOIntDisable(YAW_QUAD_BASE, YAW_QUAD_INT_PIN_1 | YAW_QUAD_INT_PIN_2);

    // Set the pins as input
    GPIOPinTypeGPIOInput(YAW_QUAD_BASE, YAW_QUAD_PIN_1 | YAW_QUAD_PIN_2);

    // Set the GPIO pins to weak pull down, 2mA
    GPIOPadConfigSet(YAW_QUAD_BASE, YAW_QUAD_PIN_1 | YAW_QUAD_PIN_2, YAW_QUAD_STRENGTH, YAW_QUAD_PIN_TYPE);

    // Set the GPIO pins to generate interrupts on both rising & falling edges
    GPIOIntTypeSet(YAW_QUAD_BASE, YAW_QUAD_PIN_1 | YAW_QUAD_PIN_2, YAW_QUAD_EDGE_TYPE);

    // Register the interrupt handler
    GPIOIntRegister(YAW_QUAD_BASE, yawIntHandler);

    // Enable interrupts
    GPIOIntEnable(YAW_QUAD_BASE, YAW_QUAD_INT_PIN_1 | YAW_QUAD_INT_PIN_2);

    // Yaw Ref
    SysCtlPeripheralEnable(YAW_REF_PERIPH);
    GPIOIntDisable(YAW_REF_BASE, YAW_REF_INT_PIN);
    GPIOPinTypeGPIOInput(YAW_REF_BASE, YAW_REF_PIN);
    GPIOPadConfigSet(YAW_REF_BASE, YAW_REF_PIN, YAW_REF_STRENGTH, YAW_REF_PIN_TYPE);
    GPIOIntTypeSet(YAW_REF_BASE, YAW_REF_PIN, YAW_REF_EDGE_TYPE);
    GPIOIntRegister(YAW_REF_BASE, yawRefIntHandler);
    GPIOIntEnable(YAW_REF_BASE, YAW_REF_INT_PIN);

}

void yawIntHandler(void) {

    // Clear flags
    GPIOIntClear(YAW_QUAD_BASE, YAW_QUAD_INT_PIN_1 | YAW_QUAD_INT_PIN_2);

    if (yawCalibrated) {

        // Pin readouts
        bool phaseA = GPIOPinRead(YAW_QUAD_BASE, YAW_QUAD_PIN_1);
        bool phaseB = GPIOPinRead(YAW_QUAD_BASE, YAW_QUAD_PIN_2);

        // Update yaw state
        yawUpdateState(phaseA, phaseB);
    }

}

void yawRefIntHandler(void) {

    GPIOIntClear(YAW_REF_BASE, YAW_REF_INT_PIN);

    if (!yawCalibrated) {

        slot_count = 0;
        yawCalibrated = true;

    }

}

void yawUpdateState(bool phaseA, bool phaseB) {

    /* Depending on the direction of rotation you will get two outcomes
     *
     *  Phase B leads Phase A
     *
     * 0, 1, 3, 2  => CCW
     *   or
     * 0, 2, 3, 1  => CW
     *
     * */

    // Quadrature Array
    int8_t QuadArray[16] = {0, -1, 1, 2,
                                        1, 0, 2, -1,
                                        -1, 2, 0, 1,
                                        2, 1, -1, 0};

    // Get current state value between 0 & 3
    uint8_t currentState = (phaseA << 1) | phaseB;

    // If current state is the same as previous return
    if (currentState == previousState) {
        quadState = QUAD_STATE_NC;
        return;
    }

    // Checks whether the yaw if changing direction in CW or CCW
    int8_t direction = QuadArray[previousState * 4 + currentState];
    switch (direction) {
        // CW
        case -1:
            quadState = QUAD_STATE_CW;
            if (--slot_count < -MAX_SLOT_COUNT + 1) {
                slot_count = -MAX_SLOT_COUNT;
            }
            break;
        // CCW
        case 1:
            quadState = QUAD_STATE_CCW;
            if (++slot_count > MAX_SLOT_COUNT - 1) {
                slot_count = 0;
            }

            break;
        // INVALID
        case 2:
            quadState = QUAD_STATE_INVALID;
            break;
    }

    previousState = currentState;

}

int16_t getYawDeciDegrees(void) {

    int16_t yawDeciDegrees = (slot_count * DECIDEGREES_IN_CIRCLE) / MAX_SLOT_COUNT;

    if (yawDeciDegrees < -DECIDEGREES_IN_CIRCLE / 2) {
        yawDeciDegrees += DECIDEGREES_IN_CIRCLE;
    } else if (yawDeciDegrees >= DECIDEGREES_IN_CIRCLE / 2) {
        yawDeciDegrees -= DECIDEGREES_IN_CIRCLE;
    }

    return yawDeciDegrees;
}

int16_t getSlotCount(void) {
    return slot_count;
}

uint8_t returnPhase(void) {
    return count;
}

bool isYawCalibrated(void) {
    return yawCalibrated;
}







