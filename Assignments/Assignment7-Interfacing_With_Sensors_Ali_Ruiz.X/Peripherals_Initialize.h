/*
 Header File for Main_SafeBox.c containing functions
 * for the initialization of ports for peripherals
 * 
 * Author: Juan Ali Ruiz Guzman
 * 
 * 
 
 
 
 */
// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef PERIPHERALS_INITIALIZE_H
#define	PERIPHERALS_INITIALIZE_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

#define _XTAL_FREQ 4000000 // Fosc frequency for _delay() library
#define FCY (_XTAL_FREQ / 4)

//Inputs Definitions
#define photores1 PORTAbits.RA2
#define photores2 PORTAbits.RA3
#define enter_val_button PORTAbits.RA4
#define _interupt_button LATBbits.LATB0


//Outputs Definitions
#define sysled PORTCbits.RC2
#define motor PORTCbits.RC5
#define _buzzer PORTCbits.RC4



//Function Declarations:
void setup_sevensegports(void);
void _inputs_init(void);
void _outputs_init(void);
void INTERRUPT_Initialize(void);


// External Constant Maps:
extern const char segMap[10];

void _inputs_init(){
    TRISA = 0b00111100;
    LATA = 0x00;
    ANSELA = 0x00;
    
    TRISB = 0b00000001;
    LATB = 0x00;
    ANSELB = 0x00;
}

void _outputs_init(){
    TRISC = 0x00;
    LATC = 0x00;
    ANSELC = 0x00;
    PORTC = 0x00;
}


void setup_sevensegports() {
    TRISD = 0x00; // All PORTD for first 7-segment
    LATD = segMap[0];
    PORTD = segMap[0];
    ANSELD = 0x00;
    

}
void INTERRUPT_Initialize (void)
{
    INTCON0bits.GIEH = 1; // Enable high priority interrupts
    INTCON0bits.GIEL = 1; // Enable low priority interrupts
    INTCON0bits.IPEN = 1; // Enable interrupt priority

    INTCON0bits.INT0EDG=1; //rising edge
    IPR1bits.INT0IP=1;
    PIE1bits.INT0IE=1;

    PIR1bits.INT0IF=0;
    // Change IVTBASE if required
    IVTBASEU = 0x00; // Optional
    IVTBASEH = 0x40; // Default is 0x0008
    IVTBASEL = 0x08;
}



#endif	/* PERIPHERALS_INITIALIZE_H */

