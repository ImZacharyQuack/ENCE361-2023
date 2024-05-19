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
//    Header file for motors.c
//
// ==========================================================



#ifndef MOTORS_H_
#define MOTORS_H_

/* FUNCTIONS */
// Initialises the clock 
void initMotors(void);

void setMainDutyCycle(int8_t duty);
void setTailDutyCycle(int8_t duty);

void startMainMotor(void);
void startTailMotor(void);

void stopMainMotor(void);
void stopTailMotor(void);

uint8_t  getMainDutyCycle(void);
uint8_t  getTailDutyCycle(void);

void increaseTailDutyCycle(int8_t increment);
void decreaseTailDutyCycle(int8_t decrement);



#endif // MOTORS_H_
