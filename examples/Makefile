#===============================================================================
# DEV65 Build Rules
#-------------------------------------------------------------------------------
# Copyright (C),2015-2016 HandCoded Software Ltd.
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

# Update the following line to reflect where you have installed the ZIP file
# containing the JAVA classes relative to where the assembler is invoked.

DEV65_DIR	=	..

#===============================================================================

DEV65_JAR	=	$(DEV65_DIR)/Dev65.jar

AS65_CLASS	=	uk.co.demon.obelisk.w65xx.As65

LK65_CLASS	=	uk.co.demon.obelisk.w65xx.Lk65

LB65_CLASS	=	uk.co.demon.obelisk.w65xx.Lb65

AS65		=	java -cp $(DEV65_JAR) $(AS65_CLASS)

LK65		=	java -cp $(DEV65_JAR) $(LK65_CLASS)

LB65		=	java -cp $(DEV65_JAR) $(LB65_CLASS)

RM			=	erase

#===============================================================================
# Rules
#-------------------------------------------------------------------------------

.asm.obj:
	$(AS65) $(AS65_FLAGS) $<

