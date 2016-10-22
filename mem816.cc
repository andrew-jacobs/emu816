//==============================================================================
//                                          .ooooo.     .o      .ooo   
//                                         d88'   `8. o888    .88'     
//  .ooooo.  ooo. .oo.  .oo.   oooo  oooo  Y88..  .8'  888   d88'      
// d88' `88b `888P"Y88bP"Y88b  `888  `888   `88888b.   888  d888P"Ybo. 
// 888ooo888  888   888   888   888   888  .8'  ``88b  888  Y88[   ]88 
// 888    .o  888   888   888   888   888  `8.   .88P  888  `Y88   88P 
// `Y8bod8P' o888o o888o o888o  `V88V"V8P'  `boood8'  o888o  `88bod8'  
//                                                                    
// A Portable C++ WDC 65C816 Emulator  
//------------------------------------------------------------------------------
// Copyright (C),2016 Andrew John Jacobs
// All rights reserved.
//
// This work is made available under the terms of the Creative Commons
// Attribution-NonCommercial-ShareAlike 4.0 International license. Open the
// following URL to see the details.
//
// http://creativecommons.org/licenses/by-nc-sa/4.0/
//------------------------------------------------------------------------------

#include "mem816.h"

mem816::Addr	mem816::memMask;
mem816::Addr	mem816::ramSize;

mem816::Byte   *mem816::pRAM;
const mem816::Byte *mem816::pROM;

//==============================================================================

// Never used.
mem816::mem816()
{ }

// Never used.
mem816::~mem816()
{ }

// Sets up the memory areas using a dynamically allocated array
void mem816::setMemory(Addr memMask, Addr ramSize, const Byte *pROM)
{
	setMemory(memMask, ramSize, new Byte[ramSize], pROM);
}

// Sets up the memory area using pre-allocated array
void mem816::setMemory(Addr memMask, Addr ramSize, Byte *pRAM, const Byte *pROM)
{
	mem816::memMask = memMask;
	mem816::ramSize = ramSize;
	mem816::pRAM = pRAM;
	mem816::pROM = pROM;
}