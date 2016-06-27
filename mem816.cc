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
// Copyright (C)2016 Andrew John Jacobs
// All rights reserved.
//
// This work is made available under the terms of the Creative Commons
// Attribution-NonCommercial-ShareAlike 4.0 International license. Open the
// following URL to see the details.
//
// http://creativecommons.org/licenses/by-nc-sa/4.0/
//------------------------------------------------------------------------------

#include "mem816.h"

//==============================================================================

// Construct a memory area using dynamic allocation
mem816::mem816(Addr memMask, Addr ramSize, const Byte *pROM)
	: memMask(memMask), ramSize(ramSize), pROM(pROM), freeRAM(true)
{
	pRAM = new Byte[ramSize];
}

// Construct a memory area using statically allocated arrays
mem816::mem816(Addr memMask, Addr ramSize, Byte *pRAM, const Byte *pROM)
	: memMask(memMask), ramSize(ramSize), pRAM(pRAM), pROM(pROM), freeRAM(false)
{ }

// Destory an instance releasing the RAM if allocated.
mem816::~mem816()
{
	if (freeRAM) delete pRAM;
}