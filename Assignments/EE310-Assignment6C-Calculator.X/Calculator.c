/*
 * ---------------------
 * Title: Calculator in C
 * ---------------------
 * Program Details:
 *  The purpose of this program is to make a calculator
 * Inputs: get_key() from the keypad
 * Outputs: display_number() onto seven segments
 * Setup: C- Simulator
 * Date: 05 April, 2025
 * File Dependencies / Libraries: It is required to include the 
 * Configuration Header File 
 * Compiler: xc8, 3.0
 * Author: Juan Ali Ruiz Guzman
 * Versions:
 *      V1.0: 05 April 2025 - Creation of keypad and seven segment configurations
 *      V1.2: 06 April 2025 - Creation of Calculation and Error Functions
 *      V1.3: 07 April 2025 - Creation of Main Function calling outside functions
 *      V1.4: 08 April 2025 - Reworking waitfor_key and display_number functions
 *      V1.5: 08 April 2025 - attempted fixing multiplexing 'coppying' issues by adding transistors
 *      V1.6: 09 April 2025 - implemented more primitive approach to get dual seven segments working by assigning each their own ports
 *      V1.7: 09 April 2025 - fixed second seven segment display PORTC bits 0 and 1 not outputting by using PORTA0 and PORTA1
 *      V1.8: 09 April 2025 - fixed second input not accepting second digit by adding else if instead of going straight to state 2
 *      V1.9: 10 April 2025 - removed debugging and commented out code
 */

// PIC18F46K42 Configuration Bit Settings

#include <xc.h>  // Must have this
#include "MyConfigFile.h"
#include "PeripheralSetup.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

#define _XTAL_FREQ 4000000 // Fosc frequency for _delay() library
#define FCY (_XTAL_FREQ / 4)

#define DIGIT1 0x01
#define DIGIT2 0x02

unsigned char X_Input_REG = 0;
unsigned char Y_Input_REG = 0;
unsigned char Operation_REG = 0;
signed char Display_Result_REG = 0;
unsigned char current_input = 0;  // Tracks live input (00?99)
char operand = 0;                 // 'A', 'B', 'C', 'D'
char state = 0; // 0 = input X_Input_REG, 1 = operand pressed, 2 = input Y_input_REG, 3 = show result
char error_message = 0;
signed int result = 0;

void calculate_result();
void displayEE();
void display_number(signed int result);

void main() {
    setup_keypadports();
    setup_sevensegports();
    
    while (1) {
        char key = waitfor_key();

        if (key == 'C') {
            // Clear inputs and reset
            X_Input_REG = 0;
            Y_Input_REG = 0;
            current_input = 0;
            operand = 0;
            state = 0;
            PORTD = segMap[0];
            PORTC = segMap[0];
            PORTAbits.RA0 = PORTCbits.RC0;
            PORTAbits.RA1 = PORTCbits.RC1;
            continue;
        }

        switch (state) {
            case 0: // First number input
                if (key >= '0' && key <= '9') { //check for values within 0-9
                    current_input = current_input * 10 + (key - '0');
                    if (current_input > 99) current_input = 99;
                    X_Input_REG = current_input;
                    display_number(X_Input_REG);
                } else if (key == '+' || key == '-' || key == '*' || key == '/') {
                    if(state == 0){
                        operand = key;
                        Operation_REG = operand;
                        current_input = 0;
                        state = 1;
                    }
                }
                break;

            case 1: // Second number input
                if (key >= '0' && key <= '9') {
                    current_input = current_input * 10 + (key - '0');
                    if (current_input > 99) current_input = 99;
                    Y_Input_REG = current_input;
                    display_number(Y_Input_REG);
                }
                    else if (key == '='){
                    state = 2;
                    }
                break;

            case 2: // Wait for '=' (#)
                if (key == '=') {
                    calculate_result();
                    result = Display_Result_REG;
                    display_number(result);
      
                    __delay_ms(1500);
                    
                }
                break;
        }

    }
}

const char keypad[4][4] = {
    {'1', '2', '3', '+'},
    {'4', '5', '6', '-'},
    {'7', '8', '9', '*'},
    {'C', '0', '=', '/'}
};

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

void setup_keypadports() {
    TRISB = 0b11110000; // Rows RB0-RB3 output, columns RB4-RB7 input
    LATB = 0x00;
    ANSELB = 0x00;
}

void setup_sevensegports() {
    TRISD = 0x00; // All PORTD for first 7-segment
    LATD = 0x00;
    PORTD = segMap[0];
    ANSELD = 0x00;

    TRISC = 0x00; //All PORTA for second 7-segment
    LATC = 0x00;
    PORTC = segMap[0];
    ANSELC = 0x00;
    
    TRISA = 0x00;
    LATA = 0x00;
    PORTA = 0b00000011;
    ANSELA = 0x00;

}


// Matrix keypad scanning
char scan_keypad() {
    
    // Scan function for keypad button press
    for (int row = 0; row < 4; row++) {
        // Set all rows LOW
    ROW1 = ROW2 = ROW3 = ROW4 = 0;

    // Drive current row HIGH
        switch (row) {
            case 0: ROW1 = 1; break;
            case 1: ROW2 = 1; break;
            case 2: ROW3 = 1; break;
            case 3: ROW4 = 1; break;
        }

        __delay_ms(5); // delay

        if (COL1 == 1) { while (COL1 == 1); return keypad[row][0]; }
        if (COL2 == 1) { while (COL2 == 1); return keypad[row][1]; }
        if (COL3 == 1) { while (COL3 == 1); return keypad[row][2]; }
        if (COL4 == 1) { while (COL4 == 1); return keypad[row][3]; }
    }

    return 0; // No key pressed
}


// Full key retrieval (press & release)
char waitfor_key() {
    char key = 0;
    int timeout = 2000;

    // Wait for press
    while ((key = scan_keypad()) == 0 && timeout--)
        __delay_ms(1);

    if (key == 0) return 0;

    __delay_ms(20); // Debounce
    return key;
}

// Function for Calculation
void calculate_result() {
//    error_message = 0;

    if (operand == '+') {
        Display_Result_REG = X_Input_REG + Y_Input_REG;
    }
    else if (operand == '-') {
        Display_Result_REG = X_Input_REG - Y_Input_REG;
    }
    else if (operand == '*') {
        Display_Result_REG = X_Input_REG * Y_Input_REG;
    }
    else if (operand == '/') {
        if (Y_Input_REG != 0) { // Division.Check for divide-by-zero to avoid crash. 
            Display_Result_REG = X_Input_REG / Y_Input_REG;
            }
        else{
            displayEE();
        }

    }
    else {
        displayEE();
    }
    
}

void displayEE() {

    PORTD = 0b10001111; //Display 'E' on first display
    PORTC = 0b10001111; //Display 'E' on second display
    PORTAbits.RA0 = PORTCbits.RC0;
    PORTAbits.RA1 = PORTCbits.RC1;

    __delay_ms(1500);

}

// Function for displaying a number onto seven segment
void display_number(signed int result) {
    if (result > 99) result = 99;
    if (result < -99) result = 99;
    bool neg = false;
    
    if(result < 0) {
        result =(~result)+1;
        neg = true;
    }
   

    unsigned char tens = result / 10;
    unsigned char ones = result % 10;


    // Display tens digit
    PORTD = segMap[tens];
    if (neg == true){
    PORTDbits.RD5 = 1;
    } 


    // Display ones digit
    PORTC = segMap[ones];
    PORTAbits.RA0 = PORTCbits.RC0;
    PORTAbits.RA1 = PORTCbits.RC1;

    __delay_ms(200);
}

