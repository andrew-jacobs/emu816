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

#ifndef MEM816_H
#define MEM816_H

#include "wdc816.h"

class mem816 :
	public wdc816
{
public:
	mem816(Addr memMask, Addr ramSize, const Byte *pROM);
	mem816(Addr memMask, Addr ramSize, Byte *pRAM, const Byte *pROM);
	~mem816();

	INLINE Byte getByte(Addr ea)
	{
		if ((ea &= memMask) < ramSize)
			return (pRAM[ea]);

		return (pROM[ea - ramSize]);
	}

	INLINE Word getWord(Addr ea)
	{
		return (join(getByte(ea + 0), getByte(ea + 1)));
	}

	INLINE Addr getAddr(Addr ea)
	{
		return (join(getByte(ea + 2), getWord(ea + 0)));
	}

	INLINE void setByte(Addr ea, Byte data)
	{
		if ((ea &= memMask) < ramSize)
			pRAM[ea] = data;
	}

	INLINE void setWord(Addr ea, Word data)
	{
		setByte(ea + 0, lo(data));
		setByte(ea + 1, hi(data));
	}

private:
	const Addr	memMask;
	const Addr	ramSize;

	Byte	   *pRAM;
	const Byte *pROM;

	const bool	freeRAM;
};
#endif
