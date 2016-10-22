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

#ifndef WDC816_H
#define WDC816_H

#ifdef CHIPKIT
# define INLINE inline
#else
# define INLINE inline
#endif

// The wdc816 class defines common types for 8-, 16- and 24-bit data values and
// a set of common functions for manipulating them.

class wdc816
{
public:
	// Common types for memory and register sizes
	typedef unsigned char	Bit;
	typedef unsigned char	Byte;
	typedef unsigned short	Word;
	typedef unsigned long	Addr;

	// Convert a value to a hex string.
	static char *toHex(unsigned long value, unsigned int digits);

	// Return the low byte of a word
	INLINE static Byte lo(Word value)
	{
		return ((Byte) value);
	}

	// Return the high byte of a word
	INLINE static Byte hi(Word value)
	{
		return (lo(value >> 8));
	}

	// Convert the bank number into a address
	INLINE static Addr bank(Byte b)
	{
		return (b << 16);
	}

	// Combine two bytes into a word
	INLINE static Word join(Byte l, Byte h)
	{
		return (l | (h << 8));
	}

	// Combine a bank and an word into an address
	INLINE static Addr join(Byte b, Word a)
	{
		return (bank(b) | a);
	}

	// Swap the high and low bytes of a word
	INLINE static Word swap(Word value)
	{
		return ((value >> 8) | (value << 8));
	}

protected:
	wdc816();
	~wdc816();
};
#endif