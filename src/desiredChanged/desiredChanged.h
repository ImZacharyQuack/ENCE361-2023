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
//    Header file for clock.c
//
// ==========================================================



#ifndef DESIREDCHANGE_H_
#define DESIREDCHANGE_H_



/* FUNCTIONS */
void initDesiredChange(void);

void desiredIncrementYaw(void);
void desiredDecrementYaw(void);
int16_t getDesiredYaw(void);
void setDesiredYaw(int16_t yaw);

void desiredIncrementAltitude(void);
void desiredDecrementAltitude(void);
uint8_t getDesiredAltitude(void);
void setDesiredAltitude(uint8_t alt);

#endif // DESIREDCHANGE_H_
