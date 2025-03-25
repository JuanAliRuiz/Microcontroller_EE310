//-----------------------------
// Title: EE310 Assignment 6B
//-----------------------------
// Purpose: This program used two buttons as inputs that start a counter that incriments, 
// decrements, or clears/resets depending on which or none of the buttons are pressed 
// Dependencies: MyConfigFile.inc
// Compiler: pic-as (v3.0) C:\Program Files\Microchip\xc8\v3.00\pic-as\bin
// Author: Juan Ali Ruiz Guzman 
// OUTPUTS: The outputs of these programs are Count_INC, Count_DEC, and COUNT_RST    
// INPUTS: inputs of this program are BUTTONA and BUTTONB
// which are connected to their repective button switches     
// Versions:
//  	V1.0: 03/17/2025 - First Partial Version
//	V1.2: 03/19/2025 - Revised output functions
//	V1.3: 03/20/2025 - Revised output functions again previous attempt didn't work
//	V1.4: 03/21/2025 - Revised version using pointers to optimize code
//	V1.5: 03/23/2025 - Mostly fixed program using pointers
//	V1.6: 03/24/2025 - Removed commented out old code and fixed display skipping F and lagging at 6
//-----------------------------

; Initialization - make sure the path is correct
;---------------------
#include ".\MyConfigFile.inc"
;#include "C:\Users\student\Documents\myMPLABXProjects\ProjectFirstAssemblyMPLAB\FirstAssemblyMPLAB.X\AssemblyConfig.inc"

#include <xc.inc>
;----------------
; PROGRAM INPUTS
;----------------
;The DEFINE directive is used to create macros or symbolic names for values.
;It is more flexible and can be used to define complex expressions or sequences of instructions.
;It is processed by the preprocessor before the assembly begins.   

Inner_loop  equ 100 ;in decimal
Middle_loop equ	50
Outer_loop  equ 50
;---------------------
; Definitions
;---------------------
#define LEDA      PORTD,7
#define LEDB	  PORTD,6
#define	LEDC	  PORTD,4
#define LEDD	  PORTD,2
#define	LEDE	  PORTD,1
#define	LEDF	  PORTD,0
#define	LEDG	  PORTD,3
#define	LEDDP	  PORTD,5 
;---------------------
; Program Constants
;---------------------
; The EQU (Equals) directive is used to assign a constant value to a symbolic name or label.
; It is simpler and is typically used for straightforward assignments.
;It directly substitutes the defined value into the code during the assembly process.
     REG13	equ	23h
     REG14	equ	24h
     REG15	equ	25h
    COUNT	equ	0x12
    
PSECT absdata,abs,ovrld        ; Do not change
	
ORG	0x0 ;Reset the vector
	
;ORG	0x00	;Place Pattern values at register 0
LED_PATTERNS:
    MOVLW   0b11010111
    MOVWF   0x00
    MOVLW   0b01010000
    MOVWF   0x01
    MOVLW   0b11001110
    MOVWF   0x02
    MOVLW   0b11011100
    MOVWF   0x03
    MOVLW   0b01011001
    MOVWF   0x04
    MOVLW   0b10011101
    MOVWF   0x05
    MOVLW   0b10011111
    MOVWF   0x06
    MOVLW   0b11010000
    MOVWF   0x07
    MOVLW   0b11011111
    MOVWF   0x08
    MOVLW   0b11011101
    MOVWF   0x09
    MOVLW   0b11011011
    MOVWF   0x0A
    MOVLW   0b00011111
    MOVWF   0x0B
    MOVLW   0b10000111
    MOVWF   0x0C
    MOVLW   0b01011110
    MOVWF   0x0D
    MOVLW   0b10001111
    MOVWF   0x0E
    MOVLW   0b10001011
    MOVWF   0x0F
    
ORG	0x60 ;Start Program at register 60
    CALL	BUTTONSCONFIG	    ;configure the inputs and outputs
    CALL	FSRCONFIG	    ;setup the FSR
GOTO        _start1
_start1:
    CALL    PRESSEDFUNC	    ; check if the buttons are still being pressed
    GOTO    _start1	    ;restart the main program and loop through	

BUTTONSCONFIG:
    BANKSEL PORTB
    CLRF    PORTB
    BANKSEL LATB
    CLRF    LATB
    BANKSEL ANSELB
    CLRF    ANSELB      ; Set all PORTB pins to digital
    ; Configure PORTB1 and PORTB0 as Inputs
    BANKSEL TRISB
    MOVLW   0x3        ; bit 0 and bit 1 are set as input
    MOVWF   TRISB
    ;Set PORTD as output
    BANKSEL PORTD
    CLRF    PORTD
    BANKSEL LATD    ;Allows for grabing values in RD while debugging
    CLRF    LATD
    BANKSEL ANSELD
    CLRF    ANSELD
    BANKSEL TRISD
    MOVLW   0b0000000	;Set the bits in PORTD as outputs
    MOVWF   TRISD
    RETURN
    
FSRCONFIG:;FSR Configuration
    MOVLW 0x00         ; Start address in RAM
    MOVWF FSR0L
    CLRF FSR0H         ; Assume that the address is within bank 0

    MOVLW 0x10         ; Number of bytes to read
    MOVWF COUNT
    
    MOVLW 0b11010111
    MOVWF PORTD
    RETURN
    
PRESSEDFUNC:	    ;Function for checking if both buttons are not pressed
    BTFSC   PORTB,0   ;Check if bit 0 is set 
    BTFSS   PORTB,1   ;Check if bit 1 is set
    GOTO    TEST
    GOTO resetdisplay	;if they are presed return to main program
TEST:    BTFSC	PORTB,1  	    ;check to see if only Button A is pressed
    GOTO    buttonaonly	;if it is pressed go to the incrementing loop
    BTFSC   PORTB,0 ;check to see if only Button B is pressed
    GOTO    buttonbonly	;if it is pressed go to the decrementing loop
    GOTO    STOPCOUNT	    ;go to the stopcount function if none of the buttons are pressed
    RETURN
   
resetdisplay:
    MOVLW   0x0	    
    MOVWF   FSR0L	;Go to the begining of the pattern list and display zero
    CLRF    FSR0H
    MOVF   INDF0,W	;turn on all bits exept for 3 and 5 to make a zero shape
    MOVWF   PORTD
    CALL    _delay
    GOTO    _start1

buttonaonly:	    ;function for when only button A is pressed

incloop: 
    INCF    FSR0L,F ;Load next address
    BCF  FSR0L, 4	
    BCF  FSR0L, 5
    BCF  FSR0L, 6
    BCF  FSR0L, 7	; clear the upper byte of the FSR pointer
    MOVF    INDF0,W ;Load the first pattern from the table
    MOVWF   PORTD   ;move pattern to the seven segment LEDs
    CALL    _delay  ;Call the delay loop
    CALL    PRESSEDFUNC	;Check to make sure the button is still pressed
    DECFSZ  COUNT,F
    GOTO    _countreset
    BRA	    incloop ;Loop until the button is released
    RETURN

buttonbonly:	    ;function for when only button B is pressed
;    BTFSS   PORTB,0
;    RETURN
decloop: 
    DECF    FSR0L,F	;load the next address
    BCF  FSR0L, 4
    BCF  FSR0L, 5
    BCF  FSR0L, 6
    BCF  FSR0L, 7	;clear the upper byte of the FSR pointer
    MOVF    INDF0,W	;Load current LED pattern from table
    MOVWF   PORTD	;Output to the seven segment
    CALL    _delay
    CALL    PRESSEDFUNC	;Check to make sure the button is still pressed
    INCFSZ  COUNT,F
    GOTO    _countreset
    BRA	    decloop
    RETURN

_countreset:	;Reset the counter to move the pointer back up to the top of the LED PATTERN list
    MOVLW 0x00
    MOVWF FSR0L         ; Reset pointer to beginning of list
    MOVLW 0x10
    MOVWF COUNT         ; Reset count to 16
    RETURN
    
STOPCOUNT:	;function for when neither of the buttons are pressed
    MOVF    INDF0 ,W
    MOVWF   PORTD	;keep the last known value as output in PORTD
    CALL    _delay	;call delay loop  
    GOTO    _start1	;return to the main program starting point

_delay:	    ;-----The Delay Subroutine     
    MOVLW       Inner_loop
    MOVWF       REG13
    MOVLW	Middle_loop
    MOVWF	REG15
    MOVLW       Outer_loop
    MOVWF       REG14
_loop1:
    DECF        REG13,1
    BNZ         _loop1
    MOVLW       Inner_loop ; Re-initialize the inner loop for when the outer loop decrements.
    MOVWF       REG13
    DECF        REG15,1 // middle loop
    BNZ        _loop1
    MOVLW	Middle_loop
    MOVWF	REG15
    DECF	REG14,1	    //Outer Loop
    BNZ		_loop1
    RETURN
    
    
END   
    
    


