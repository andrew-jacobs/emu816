# emu816 - A C++ based 65C816 Emulator

The repository contains the source code for a simple 65C816 emulator for Windows,
Linux and the embedded ChipKIT platform.

This is the first release of the code and it has only had a limited amount of
testing. Currently there is no support for decimal arithmetic but the full
instruction set is supported.

There is no I/O at the moment or source of interrupts. Executing a WDM #$FF will
cause the emulator to exit.

## Building

The code is provided with a Visual Studio project for Windows and a Makefile for
Linux plaforms.

A (very) simple example built with my DEV65 assembler is provided in the examples
folder. Use the following command to run it.

```
emu816 -t examples/simple/simple.s28
```
