;===============================================================================
; __        ____  ____   ____ ___  _  __   
; \ \      / / /_| ___| / ___( _ )/ |/ /_  
;  \ \ /\ / / '_ \___ \| |   / _ \| | '_ \ 
;   \ V  V /| (_) |__) | |__| (_) | | (_) |
;    \_/\_/  \___/____/ \____\___/|_|\___/ 
;
; Western Design Center W65C816 device definitions
;-------------------------------------------------------------------------------
; Copyright (C)2015 HandCoded Software Ltd.
; All rights reserved.
;
; This work is made available under the terms of the Creative Commons
; Attribution-NonCommercial-ShareAlike 4.0 International license. Open the
; following URL to see the details.
;
; http://creativecommons.org/licenses/by-nc-sa/4.0/
;
;===============================================================================
; Notes:
;
; Various macros and definitions for the W65C816 microcontroller.
;
;===============================================================================
; Revision History:
;
; 2015-12-18 AJ Initial version
;-------------------------------------------------------------------------------
; $Id$
;-------------------------------------------------------------------------------

                .65816

;===============================================================================
; Status Register Bits
;-------------------------------------------------------------------------------

N_FLAG          .equ    1<<7
V_FLAG          .equ    1<<6
M_FLAG          .equ    1<<5
X_FLAG          .equ    1<<4
B_FLAG          .equ    1<<4
D_FLAG          .equ    1<<3
I_FLAG          .equ    1<<2
Z_FLAG          .equ    1<<1
C_FLAG          .equ    1<<0

;==============================================================================
; Macros
;------------------------------------------------------------------------------

; Puts the processor in emulation mode. A, X and Y become 8-bits and the stack
; is fixed at $0100-$01ff.

emulate         .macro
                sec
                xce
                .endm

; Puts the processor in native mode. The size of the memory and index register
; operations is not controlled by the M & X bits in the status register.

native          .macro
                clc
                xce
                .endm

; Resets the M bit making the accumator and memory accesses 16-bits wide.

long_a          .macro
                rep     #M_FLAG
                .longa  on
                .endm

; Resets the X bit making the index registers 16-bits wide

long_i          .macro
                rep     #X_FLAG
                .longi  on
                .endm

; Resets the M and X bits making the accumator, memory accesses and index
; registers 16-bits wide.

long_ai         .macro
                rep     #M_FLAG|X_FLAG
                .longa  on
                .longi  on
                .endm

; Sets the M bit making the accumator and memory accesses 16-bits wide.

short_a         .macro
                sep     #M_FLAG
                .longa  off
                .endm

short_i         .macro
                sep     #X_FLAG
                .longi  off
                .endm

short_ai        .macro
                sep     #M_FLAG|X_FLAG
                .longa  off
                .longi  off
                .endm
