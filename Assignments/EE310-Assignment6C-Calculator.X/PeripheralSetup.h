// PeripheralSetup.h
// Author: Juan Ali Ruiz Guzman
// Date: 05 April 2025
// Description: Header for calculator project using 4x4 keypad and dual 7-segment display (PIC18F46K42)

#ifndef PERIPHERAL_SETUP_H
#define PERIPHERAL_SETUP_H

#include <xc.h>
#include "PragmaConfig.h"   
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define _XTAL_FREQ 4000000
#define FCY (_XTAL_FREQ / 4)

// Keypad Matrix Definitions (Row: Output, Col: Input)
#define ROW1 LATBbits.LATB0
#define ROW2 LATBbits.LATB1
#define ROW3 LATBbits.LATB2
#define ROW4 LATBbits.LATB3

#define COL1 PORTBbits.RB4
#define COL2 PORTBbits.RB5
#define COL3 PORTBbits.RB6
#define COL4 PORTBbits.RB7

//// Segment Control bits
//#define DIGIT1 LATCbits.LATC0
//#define DIGIT2 LATCbits.LATC1

//Function Declarations:
void setup_keypadports(void);
void setup_sevensegports(void);

// Keypad Functions:
char scan_keypad(void);
char waitfor_key(void);

// External Constant Maps:
extern const char keypad[4][4];
extern const char segMap[10];

#endif







////this file contains functions setting up the peripherals attatched to the microcontroller
//// This file is solely used for the calculator project and only includes configs for 
//// the display and the keypad in C code
//// Date: 05 April 2025
//// Author: Juan Ali Ruiz Guzman
//
////initializations:
//#include <xc.h> // must have this
//#include "PragmaConfig.h"
////#include "../../../../../Program Files/Microchip/xc8/v2.40/pic/include/proc/pic18f46k42.h"
////include "C:/Program Files/Microchip/xc8/v3.00/pic/include/proc/pic18f46k42.h"
////#include "C:\Program Files\Microchip\xc8\v2.40\pic\include\proc\pic18f46k42"
//#include <stdio.h>
//#include <stdlib.h>
//#include <math.h>
//
//#define _XTAL_FREQ 4000000                 // Fosc  frequency for _delay()  library
//#define FCY    _XTAL_FREQ/4
//
////Define the rows and columns of the Keypad
//#define ROW1 LATBbits.LATB0
//#define ROW2 LATBbits.LATB1
//#define ROW3 LATBbits.LATB2
//#define ROW4 LATBbits.LATB3
//
//#define COL1 PORTBbits.RB4
//#define COL2 PORTBbits.RB5
//#define COL3 PORTBbits.RB6
//#define COL4 PORTBbits.RB7
//
//void MSDelay(unsigned int, unsigned int); //Function declaration
//void setup_keypadports();
//void setup_sevensegports();
//const char keypad[4][4];
//char scan_keypad();
//char get_key();
//const char segMap[10];
//
//
//
//// Keypad layout
//const char keypad[4][4] = {
//    {'1','2','3','A'},
//    {'4','5','6','B'},
//    {'7','8','9','C'},
//    {'*','0','#','D'}
//};
//
//// SevenSegment Map
//const char segMap[10] = {
//    0b11010111, //0
//    0b01010000, //1
//    0b11001110, //2
//    0b11011100, //3
//    0b01011001, //4
//    0b10011101, //5
//    0b10011111, //6
//    0b11010000, //7
//    0b11011111, //8
//    0b11011101 //9
//};
//
//
//////Function determining which key was pressed
////char scan_keypad() {
////    
////    
////for (int row = 0; row < 4; row++) {
////// Set all rows high
////LATB &= 0xF0;
////LATB |= (1 << row);  // Set one row LOW at a time
////        
////        MSDelay(5,5); // debounce delay
////
////        // Read columns
////        if (COL1) return keypad[row][0];
////        if (COL2) return keypad[row][1];
////        if (COL3) return keypad[row][2];
////        if (COL4) return keypad[row][3];
////    }
////    return 0;  // No key pressed
////    }
//
//
//// Function for initializing the keypad ports
//void setup_keypadports()
//{
//    LATB = 0;
//    ANSELB = 0;
//    PORTB = 0;
//    TRISB = 0b11110000;
//    
//}
//
////Function for fetching the key being pressed
//char get_key() {
//    char key = -1;
//    PORTB=2;
//    if (PORTBbits.RB5==1)
//        key=9;
//    else if(PORTBbits.RB6==1)
//        key=6;
//    else if(PORTBbits.RB7==1)
//        key=3;
//    PORTB=4;
//    if (PORTBbits.RB4==1)
//        key=0;
//    else if(PORTBbits.RB5==1)
//        key=8;
//    else if(PORTBbits.RB6==1)
//        key=5;
//    else if(PORTBbits.RB7==1)
//        key=2;
//     PORTB=8;
//    if (PORTBbits.RB5==1)
//        key=7;
//    else if(PORTBbits.RB6==1)
//        key=4;
//    else if(PORTBbits.RB7==1)
//        key=1;
//
//    return key;
//}
//
//
//
////Delay Function
//void MSDelay  (unsigned int itime1, unsigned int itime2)
//{
//    unsigned int i; unsigned int j;
//    for(i=0; i<itime1; i++) //should be able to see itime as a variable
//        for(j=0; j<itime2; j++);
//        
//}
//
//// Segment pins -> PORTD (RD0-RD7)
//// Digit select pins -> PORTCbits.RC0 and RC1
//
//void setup_sevensegports() {
//    //Digital Com Pins
//    TRISD = 0x00; // Segment output
//    TRISCbits.TRISC0 = 0; // Digit select
//    TRISCbits.TRISC1 = 0; // Digit 1 and 2 enable
//
//    PORTC = 0x00;
//
//    
//    //Seven Segment regular pins
//    PORTD = 0;
//    LATD = 0;
//    ANSELD = 0;
//    TRISD = 0x00;  // All PORTD as output (segments)
//    PORTD = 0b11010111; //0
//
//}
//
//
//
