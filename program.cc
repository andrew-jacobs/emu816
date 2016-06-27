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

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

#include <string.h>

#include "mem816.h"
#include "emu816.h"

//==============================================================================
// Memory Definitions
//------------------------------------------------------------------------------

// On Windows/Linux create a 512Kb RAM area - No ROM.
#define	RAM_SIZE	(512 * 1024)
#define MEM_MASK	(512 * 1024L - 1)

mem816 mem(MEM_MASK, RAM_SIZE, NULL);
emu816 emu(mem);

bool trace = false;

//==============================================================================

// Initialise the emulator
void setup()
{
	emu.reset(trace);
}

// Execute instructions
void loop()
{
	emu.step();
}

//==============================================================================
// S28 Record Loader
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

//==============================================================================
// Command Handler
//------------------------------------------------------------------------------

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
		file.close();
	}
	else
		cerr << "Failed to open file" << endl;

}

int main(int argc, char **argv)
{
	int	index = 1;

	while (index < argc) {
		if (argv[index][0] != '-') break;

		if (!strcmp(argv[index], "-t")) {
			trace = true;
			++index;
			continue;
		}

		if (!strcmp(argv[index], "-?")) {
			cerr << "Usage: emu816 [-t] s28-file ..." << endl;
			return (1);
		}

		cerr << "Invalid: option '" << argv[index] << "'" << endl;
		return (1);
	}

	if (index < argc)
		do {
			load(argv[index++]);
		} while (index < argc);
	else {
		cerr << "No S28 files specified" << endl;
		return (1);
	}

	setup();
	for (;;) loop();

	return(0);
}
