
CPPFLAGS=-O3

all:	emu816

clean:
	$(RM) *.o
	$(RM) emu816

emu816:	wdc816.o emu816.o mem816.o program.o
	g++ wdc816.o emu816.o mem816.o program.o -o emu816

wdc816.o: \
	wdc816.cc wdc816.h

emu816.o: \
	emu816.cc emu816.h wdc816.h mem816.h

mem816.o: \
	mem816.cc mem816.h wdc816.h

program.o: \
	program.cc emu816.h mem816.h wdc816.h
