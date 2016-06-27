
#include "mem816.h"

mem816::mem816(Addr memMask, Addr ramSize, const Byte *pROM)
	: memMask(memMask), ramSize(ramSize), pROM(pROM)
{
	pRAM = new Byte[ramSize];
}

mem816::mem816(Addr memMask, Addr ramSize, Byte *pRAM, const Byte *pROM)
	: memMask(memMask), ramSize(ramSize), pRAM (pRAM), pROM(pROM)
{ }

mem816::~mem816()
{ }
