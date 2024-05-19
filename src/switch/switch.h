// ==========================================================
//
// altitude.h
//
// ENCE361 Helicopter Project
//
// Authors:
//    - Zachary Avis | 27167186  | zav10@uclive.ac.nz
//
// Description:
//    Header file for altitude.c
//
// ==========================================================

#ifndef SWITCH_H_
#define SWITCH_H_

enum switch_state { SWITCH_DOWN, SWITCH_UP };
typedef enum switch_state switchState;

enum switch_slider { SWITCH_SW1 = 0 };
typedef enum switch_slider switchSlider;

void initSwitch(void);
void switchUpdate(void);

switchState switchCheck(switchSlider switchType);
bool switchChanged(switchSlider switchType);

#endif // SWITCH_H_
