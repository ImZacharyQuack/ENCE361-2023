// ==========================================================
//
// kernel.c
//
// ENCE361 Helicopter Project
//
// Authors:
//    - Zachary Avis | 27167186 | zav10@uclive.ac.nz
//    - fname lname  | 00000000 | aaa00@uclive.ac.nz
//
// Description:
//    Header file for kernel.c
//
// ==========================================================


#include <stdint.h>
#include <stdbool.h>

#ifndef SCHEDULER_H_
#define SCHEDULER_H_


/* FUNCTIONS */
// Initialises the kernel 
void initScheduler();
void initSchedulerSysTick(void);

void schedulerAddTask(void (*runTask)(void),  uint16_t frequency, int16_t priority);
void schedulerUpdateTicks(void);
void schedulerRun(void);

//void SysTickIntHandler(void);

#endif // SCHEDULER_H_


