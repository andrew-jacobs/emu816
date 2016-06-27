#ifndef MEM816
#define MEM816
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
		return (join(getByte(ea + 0), getWord(ea + 0)));
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
};
#endif
