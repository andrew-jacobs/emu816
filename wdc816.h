#ifndef WDC816
#define WDC816

//#define CHIPKIT

#ifdef CHIPKIT
#define INLINE inline
#else
#define INLINE inline
#endif

class wdc816
{
public:
	// Common types for memory and register sizes
	typedef unsigned char Bit;
	typedef unsigned char Byte;
	typedef unsigned short Word;
	typedef unsigned long Addr;

	wdc816 ();
	~wdc816();

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
};
#endif
