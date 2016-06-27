#===============================================================================
# Makefile for Simple Example
#-------------------------------------------------------------------------------
# Copyright (C)2016 HandCoded Software Ltd.
# All rights reserved.
#
# This work is made available under the terms of the Creative Commons
# Attribution-NonCommercial 2.0 license. Open the following URL to see the
# details.
#
# http://creativecommons.org/licenses/by-nc/2.0/
#-------------------------------------------------------------------------------
#
# Notes:
#
#
#-------------------------------------------------------------------------------

include	../Makefile

#===============================================================================
# Targets
#-------------------------------------------------------------------------------

OBJS		= \
	simple.obj
	

all:	simple.s28

clean:
	$(RM) *.obj
	$(RM) *.bin
	$(RM) *.hex
	$(RM) *.txt
	$(RM) *.lst
	$(RM) *.map
	$(RM) *.s28

simple.s28: $(OBJS)
	$(LK65) -bss $$0000-$$7FFF -code $$F000-$$FFFF -s28 -output $@ $(OBJS)

#===============================================================================
# Dependencies
#-------------------------------------------------------------------------------

simple.obj: \
	simple.asm ..\w65c816.inc
