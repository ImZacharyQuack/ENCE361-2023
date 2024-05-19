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



#ifndef FLIGHTSTATE_H_
#define FLIGHTSTATE_H_


enum flightStates { LANDED = 0, TAKING_OFF, FLYING, LANDING };
typedef enum flightStates FlightStates;


/* FUNCTIONS */
void initFlightState(void);

FlightStates getFlightState(void);
char* getFlightStateString(void);
void setFlightState(FlightStates state);
void updateFlightState(void);

#endif // FLIGHTSTATE_H_
