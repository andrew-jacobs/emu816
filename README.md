# emu816 - A C++ based 65C816 Emulator

The repository contains the source code for a simple 65C816 emulator for Windows,
Linux and the embedded ChipKIT platform.

The major change since the last release has been the recoding of all the classes
to make them static. This was done to increase the execution performance of the
code. On my development laptop (AMD8 1.8GHz) it now runs at an emulated speed of
around 225 MHz with full optimization.

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
