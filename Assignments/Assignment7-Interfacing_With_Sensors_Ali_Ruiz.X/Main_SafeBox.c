/*
 * ---------------------
 * Title: Interfacting With Sensors
 * ---------------------
 * Program Details:
 *  The purpose of this program is to make a a security system for a safebox
 * Inputs: 2 photoresistors act as touchless switches that increment values to input into program.
 * 2 buttons are also used with _interupt_button causing the interrupt function to run
 * and enter_val_button entering the values into the program for evaluation
 * 
 * 
 * Outputs: 
 * a Seven-Segment Display, a buzzer, and a motor are the outputs of this program.
 * The seven-segment is controlled by display_number(); and it outputs the real time input values
 * as well as a '7' when the wrong code is input.
 * 
 * The buzzer is triggered either for an elongaded beep for a wrong code input,
 * or a melody when the interrupt is triggered. 
 * 
 * The motor is triggered when the correct code is input and runs for a short time before turning off and resetting.
 * 
 * Setup: C- Simulator
 * Date: 14 April, 2025
 * File Dependencies / Libraries: It is required to include the 
 * Configuration Header File 
 * Compiler: xc8, 3.0
 * MPLAB X IDE v6.20
 * Author: Juan Ali Ruiz Guzman
 * Versions:
 *          V1.0 - 04/15/2025 - Beginning of Function definitions and code
 *          V1.1 - 04/15/2025 - Header Files complete with Initializations and Functions complete
 *          V1.2 - 04/16/2025 - Main file 'Main_SafeBox' created with main code written
 * `        V1.3 - 04/16/2025 - Fixed bug with codeinput() function which was not accepting values correctly
 *          V1.4 - 04/17/2025 - wrote code to debug motor function triggering the interrupt function
 *          V1.5 - 04/17/2025 - Final Code complete and commented out code was cleaned up and removed.
 * 
 * Future Optimizations: 
 * Add a reset to the number1 and number 2 values once the while loop completes in the main fie
 * Add a keypad to the circuit where the secret code can be changed through the keypad.
 * Add a second seven segment display to display which stage is active while the code runs
 * 
 */

// PIC18F46K42 Configuration Bit Settings

#include <xc.h>  // Must have this
#include "Peripherals_Initialize.h"
#include "PragmaConfig.h"
#include "Functions_.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

#define _XTAL_FREQ 4000000 // Fosc frequency for _delay() library
#define FCY (_XTAL_FREQ / 4)





void main (void){
//setup_keypadports;
setup_sevensegports();
_inputs_init();
_outputs_init();
INTERRUPT_Initialize();


//While loop to keep the program running
while(1){
    
while(state != 2){
    SYS_LED(); //call led turn on function
    codeinput(); //call the inputs from the user
    __delay_ms(300);
}
comparefunc(); // Compare the input code to the secret code
reset(); // reset the seven seg to zero
}

    
}


