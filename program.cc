
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

#include "mem816.h"
#include "emu816.h"

//==============================================================================
// Memory Definitions and Access
//------------------------------------------------------------------------------

#define	RAM_SIZE	(384 * 1024)
#define ROM_SIZE	(128 * 1024L)
#define MEM_MASK	(512 * 1024L - 1)

// The ROM area 128K
const wdc816::Byte	ROM[ROM_SIZE] =
{
	0x00, 0x00, 0x00,
};

mem816 mem(MEM_MASK, RAM_SIZE, ROM);
emu816 emu(mem);

//==============================================================================
//------------------------------------------------------------------------------

void setup()
{
	emu.reset();
}

void loop()
{
	emu.step();
}

//==============================================================================
//------------------------------------------------------------------------------

unsigned int toNybble(char ch)
{
	if ((ch >= '0') && (ch <= '9')) return (ch - '0');
	if ((ch >= 'A') && (ch <= 'F')) return (ch - 'A' + 10);
	if ((ch >= 'a') && (ch <= 'f')) return (ch - 'a' + 10);
	return (0);
}

unsigned int toByte(string &str, int &offset)
{
	unsigned int	h = toNybble(str[offset++]) << 4;
	unsigned int	l = toNybble(str[offset++]);

	return (h | l);
}

unsigned long toAddr(string &str, int &offset)
{
	unsigned long	h = toByte(str, offset) << 16;
	unsigned long	m = toByte(str, offset) << 8;
	unsigned long	l = toByte(str, offset);

	return (h | m | l);
}

void load(char *filename)
{
	ifstream	file(filename);
	string	line;

	if (file.is_open()) {
		cout << ">> Loading S28: " << filename << endl;

		while (!file.eof()) {
			file >> line;
			if ((line[0] == 'S') && (line[1] == '2')) {
				int offset = 2;
				unsigned int count = toByte(line, offset);
				unsigned long addr = toAddr(line, offset);
				count -= 4;
				while (count-- > 0) {
					mem.setByte(addr++, toByte(line, offset));
				}
			}
		}
	}

	file.close();
}

int main(int argc, char **argv)
{
	for (int index = 1; index < argc;)
		load(argv[index++]);

	setup();
	for (;;) loop();

	return(0);
}
