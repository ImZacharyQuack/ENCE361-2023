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
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "driverlib/interrupt.h"
#include "driverlib/debug.h"

#include "utils/ustdlib.h"


#include "src/circBufT/circBufT.h"
#include "altitude.h"


/* CONSTANTS */
// BUFFER Size of 16 -> minimize the time it takes to get & average the sample value
#define BUFFER_SIZE             16

#define ALTITUDE_ADC_PERIPH       SYSCTL_PERIPH_ADC0
#define ALTITUDE_ADC_BASE           ADC0_BASE
#define ALTITUDE_ADC_CHANNEL    ADC_CTL_CH0


/* VARIABLES */
static circBuf_t inBuffer;

static uint32_t numberOfSamplesTaken;

static int16_t meanADC;
static int32_t sumADC;
static int16_t referenceADC;

bool refDone = false;

/* FUNCTIONS */
void ADCIntHandler(void) {

    uint32_t value;

    // Get the new sample from the ADC module.
    ADCSequenceDataGet(ALTITUDE_ADC_BASE, 3, &value);
    numberOfSamplesTaken++;

    // Write the new value to the circular buffer.
    writeCircBuf(&inBuffer, value);

    // Clean up, clearing the interrupt.
    ADCIntClear(ALTITUDE_ADC_BASE, 3);
}

// Init function
void initADC(void) {
    SysCtlPeripheralEnable(ALTITUDE_ADC_PERIPH);
    ADCSequenceConfigure(ALTITUDE_ADC_BASE, 3, ADC_TRIGGER_PROCESSOR, 0);
    ADCSequenceStepConfigure(ALTITUDE_ADC_BASE, 3,  0, ALTITUDE_ADC_CHANNEL | ADC_CTL_IE | ADC_CTL_END);
    ADCSequenceEnable(ALTITUDE_ADC_BASE, 3);
    ADCIntRegister(ALTITUDE_ADC_BASE, 3, ADCIntHandler);
    ADCIntEnable(ALTITUDE_ADC_BASE, 3);
}

void initAltitude(void) {

    numberOfSamplesTaken = 0;

    initCircBuf(&inBuffer, BUFFER_SIZE);

    initADC();
}

// Called at 512 Hz

void updateAltitude(void) {

    int16_t i;

    // Loops through buffer & adds all the entries
    sumADC = 0;
    for (i = 0; i < BUFFER_SIZE; i++) {
        sumADC = sumADC + readCircBuf(&inBuffer);
    }

    // Gets sample average
    meanADC = (2 * sumADC + BUFFER_SIZE) / (2 * BUFFER_SIZE);

    // Checks if the reference adc has been set, but also checks if the buffer has been fulled with
    // samples before setting
    if (isBufferFull() && !refDone) {
        setAltitudeReference();
        refDone = true;
    }

}

void processADC(void) {
    ADCProcessorTrigger(ALTITUDE_ADC_BASE, 3);
}

int32_t getMeanADC(void) {
    return meanADC;
}

int16_t getAltitudePercent(void) {
    // The helicopter rig will drop 1V as it goes from landed position to 100% flying position
    // 4095 * 1V / 3.3V = ~1241
    return (referenceADC - meanADC) * 100 / 1241;
}

void altitudeCalibrate(void) {
    setAltitudeReference();
    refDone = true;
}

void setAltitudeReference(void) {
    referenceADC = meanADC;
}

bool isBufferFull(void) {
    return numberOfSamplesTaken > BUFFER_SIZE;
}

int32_t getRefADC(void) {
    return referenceADC;
}

bool isAltitudeCalibrated(void) {
    return refDone;
}








