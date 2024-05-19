// ==========================================================
//
// control.c
//
// ENCE361 Helicopter Project
//
// Authors:
//    - Zachary Avis | 27167186 | zav10@uclive.ac.nz
//
// Description:
//    Main file for program
//
// ==========================================================



/* INCLUDES */
#include <stdint.h>

#include "control.h"

#include "src/utils/utils.h"
#include "src/altitude/altitude.h"
#include "src/yaw/yaw.h"
#include "src/desiredChanged/desiredChanged.h"
#include "src/motors/motors.h"



/* CONSTANTS */
#define IDLE_MAIN_DUTY 10
#define MAX_MAIN_DUTY 98
#define MIN_MAIN_DUTY 2

#define MAX_TAIL_DUTY 98
#define MIN_TAIL_DUTY 2

// Numbers are times by GAIN_MULTIPLIER to get high precision while also avoiding using
// floats

// Storing PID gains as 100 times their actual values to avoid floating points & for more precision
// when converting

static ControlGain controlGainAtltitude;
static ControlGain controlGainYaw;

/* VARIABLES */

static uint16_t pwm;

static bool enableAltitudeControl;
static bool enableYawControl;

/* FUNCTIONS */
void controlUpdateAltitude(void);
void controlUpdateYaw(void);

void initControl(void) {

    // 0.65f, 0.012f, 0.8f
    // 0.8f, 0.009f, 0.8f
    controlGainAtltitude = (ControlGain) { 0.65f, 0.012f, 0.8f, 0, 0 };
    controlGainYaw =  (ControlGain) { 0.5f, 0.01f, 0.7f, 0, 0 };

    controlEnableYaw(true);
    controlEnableAltitude(true);

}


void updateControl(void) {
    controlUpdateAltitude();
    controlUpdateYaw();
}

void controlUpdateAltitude(void) {

    if (!enableAltitudeControl) {
        return;
    }

    int16_t mainDutyCycle;
    float P_alt, dI_alt, D_alt;

    // Difference between what the desired altitude is & what the current altitude is
    int16_t error_alt = getDesiredAltitude() - getAltitudePercent();

    // Control is called at regular interevals so can ignore deltaT
    P_alt = controlGainAtltitude.Kp * error_alt;
    if (controlGainAtltitude.duty > 10 && controlGainAtltitude.duty < 90) {
        controlGainAtltitude.accumulativeError += error_alt;
    }
    dI_alt = controlGainAtltitude.Ki * controlGainAtltitude.accumulativeError;
    D_alt = (controlGainAtltitude.prevError - error_alt) * controlGainAtltitude.Kd;

    mainDutyCycle = P_alt + dI_alt + D_alt + 10;
    mainDutyCycle = clamp(mainDutyCycle, MIN_MAIN_DUTY, MAX_MAIN_DUTY);

    controlGainAtltitude.prevError = error_alt;

    controlGainAtltitude.duty = mainDutyCycle;
    setMainDutyCycle(mainDutyCycle);

}

void controlUpdateYaw(void) {

    if (!enableYawControl) {
        return;
    }

    int8_t tailDutyCycle;
    uint32_t P_yaw, dI_yaw, D_yaw;
    //bool clockwise = true;

    // Difference between what the desired yaw is & what the current yaw is
    // Internally "decidegrees" are used which is just 360 * 10
    int16_t error_yaw = (getDesiredYaw() - getYawDeciDegrees()) / 10;

    P_yaw = error_yaw * controlGainYaw.Kp;
    if (controlGainYaw.duty > 10 && controlGainYaw.duty < 90) {
        controlGainAtltitude.accumulativeError += error_yaw;
    }
    dI_yaw = controlGainYaw.accumulativeError + controlGainYaw.Ki * error_yaw
            ;
    D_yaw = (controlGainYaw.prevError - error_yaw) * controlGainYaw.Kd;
    controlGainYaw.prevError = error_yaw;

    tailDutyCycle = P_yaw + dI_yaw + D_yaw;
    tailDutyCycle = clamp(tailDutyCycle, MIN_TAIL_DUTY, MAX_TAIL_DUTY);

    controlGainYaw.duty = tailDutyCycle;
    setTailDutyCycle(tailDutyCycle);

}

void controlEnableAltitude(bool enable) {

    enableAltitudeControl = enable;

    if (!enableAltitudeControl) {

           controlGainAtltitude.duty = 0;
           controlGainAtltitude.prevError = 0;
           controlGainAtltitude.accumulativeError = 0;
           setMainDutyCycle(0);

       } else {

           setMainDutyCycle(controlGainAtltitude.duty);

       }

}

void controlEnableYaw(bool enable) {

    enableYawControl = enable;

    if (!enableYawControl) {

        controlGainYaw.duty = 0;
        controlGainYaw.prevError = 0;
        controlGainYaw.accumulativeError = 0;
        setTailDutyCycle(0);

    } else {

//        setTailDutyCycle(controlGainYaw.duty);
        setTailDutyCycle(5);

    }

}



uint16_t getPWM(void) {
    return pwm;
}

































