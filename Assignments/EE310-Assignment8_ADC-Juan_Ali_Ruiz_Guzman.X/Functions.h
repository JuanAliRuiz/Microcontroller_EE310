/* 
 * File:   Functions.h
 * This file serves as functions that the main program call for interrupt programs
 * as well as other functions needed for the program
 * Author: Juan Ali Ruiz Guzman
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef FUNCTIONS_H
#define	FUNCTIONS_H

#define _XTAL_FREQ 4000000                 // Fosc  frequency for _delay()  library
#define FCY    _XTAL_FREQ/4

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <pic18f47k42.h>

#define RS LATD0                   /* PORTD 0 pin is used for Register Select */
#define EN LATD1                   /* PORTD 1 pin is used for Enable */
#define ldata LATB                 /* PORTB is used for transmitting data to LCD */
#define LCD_Port TRISB              
#define LCD_Control TRISD
#define BUZZER PORTDbits.RD2
#define Vref 3.3 // voltage reference 

// Define some basic note frequencies (in Hz)
#define NOTE_C4  261
#define NOTE_D4  294
#define NOTE_E4  329
#define NOTE_F4  349
#define NOTE_G4  392
#define NOTE_A4  440
#define NOTE_B4  493
#define NOTE_C5  523




void ADC_RA2Conver(void);
void ADC_RA3Conver(void);
void __interrupt(irq(default),base(0x4008)) my_ISR(void);
void timer_func(void);
void volt_read_func(void);
void play_note(unsigned int frequency, unsigned int duration_ms);
void play_melody(void);


int digital; // holds the digital value
float dB; // holds the analog value(lux)
float voltage; // hold the analog value (volt))
char dBStr[10];
char data[10];
int state = 0;
float counter = 10;
char counterStr[11];

/****************************Functions********************************/


//Function for microphone measurements
void ADC_RA2Conver(void){ 
    ADPCH = 0x02; // select AN2 (RA2)
    __delay_ms(5);
    //Do set ADCON0 Go to start conversion
    ADCON0bits.GO = 1;
    while (ADCON0bits.GO); //Wait for conversion done
    digital = (ADRESH*256) | (ADRESL); /*Combine 8-bit LSB and 2-bit MSB*/
    dB = 20 * log10((float)digital * ((float)Vref / 4096.0));
    
    /*This is used to convert integer value to ASCII string*/
    sprintf(dBStr,"%.2f",dB);
    strcat(dBStr,"   dB ");	/*Concatenate result and unit to print*/
    
    LCD_String_xy(1,0,"Input Sound: ");    /* Display string at location(row,location). */
                                   /* This function passes string to display */
    LCD_String_xy(2,0,dBStr);   /*Display string at location(row,location). */
            
    
}
//Function for Voltage meter
void ADC_RA3Conver(void){
        ADPCH = 0x03;  // Select AN3 (RA3)
        __delay_us(5);
        //DO: Set ADCON0 Go to start conversion
        ADCON0bits.GO = 1;
        while (ADCON0bits.GO); //Wait for conversion done
        
        digital = (ADRESH*256) | (ADRESL);/*Combine 8-bit LSB and 2-bit MSB*/
        // DO: define voltage = Vref/4096 (note that voltage is float type
        voltage = digital*((float)Vref/(float)(4096));
        
        /*This is used to convert integer value to ASCII string*/
        sprintf(data,"%.2f",voltage);
        strcat(data," V");	/*Concatenate result and unit to print*/
        
        LCD_String_xy(1,0,"Input Voltage: ");    /* Display string at location(row,location). */
                                   /* This function passes string to display */
        LCD_String_xy(2,0,data);   /*Display string at location(row,location). */
                                   /* This function passes string to display */ 
}

// Interrupt function for both interrupt buttons
void __interrupt(irq(IRQ_IOC),base(0x4008)) ISR(void)
{
    if (PIR0bits.IOCIF == 1){
        if (IOCAFbits.IOCAF0) // Check if RA0 caused interrupt
        { // Handle RA0 event
            play_note(NOTE_C4,2000);
            LCD_Clear();
            volt_read_func();
            LCD_Clear();
            __delay_ms(300);
            IOCAFbits.IOCAF0 = 0; // Clear individual flag
        }
        if (IOCAFbits.IOCAF1) // Check if RA1 caused interrupt
        { //Handle RA1 event
            play_note(NOTE_D4,2000);
            LCD_Clear();
            timer_func();
            play_note(NOTE_E4, 4000);
            LCD_Clear();
            __delay_ms(300);
            IOCAFbits.IOCAF1 = 0; // Clear individual flag
        }
    }
        PIR0bits.IOCIF = 0; // Always clear global IOC flag!
}

//Count Down function for the delay interrupt 
void timer_func(void){
    counter = 10;
    while (counter >= 0){
        play_melody();
        /*This is used to convert integer value to ASCII string*/
        sprintf(counterStr,"%.2f",counter);
        strcat(counterStr," s ");	/*Concatenate result and unit to print*/
        LCD_String_xy(1,0,"Please Wait: ");    /* Display string at location(row,location). */
                                   /* This function passes string to display */
        LCD_String_xy(2,0,counterStr);   /*Display string at location(row,location). */    
        PORTDbits.RD3 = 1; //turn on the LED
        __delay_ms(1000);
        PORTDbits.RD3 = 0;
        __delay_ms(200);
        counter--;
    }
   counter = 10;
}
//embedded function to read voltage when interrupt RA0 is flagged
void volt_read_func(void){
    while (PORTAbits.RA4 != 1){
        ADC_RA3Conver();
    }
    play_note(NOTE_F4,2000);
}

// Delay function for toggling RD2 pin to play a note on the buzzer
void play_note(unsigned int frequency, unsigned int duration_ms)
{
    unsigned int i;
    unsigned long delay_us = 1000000UL / frequency / 2;  // Half period
    unsigned long cycles = (unsigned long)duration_ms * 1000UL / (delay_us * 2);

    for (i = 0; i < cycles; i++)
    {
        BUZZER = 1;
        __delay_us(100); // Use fixed small delay
        BUZZER = 0;
        __delay_us(100); // You can adjust this based on desired frequency
    }
}

// Melody function for the buzzer
void play_melody(void)
{
    // Set RD2 as output
    TRISDbits.TRISD2 = 0;

    play_note(NOTE_C4, 400);
    play_note(NOTE_D4, 400);
    play_note(NOTE_E4, 400);
    play_note(NOTE_F4, 400);
    play_note(NOTE_G4, 400);
    play_note(NOTE_A4, 400);
    play_note(NOTE_B4, 400);
    play_note(NOTE_C5, 600);
    
    __delay_ms(500); // Pause
}
#endif	/* FUNCTIONS_H */

