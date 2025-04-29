/*
 * ---------------------
 * Title: ADC-Interfacing Assignment 8
 * ---------------------
 * Program Details:
 *  The purpose of this program is to interface analog peripherals and use interrupts
 * Inputs: Two Switches act as interupt buttons which trigger the voltage meter
 * and the delay program.
 * Another third switch moves the program out of the voltage meter and back into the 
 * microphone sensor readings.
 * The microphone sensor is another input that is is feeding analog voltage into the pic
 * the potentinometer is also another input that is used to put voltage into the pic
 * for the voltage meter.

 * 
 * Outputs: 
 * A 16x2 LCD display is is the output of the program as well as an LED, and a Buzzer.
 * The 16x2 LCD displays all the results of the program
 * the LED blinks 10 times when the delay is activated
 * The buzzer plays notes when buttons are pressed and a melody when delay is triggered.
 * 
 * Setup: C- Simulator
 * Date: 26 April, 2025
 * File Dependencies / Libraries: It is required to include the 
 * Configuration Header File 
 * Compiler: xc8, 3.0
 * MPLAB X IDE v6.20
 * Author: Juan Ali Ruiz Guzman
 * Versions:
 *          V1.0 - 04/26/2025 - Beginning of Function definitions and code
 *          V1.1 - 04/26/2025 - Header Files complete with Initializations and Functions complete
 *          V1.2 - 04/27/2025 - Main file 'ADC_Main.c' created with main code written
 * `        V1.3 - 04/28/2025 - Fixed bug with delay interrupt where it was getting stuck at timer_func();
 *          V1.4 - 04/28/2025 - wrote code to debug interrupt functions
 *          V1.5 - 04/28/2025 - Final Code complete and commented out code was cleaned up and removed.
 * 
 * Future Optimizations: 
 * Add other peripherals or do something with the readings taken from the microphone sensor
 * 
 */

// PIC18F46K42 Configuration Bit Settings



#include <xc.h>  // Must have this
#include "PragmaConfig.h"
#include "Initialize.h"
#include "Functions.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pic18f47k42.h>


#define _XTAL_FREQ 4000000                 // Fosc  frequency for _delay()  library
#define FCY    _XTAL_FREQ/4





/*****************************Main Program*******************************/
void main(void)
{     
    PORTS_Init();  //initialize the ports used
    ADC_Init(); // initialize the ADC 
    LCD_Init();                    /* Initialize 16x2 LCD */
    INTERRUPT_Initialize(); // initialize the interrupt bits and flags
    LCD_Clear(); //clear the LCD 
    while(1){
    ADC_RA2Conver(); //grab readings from microphone sensor
    }
}