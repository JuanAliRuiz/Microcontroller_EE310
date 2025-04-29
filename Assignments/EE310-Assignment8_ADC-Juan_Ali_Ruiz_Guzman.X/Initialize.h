/* 
 * File:   Initialize.h 
 * This file serves as functions that the main program call for initializing
 * both ports and peripherals
 * Author: Juan Ali Ruiz Guzman
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef Initialize_H
#define	Initialize_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pic18f47k42.h>

#define _XTAL_FREQ 4000000                 // Fosc  frequency for _delay()  library
#define FCY    _XTAL_FREQ/4

#define RS LATD0                   /* PORTD 0 pin is used for Register Select */
#define EN LATD1                   /* PORTD 1 pin is used for Enable */
#define ldata LATB                 /* PORTB is used for transmitting data to LCD */
#define LCD_Port TRISB              
#define LCD_Control TRISD

void INTERRUPT_Initialize (void);
void PORTS_Init(void);
void ADC_Init(void);
void LCD_Clear();
void LCD_Command(char );
void LCD_Char(char x);
void LCD_String(const char *);
void LCD_String_xy(char ,char ,const char*);
void MSdelay(unsigned int);
void LCD_Init();

void INTERRUPT_Initialize(void)
{
    INTCON0bits.GIEH = 1;  // Enable high priority interrupts
    INTCON0bits.GIEL = 1;  // Enable low priority interrupts
    INTCON0bits.IPEN = 1;  // Enable interrupt priority

    // Enable rising edge interrupt on RA0 (IOC0)
    IOCAPbits.IOCAP0 = 1;  // Enable rising edge detection on RA0
    // Enable rising edge interrupt on RA1 (IOC1)
    IOCAPbits.IOCAP1 = 1;  // Enable rising edge detection on RA1

    // Set priority for IOC interrupt
    PIE0bits.IOCIE = 1;    // Enable IOC interrupts
    IPR0bits.IOCIP = 1;    // Set IOC interrupt to high priority

    IOCAFbits.IOCAF0 = 0; // Clear flag
    IOCAFbits.IOCAF1 = 0;
    PIR0bits.IOCIF = 0;    // Clear IOC interrupt flag

    // change the IVTBASE
    IVTBASEU = 0x00;
    IVTBASEH = 0x40;
    IVTBASEL = 0x08;
}

void PORTS_Init(void){
    // Inputs Initialize
    PORTA = 0; 
    LATA = 0; 
    ANSELA = 0;
    WPUA = 0;
    TRISA = 1;  
    PORTB = 0; //port for display
    LATB = 0; 
    ANSELB = 0b00000000; 
    TRISB = 0b00000000;       
    PORTC = 0; //Extra Port for outputs
    LATC = 0; 
    ANSELC = 0; 
    TRISC = 0b00000000;        
    PORTD = 0; // also port for output to the display
    LATD = 0; 
    ANSELD = 0; 
    TRISD = 0;
}

void ADC_Init(void)
{
    // Setup RA0, RA2, RA3 as analog inputs
    TRISAbits.TRISA0 = 1; // Set RA0 to input
    TRISAbits.TRISA2 = 1; // Set RA2 to input
    TRISAbits.TRISA3 = 1; // Set RA3 to input
    
    ANSELAbits.ANSELA0 = 0; // Set RA0 to digital
    ANSELAbits.ANSELA2 = 1; // Set RA2 to analog
    ANSELAbits.ANSELA3 = 1; // Set RA3 to analog
    
    ADCON0bits.FM = 1;  // Right justify result
    ADCON0bits.CS = 1;  // Use internal ADC clock (ADCRC)
    
    ADCLK = 0x00;       // ADC clock select register (Fosc/2)
    
    ADREF = 0x00;       // Vref+ = Vdd, Vref- = Vss (default)
    ADPREL = 0x00;      // Precharge time low byte
    ADPREH = 0x00;      // Precharge time high byte
    ADACQL = 0x00;      // Acquisition time low byte
    ADACQH = 0x00;      // Acquisition time high byte
    
    ADCAP = 0x00;       // Precharge selection (none)
    
    ADCON0bits.ON = 1;  // Turn ADC On
}

void LCD_Clear()
{
        LCD_Command(0x01); /* clear display screen */
}

void LCD_Command(char cmd )
{
    ldata= cmd;            /* Send data to PORT as a command for LCD */   
    RS = 0;                /* Command Register is selected */
    EN = 1;                /* High-to-Low pulse on Enable pin to latch data */ 
    NOP();
    EN = 0;
    MSdelay(3); 
}

void LCD_Char(char dat) //function for using characters to display on LCD
{
    ldata= dat;            /* Send data to LCD */  
    RS = 1;                /* Data Register is selected */
    EN=1;                  /* High-to-Low pulse on Enable pin to latch data */   
    NOP();
    EN=0;
    MSdelay(1);
}

void LCD_String(const char *msg) // function for using strings in LCD
{
    while((*msg)!=0)
    {       
      LCD_Char(*msg);
      msg++;    
        }
}

void LCD_String_xy(char row,char pos,const char *msg) //display function for the lines on the display
{
    char location=0;
    if(row<=1)
    {
        location=(0x80) | ((pos) & 0x0f); /*Print message on 1st row and desired location*/
        LCD_Command(location);
    }
    else
    {
        location=(0xC0) | ((pos) & 0x0f); /*Print message on 2nd row and desired location*/
        LCD_Command(location);    
    }  
    LCD_String(msg);

}
/*********************************Delay Function********************************/
void MSdelay(unsigned int val)
{
     unsigned int i,j;
        for(i=0;i<val;i++)
            for(j=0;j<165;j++);      /*This count Provide delay of 1 ms for 8MHz Frequency */
}

void LCD_Init() // Function for initializing the LCD
{

    MSdelay(15);           /* 15ms,16x2 LCD Power on delay */
    LCD_Port = 0x00;       /* Set PORTB as output PORT for LCD data(D0-D7) pins */
    LCD_Control = 0x00;    /* Set PORTD as output PORT LCD Control(RS,EN) Pins */
    LCD_Command(0x01);     /* clear display screen */
    LCD_Command(0x38);     /* uses 2 line and initialize 5*7 matrix of LCD */
    LCD_Command(0x0c);     /* display on cursor off */
    LCD_Command(0x06);     /* increment cursor (shift cursor to right) */
    
    LCD_String_xy(1,0," Juan Ali Ruiz ");    /* Display string at location(row,location). */
                                   /* This function passes string to display */
    LCD_String_xy(2,0,"EE310 @ SSU");   /*Display string at location(row,location). */
                                   /* This function passes string to display */    
    __delay_ms(5000);
}
#endif	/* Initialize_H */

