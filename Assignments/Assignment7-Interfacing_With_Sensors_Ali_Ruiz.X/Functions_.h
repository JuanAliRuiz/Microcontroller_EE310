/*
 Header File for Main_SafeBox.c containing functions
 * for functions called by main file
 * 
 * Author: Juan Ali Ruiz Guzman
 * 
 * 
 
 
 
 */
// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef Functions__H
#define	Functions__H

#include <xc.h> // include processor files - each processor file is guarded.  
#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include "Peripherals_Initialize.h"

#define _XTAL_FREQ 4000000 // Fosc frequency for _delay() library
#define FCY (_XTAL_FREQ / 4)

const char segMap[10] = {
    0b11010111, // 0
    0b01010000, // 1
    0b11001110, // 2
    0b11011100, // 3
    0b01011001, // 4
    0b10011101, // 5
    0b10011111, // 6
    0b11010000, // 7
    0b11011111, // 8
    0b11011101  // 9
};
signed int result = 0;
signed int secretcode = 0x23;
unsigned int number1 = 0;
unsigned int number2 = 0;
unsigned int state = 0;
unsigned int combinednum = 0;
unsigned int duration_ms = 0;


//Function Declarations:
void comparefunc(void);
void _enter_val_button(void);


//Photoresistor Function:
void codeinput(void);

//System LED Function;
void SYS_LED(void);

//Motor Function:
void motorfunc(void);

//Buzzer Function:
void buzzerfunc(void);

//Melody Functions:
void note(unsigned int duration_ms);
void playMelody(void);
void delay_ms(unsigned int ms);

//Seven Segment Display Functions:
void display_number(unsigned int num);
void DisplayEE(void);
void reset(void);


void reset(){
    display_number(segMap[0]);
    __delay_ms(2000);
}

void comparefunc(){
    combinednum = (number1 << 4) | (number2 & 0x0F);
    if (secretcode == combinednum){
        motorfunc();
    }
    else{
        display_number(7);
        buzzerfunc();
    }
    state = 0;
}


void buzzerfunc(){
    _buzzer = 1;
    __delay_ms(2000);
    _buzzer = 0;
}

void motorfunc(){
    motor = 1;
    __delay_ms(5000);
    motor = 0;
}



void SYS_LED(){
    sysled = 1;
}

void codeinput(){
        state = 0;
        while(state == 0){
            if(photores1 == 1){
                number1++;
                display_number(number1);
                __delay_ms(800);
            }
            if(enter_val_button == 0){
                state = 1;
            }
        }
        __delay_ms(800);
        while(state == 1){
            if(photores2 == 1){
                number2++;
                display_number(number2);
                __delay_ms(800);
            }
            if(enter_val_button == 0){
                state = 2;
            }
        }

}

void display_number(unsigned int num){
    if (num < 10){
        LATD = segMap[num];
    }
    else{
        LATD = segMap[0]; //blank if out of range
    }
}

// Defining Interrupt ISR 
void __interrupt(irq(IRQ_INT0),base(0x4008)) INT0_ISR(void)
{
    sysled = 0;
        // Check if interrupt flag for INT0 is set to 1 - (note INT0 is your input)
    playMelody();
    __delay_ms(1000);
        // clear the interrupt flag for INT0 when done
    PIR1=0;
}

void delay_ms(unsigned int ms) {
    while (ms--) {
        __delay_ms(1);  // constant one per variable
    }
}

void note(unsigned int duration_ms){
    _buzzer = 1;  //Turn the buzzer on
    delay_ms(duration_ms); // wait for the desired delay
    _buzzer = 0; //turn the buzzer back off
    __delay_ms(100); //gap between notes
    
}

void playMelody(void) {
    note(200); // Beep 1
    note(200); // Beep 2
    note(400); // Beep 3
    __delay_ms(500); // Pause
    note(300); // Beep 4
    note(100); // Beep 5
}



#endif	/* Functions__H */

