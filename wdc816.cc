#include "wdc816.h"

wdc816::wdc816()
{ }

wdc816::~wdc816()
{ }

char *wdc816::toHex(unsigned long value, unsigned int digits)
{
	static char buffer[16];
	unsigned int offset = sizeof(buffer);;

	buffer[--offset] = 0;
	while (digits-- > 0) {
		buffer[--offset] = "0123456789ABCDEF"[value & 0xf];
		value >>= 4;
	}
	return (&(buffer[offset]));
}
