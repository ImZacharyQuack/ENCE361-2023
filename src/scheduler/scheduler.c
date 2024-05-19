// ==========================================================
//
// circbufT.c
//
// ENCE361 Helicopter Project
//
// Authors:
//    - Zachary Avis | 27167186 | zav10@uclive.ac.nz
//    - fname lname  | 00000000 | aaa00@uclive.ac.nz
//
// Description:
//    This module contains functions for running tasks
//
// ==========================================================



/* INCLUDES */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "driverlib/interrupt.h"

#include "scheduler.h"
#include "src/button/button.h"



/* CONSTANTS */
// The frequency that the kernel runs at in Hz
// 40 kHz || Kernels runs at 1/1000 of clock frequency
#define SCHEDULER_FREQUENCY 40000

typedef struct {

    void (*runTask)(void);
    uint16_t frequency;
    uint16_t tickCount;
    uint8_t priority;
    bool ready;

} SchedulerTask;

// Array of tasks
static SchedulerTask* tasks;


// Max amount of tasks that can be scheduled
#define MAX_TASKS 16


/* VARIABLES */
// Total number of tasks stored
static uint8_t tasks_total;

// If memory allocation was good
static bool scheduler_init_success = false;

// Frequency that the kernel runs at in Hz

static volatile uint32_t systick_count;


/* FUNCTIONS */

void SysTickIntHandler(void) {
    systick_count++;
}

void initSchedulerSysTick(void) {

    SysTickPeriodSet(SysCtlClockGet() / SCHEDULER_FREQUENCY);

    SysTickIntRegister(SysTickIntHandler);

    SysTickIntEnable();
    SysTickEnable();
}

// Initialises the kernel
void initScheduler() {

    tasks_total = 0;
    systick_count = 0;

    tasks = malloc(sizeof(SchedulerTask) * MAX_TASKS);

    scheduler_init_success = tasks != NULL;

    initSchedulerSysTick();

}

void schedulerAddTask(void (*runTask)(void), uint16_t frequency, int16_t priority) {

    if (tasks_total >= MAX_TASKS) { //|| !scheduler_init_success) {
        return;
    }

    SchedulerTask* newTask = &tasks[tasks_total];
    newTask -> runTask = runTask;
    newTask -> frequency = frequency;
    newTask -> priority = priority;
    newTask -> tickCount = 0;

    tasks_total++;

}

void schedulerRun(void) {

    if (tasks_total <= 0) {
        return;
    }

    uint32_t freqTick, taskTickCount;
    uint32_t currentTickCount = systick_count;

    uint8_t i;
    for (i = 0; i < tasks_total; i++) {

        SchedulerTask* task = &tasks[i];

        taskTickCount = task -> tickCount;
        freqTick = currentTickCount - taskTickCount;
        if (currentTickCount < taskTickCount) {
            freqTick = UINT_MAX - taskTickCount + currentTickCount;
        }

        if ( ((float) freqTick / SCHEDULER_FREQUENCY) > (1.0f / task -> frequency) ) {

            task -> runTask();
            task -> tickCount = currentTickCount;

        }

    }

}
































