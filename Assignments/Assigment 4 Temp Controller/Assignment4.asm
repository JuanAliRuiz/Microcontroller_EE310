//-----------------------------
// Title: EE310 Assignment 4
//-----------------------------
// Purpose: This program compares a measured temperature with a reference temperature 
// and makes adjustments to keep the temperature at the desired reference temperature.
// Dependencies: MyConfigFile.inc
// Compiler: pic-as (v3.0) C:\Program Files\Microchip\xc8\v3.00\pic-as\bin
// Author: Juan Ali Ruiz Guzman 
// OUTPUTS: The outputs of these programs are PORTD1 and PORTD2
// which are connected to the cooling and heating peripherals    
// INPUTS: inputs of this program are the reference Temperature and the Measured Temperature
// which are connected to their repective input peripherals     
// Versions:
//  	V1.0: 03/07/2025 - First version 
//
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

#define  measuredTempInput 	-5 ; this is the input value
#define  refTempInput 		15 ; this is the input value		

;---------------------
; Definitions
;---------------------
#define SWITCH    LATD,2  
#define LED0      PORTD,0
#define LED1	     PORTD,1
    
 
;---------------------
; Program Constants
;---------------------
; The EQU (Equals) directive is used to assign a constant value to a symbolic name or label.
; It is simpler and is typically used for straightforward assignments.
;It directly substitutes the defined value into the code during the assembly process.
    
REG10   equ     10h   // in HEX
REG11   equ     11h
REG01   equ     1h
contReg	equ	22h
calc	equ	14h
counter	equ	15h




PSECT absdata,abs,ovrld        ; Do not change
    
ORG          0                ;Reset vector
GOTO        _start1
ORG	    0x20 ;Start Program at register 20
   

_start1:
    BRA	    Hexconversion
_start2:
   MOVLW    0x00
   MOVWF    TRISD,0
   MOVLW    refTempInput
   MOVWF    0x20
   MOVLW    measuredTempInput
   MOVWF    0x21
   GOTO	    _equaltempscheck
   
_equaltempscheck:
   CPFSEQ   0x20 ;compare temps and skip next line if they are equal
   GOTO	    _hottertempcheck ;goto the hotter temp check if temps are not the same
   MOVLW    0x00 ;clear WREG
   MOVWF    contReg ;set contReg=0
   GOTO	    _LED_OFF ; goto _LED_OFF if temps are the same
   
_hottertempcheck:
    BTFSC   WREG,7
    NEGF    WREG
   CPFSLT   0x20 ;compare temps and skip next line if measuredtemp > refTemp
   GOTO	    _coldertempcheck ; goto colder temp check if statement above is not true
   MOVLW    0x2
   MOVWF    contReg ;set contReg=2
   GOTO	    _LED_COLD ;goto _LED_COLD if measured temp is greater than the reference temp

_coldertempcheck:
    BTFSC   WREG,7
    NEGF    WREG
   CPFSGT   0x20 ;compare temps and skip next line if measuredtemp < refTemp
   BRA	    _Error ; Branch to Error if after this step the measured temp is NOT less than the ref temp
   MOVLW    0x1
   MOVWF    contReg ;Set contReg=1
   GOTO	    _LED_HOT ; goto _LED_HOT if measured temp is less than the reference temp
   
_LED_OFF:
   MOVWF    0x20 ; Clear the Measured Temp register
   MOVWF    PORTD ;clear PORTD
   NOP
   NOP
   NOP
   SLEEP
   
_LED_COLD:
   MOVFF    contReg,PORTD ;turn on PORTD2 which is connected to the cooling system
   NOP
   NOP
   NOP
   NOP
   SLEEP

_LED_HOT:
   MOVFF    contReg,PORTD ;turn on PORTD1 which is connected to the heating system
   NOP
   NOP
   NOP
   SLEEP

_Error:
    NOP
    MOVLW   0x4 ;Will display 4 in WREG if an Error
    SLEEP

   
   
   ;	    Hex To Decimal Conversion
Hexconversion:
    MOVLW   refTempInput
    MOVWF   calc	    ;place refTempInput into placeholder register "calc"
    MOVLW   100
CASE1:
    BTFSC   calc,7	    ;check for negative numbers and convert to postive using 2's comp
    NEGF    calc
    INCF    counter,F
    SUBWF   calc,F
    BC	    CASE1
    DECF    counter,F
    ADDWF   calc,F
    MOVFF   counter,0x62  ;hundreths place of value in dec
    CLRF    counter	;clear counter register
    MOVLW   10		; now move 10 into counter to calc the number in the tens place
CASE2:
    INCF    counter,F
    SUBWF   calc,F
    BC	CASE2
    DECF    counter,F
    ADDWF   calc,F
    MOVFF   counter,0x61 ; ones place of value in dec
    MOVFF   calc,0x60	; tens place of value in dec
    CLRF    counter
    MOVLW   measuredTempInput	   ;repeat process above for measured temp value conversion
    MOVWF   calc
    BTFSC   calc,7
    NEGF    calc
    MOVLW   100
CASE3:
    INCF    counter,F
    SUBWF   calc,F
    BC	CASE3
    DECF    counter,F
    ADDWF   calc,F
    MOVFF   counter,0x72
    CLRF    counter
    MOVLW   10
CASE4:
    INCF    counter,F
    SUBWF   calc,F
    BC	CASE4
    DECF    counter,F
    ADDWF   calc,F
    MOVFF   counter,0x71
    MOVFF   calc,0x70
    GOTO    HERE
    HERE:
    NOP
    NOP
    GOTO    _start2	;go back to the main program
    
    
END
    