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


