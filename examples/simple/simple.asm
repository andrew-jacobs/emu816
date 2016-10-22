;===============================================================================
;
; A Simple W65C816 Program                                                                            
;-------------------------------------------------------------------------------
; Copyright (C),2016 HandCoded Software Ltd.
; All rights reserved.
;
; This work is made available under the terms of the Creative Commons
; Attribution-NonCommercial 2.0 license. Open the following URL to see the
; details.
;
; http://creativecommons.org/licenses/by-nc/2.0/
;-------------------------------------------------------------------------------
;
; Notes:
;
;
;
;-------------------------------------------------------------------------------

		.include "../w65c816.inc"

		.page0
		
;===============================================================================
; Memory Areas
;-------------------------------------------------------------------------------
		
		.space	128
STACK:						; Top of stack area

;===============================================================================
; Power On Reset Handler
;-------------------------------------------------------------------------------

		.code
		.org	$f000

		.longa	off
		.longi	off
RESET:
		sei
		cld
		native
		long_ai
		
		lda	#STACK
		tcs
		
		ldy	#100
		repeat
		 ldx	#0
		 repeat
		  dex
		 until eq
		 dey
		until eq
		wdm	#$ff
		bra	RESET
		
;===============================================================================
; Dummy Interrupt Handlers
;-------------------------------------------------------------------------------

IRQN:
COPN:
ABORTN:
NMIN:
BRKN
		bra	$
		
COP:
ABORT:
NMI:
IRQBRK:
		bra	$

;===============================================================================
; Vectors
;-------------------------------------------------------------------------------

		.org	$ffe0

                .space  4                       ; Reserved
                .word	COPN                    ; $FFE4 - COP(816)
                .word	BRKN                    ; $FFE6 - BRK(816)
                .word	ABORTN                  ; $FFE8 - ABORT(816)
                .word	NMIN                    ; $FFEA - NMI(816)
                .space	2                       ; Reserved
                .word	IRQN                    ; $FFEE - IRQ(816)

                .space	4
                .word	COP                     ; $FFF4 - COP(C02)
                .space	2                       ; $Reserved
                .word	ABORT                   ; $FFF8 - ABORT(C02)
                .word	NMI                     ; $FFFA - NMI(C02)
                .word	RESET                   ; $FFFC - RESET(C02)
                .word	IRQBRK                  ; $FFFE - IRQBRK(C02)

		.end