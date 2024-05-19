// ==========================================================
//
// motors.c
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
#include "driverlib/pin_map.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/pwm.h"

#include "motors.h"
#include "src/utils/utils.h"



/* CONSTANTS */
// PWM divider for a 40 Mhz clock
#define PWM_DIVIDER_CODE        SYSCTL_PWMDIV_8
#define PWM_DIVIDER             8

// PWM frequency for both motors
#define PWM_FREQ                250

// Main rotor => PC5 | J4-05
#define PWM_MAIN_BASE           PWM0_BASE
#define PWM_MAIN_GEN            PWM_GEN_3
#define PWM_MAIN_OUTNUM         PWM_OUT_7
#define PWM_MAIN_OUTBIT         PWM_OUT_7_BIT
#define PWM_MAIN_PERIPH_PWN     SYSCTL_PERIPH_PWM0
#define PWM_MAIN_PERIPH_GPIO    SYSCTL_PERIPH_GPIOC
#define PWM_MAIN_GPIO_BASE      GPIO_PORTC_BASE
#define PWM_MAIN_GPIO_CONFIG    GPIO_PC5_M0PWM7
#define PWM_MAIN_GPIO_PIN       GPIO_PIN_5

// Tail rotor => PF1 | J3-10
#define PWM_TAIL_BASE           PWM1_BASE
#define PWM_TAIL_GEN            PWM_GEN_2
#define PWM_TAIL_OUTNUM         PWM_OUT_5
#define PWM_TAIL_OUTBIT         PWM_OUT_5_BIT
#define PWM_TAIL_PERIPH_PWN     SYSCTL_PERIPH_PWM1
#define PWM_TAIL_PERIPH_GPIO    SYSCTL_PERIPH_GPIOF
#define PWM_TAIL_GPIO_BASE      GPIO_PORTF_BASE
#define PWM_TAIL_GPIO_CONFIG    GPIO_PF1_M1PWM5
#define PWM_TAIL_GPIO_PIN       GPIO_PIN_1



/* VARIABLES */
// Stores PWN period
static uint32_t pwm_period;

// Duty Cycle Variables
static uint8_t main_duty_cycle;
static uint8_t tail_duty_cycle;

void initMainMotor();
void initTailMotor();


/* FUNCTIONS */
void initMotors(void) {

    SysCtlPWMClockSet(PWM_DIVIDER_CODE);
    pwm_period = SysCtlClockGet() / (PWM_DIVIDER * PWM_FREQ);

    SysCtlPeripheralReset (PWM_MAIN_PERIPH_GPIO); // Used for PWM output
    SysCtlPeripheralReset (PWM_MAIN_PERIPH_PWN);

    initMainMotor();
    initTailMotor();

}

void initMainMotor(void) {
    // Init main rotor
    SysCtlPeripheralEnable(PWM_MAIN_PERIPH_PWN);
    SysCtlPeripheralEnable(PWM_MAIN_PERIPH_GPIO);
    GPIOPinConfigure(PWM_MAIN_GPIO_CONFIG);
    GPIOPinTypePWM(PWM_MAIN_GPIO_BASE, PWM_MAIN_GPIO_PIN);
    PWMGenConfigure(PWM_MAIN_BASE, PWM_MAIN_GEN, PWM_GEN_MODE_UP_DOWN | PWM_GEN_MODE_NO_SYNC);
    setMainDutyCycle(0);
    PWMGenEnable(PWM_MAIN_BASE, PWM_MAIN_GEN);
    stopMainMotor();
}

void initTailMotor(void) {
    // Init tail rotor
    SysCtlPeripheralEnable(PWM_TAIL_PERIPH_PWN);
    SysCtlPeripheralEnable(PWM_TAIL_PERIPH_GPIO);
    GPIOPinConfigure(PWM_TAIL_GPIO_CONFIG);
    GPIOPinTypePWM(PWM_TAIL_GPIO_BASE, PWM_TAIL_GPIO_PIN);
    PWMGenConfigure(PWM_TAIL_BASE, PWM_TAIL_GEN, PWM_GEN_MODE_UP_DOWN | PWM_GEN_MODE_NO_SYNC);
    setTailDutyCycle(0);
    PWMGenEnable(PWM_TAIL_BASE, PWM_TAIL_GEN);
    stopTailMotor();
}

//
void setMainDutyCycle(int8_t duty) {

    // Clamps duty variable between 0 & 100
    main_duty_cycle = clamp(duty, 0, 100);
    PWMGenPeriodSet(PWM_MAIN_BASE, PWM_MAIN_GEN, pwm_period);
    PWMPulseWidthSet(PWM_MAIN_BASE, PWM_MAIN_OUTNUM, pwm_period * main_duty_cycle / 100);

}

void setTailDutyCycle(int8_t duty) {

    // Clamps duty variable between 0 & 100
    tail_duty_cycle = clamp(duty, 0, 100);
    PWMGenPeriodSet(PWM_TAIL_BASE, PWM_TAIL_GEN, pwm_period);
    PWMPulseWidthSet(PWM_TAIL_BASE, PWM_TAIL_OUTNUM, pwm_period * tail_duty_cycle / 100);

}

//
void startMainMotor(void) {
    //setMainDutyCycle(0);
    PWMOutputState(PWM_MAIN_BASE, PWM_MAIN_OUTBIT, true);
}

void startTailMotor(void) {
    //setTailDutyCycle(0);
    PWMOutputState(PWM_TAIL_BASE, PWM_TAIL_OUTBIT, true);
}

//
void stopMainMotor(void) {
    PWMOutputState(PWM_MAIN_BASE, PWM_MAIN_OUTBIT, false);
}

void stopTailMotor(void) {
    PWMOutputState(PWM_TAIL_BASE, PWM_TAIL_OUTBIT, false);
}

//
uint8_t getMainDutyCycle(void) {
    return main_duty_cycle;
}

uint8_t getTailDutyCycle(void) {
    return tail_duty_cycle;
}

//
void increaseMainDutyCycle(int8_t increment) {
    setMainDutyCycle(getMainDutyCycle() + increment);
}

void decreaseMainDutyCycle(int8_t decrement) {
    setMainDutyCycle(getMainDutyCycle() - decrement);
}

//
void increaseTailDutyCycle(int8_t increment) {
    setTailDutyCycle(getTailDutyCycle() + increment);
}

void decreaseTailDutyCycle(int8_t decrement) {
    setTailDutyCycle(getTailDutyCycle() - decrement);
}














