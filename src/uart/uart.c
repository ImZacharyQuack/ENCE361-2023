// ==========================================================
//
// uart.c
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
//
// ==========================================================



/* INCLUDES */
#include <stdint.h>
#include <stdbool.h>

#include "stdio.h"
#include "stdlib.h"

#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"

#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "driverlib/uart.h"
#include "driverlib/pin_map.h"

#include "utils/ustdlib.h"


/* DEFINES */
#define STR_LEN 16

#define BAUD_RATE 9600

#define UART_BASE UART0_BASE
#define UART_PERIPH_UART SYSCTL_PERIPH_UART0
#define UART_PERIPH_GPIO SYSCTL_PERIPH_GPIOA
#define UART_GPIO_BASE GPIO_PORTA_BASE
#define UART_GPIO_PIN_RX GPIO_PIN_0
#define UART_GPIO_PIN_TX GPIO_PIN_1
#define UART_GPIO_PINS UART_GPIO_PIN_RX | UART_GPIO_PIN_TX

#define UART_WORD_LEN UART_CONFIG_WLEN_8
#define UART_STOP_BIT UART_CONFIG_STOP_ONE
#define UART_PAR_BIT UART_CONFIG_PAR_NONE
#define UART_CONFIG UART_WORD_LEN | UART_STOP_BIT | UART_PAR_BIT



/* FUNCTIONS */
void initUART(void) {

    SysCtlPeripheralEnable(UART_PERIPH_UART);
    SysCtlPeripheralEnable(UART_PERIPH_GPIO);

    GPIOPinTypeUART(UART_GPIO_BASE, UART_GPIO_PINS);
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);

    UARTConfigSetExpClk(UART_BASE, SysCtlClockGet(), BAUD_RATE, UART_CONFIG);

    UARTFIFOEnable(UART_BASE);
    UARTEnable(UART_BASE);

}

void uartSend(char *buffer) {

    while (*buffer) {

        UARTCharPut(UART_BASE, *buffer);
        buffer++;

    }

}

void updateUARTFlightData(void) {

    char string[STR_LEN + 1];

    // Altitude
    usnprintf(string, sizeof(string), "Alt: %4d%% [%4d%%]\r\n", getAltitudePercent(), getDesiredAltitude());
    uartSend(string);

    // Yaw
    usnprintf(string, sizeof(string), "Yaw: %4d [%4d]\r\n", getYawDeciDegrees(), getDesiredYaw());
    uartSend(string);

    // Main Duty Cycle
    usnprintf(string, sizeof(string), "Main: %4d%%\r\n", getMainDutyCycle());
    uartSend(string);

    // Tail Duty Cycle
    usnprintf(string, sizeof(string), "Tail: %4d%%\r\n", getTailDutyCycle());
    uartSend(string);

    // Flight State
    usnprintf(string, sizeof(string), "Mode: %16s\r\n", getFlightStateString());
    uartSend(string);

}















