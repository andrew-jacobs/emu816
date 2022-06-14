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

#ifndef EMU816_H
#define EMU816_H

#include "mem816.h"

#include <stdlib.h>

#if 1
# define TRACE(MNEM)	{ if (trace) dump(MNEM, ea); }
# define BYTES(N)		{ if (trace) bytes(N); pc += N; }
# define SHOWPC()		{ if (trace) show(); }
# ifdef CHIPKIT
# define ENDL()			{ Serial.println (); }
# else
# define ENDL()			{ if (trace) cout << endl; }
# endif
#else
# define TRACE(MNEM)
# define BYTES(N)		{ pc += N; }
# define SHOWPC()
# define ENDL()
#endif

// Defines the WDC 65C816 emulator. 
class emu816 :
	public mem816
{
public:
	static void reset(bool trace);
	static void step();

	INLINE static unsigned long getCycles()
	{
		return (cycles);
	}

	INLINE static bool isStopped()
	{
		return (stopped);
	}

private:
	static union FLAGS {
		struct {
			Bit				f_c : 1;
			Bit				f_z : 1;
			Bit				f_i : 1;
			Bit				f_d : 1;
			Bit				f_x : 1;
			Bit				f_m : 1;
			Bit				f_v : 1;
			Bit				f_n : 1;
		};
		Byte			b;
	}   p;

	static Bit		e;

	static union REGS {
		Byte			b;
		Word			w;
	}   a, x, y, sp, dp;

	static Word		pc;
	static Byte		pbr, dbr;

	static bool		stopped;
	static bool		interrupted;
	static unsigned long cycles;
	static bool		trace;

	emu816();
	~emu816();

	static void show();
	static void bytes(unsigned int);
	static void dump(const char *, Addr);

	// Push a byte on the stack
	INLINE static void pushByte(Byte value)
	{
		setByte(sp.w, value);

		if (e)
			--sp.b;
		else
			--sp.w;
	}

	// Push a word on the stack
	INLINE static void pushWord(Word value)
	{
		pushByte(hi(value));
		pushByte(lo(value));
	}

	// Pull a byte from the stack
	INLINE static Byte pullByte()
	{
		if (e)
			++sp.b;
		else
			++sp.w;

		return (getByte(sp.w));
	}

	// Pull a word from the stack
	INLINE static Word pullWord()
	{
		register Byte	l = pullByte();
		register Byte	h = pullByte();

		return (join(l, h));
	}

	// Absolute - a
	INLINE static Addr am_absl()
	{
		register Addr	ea = join (dbr, getWord(bank(pbr) | pc));

		BYTES(2);
		cycles += 2;
		return (ea);
	}

	// Absolute - a (fix for 4c and 20)
	INLINE static Addr am_absl2()
	{
		register Addr	ea = join(pbr, getWord(bank(pbr) | pc));

		BYTES(2);
		cycles += 2;
		return (ea);
	}

	// Absolute Indexed X - a,X
	INLINE static Addr am_absx()
	{
		register Addr	ea = join(dbr, getWord(bank(pbr) | pc)) + x.w;

		BYTES(2);
		cycles += 2;
		return (ea);
	}

	// Absolute Indexed Y - a,Y
	INLINE static Addr am_absy()
	{
		register Addr	ea = join(dbr, getWord(bank(pbr) | pc)) + y.w;

		BYTES(2);
		cycles += 2;
		return (ea);
	}

	// Absolute Indirect - (a)
	INLINE static Addr am_absi()
	{
		register Addr ia = join(pbr, getWord(bank(pbr) | pc));

		BYTES(2);
		cycles += 4;
		return (join(0, getWord(ia)));
	}

	// Absolute Indexed Indirect - (a,X)
	INLINE static Addr am_abxi()
	{
		register Addr ia = join(pbr, getWord(join(pbr, pc))) + x.w;

		BYTES(2);
		cycles += 4;
		return (join(pbr, getWord(ia)));
	}

	// Absolute Long - >a
	INLINE static Addr am_alng()
	{
		Addr ea = getAddr(join(pbr, pc));

		BYTES(3);
		cycles += 3;
		return (ea);
	}

	// Absolute Long Indexed - >a,X
	INLINE static Addr am_alnx()
	{
		register Addr ea = getAddr(join(pbr, pc)) + x.w;

		BYTES(3);
		cycles += 3;
		return (ea);
	}

	// Absolute Indirect Long - [a]
	INLINE static Addr am_abil()
	{
		register Addr ia = bank(0) | getWord(join(pbr, pc));

		BYTES(2);
		cycles += 5;
		return (getAddr(ia));
	}

	// Direct Page - d
	INLINE static Addr am_dpag()
	{
		Byte offset = getByte(bank(pbr) | pc);

		BYTES(1);
		cycles += 1;
		return (bank(0) | (Word)(dp.w + offset));
	}

	// Direct Page Indexed X - d,X
	INLINE static Addr am_dpgx()
	{
		Byte offset = getByte(bank(pbr) | pc) + x.b;

		BYTES(1);
		cycles += 1;
		return (bank(0) | (Word)(dp.w + offset));
	}

	// Direct Page Indexed Y - d,Y
	INLINE static Addr am_dpgy()
	{
		Byte offset = getByte(bank(pbr) | pc) + y.b;

		BYTES(1);
		cycles += 1;
		return (bank(0) | (Word)(dp.w + offset));
	}

	// Direct Page Indirect - (d)
	INLINE static Addr am_dpgi()
	{
		Byte disp = getByte(bank(pbr) | pc);

		BYTES(1);
		cycles += 3;
		return (bank(dbr) | getWord(bank(0) | (Word)(dp.w + disp)));
	}

	// Direct Page Indexed Indirect - (d,x)
	INLINE static Addr am_dpix()
	{
		Byte disp = getByte(join(pbr, pc));

		BYTES(1);
		cycles += 3;
		return (bank(dbr) | getWord(bank(0) | (Word)(dp.w + disp + x.w)));
	}

	// Direct Page Indirect Indexed - (d),Y
	INLINE static Addr am_dpiy()
	{
		Byte disp = getByte(join(pbr, pc));

		BYTES(1);
		cycles += 3;
		return (bank(dbr) | getWord(bank(0) | (dp.w + disp)) + y.w);
	}

	// Direct Page Indirect Long - [d]
	INLINE static Addr am_dpil()
	{
		Byte disp = getByte(join(pbr, pc));

		BYTES(1);
		cycles += 4;
		return (getAddr(bank(0) | (Word)(dp.w + disp)));
	}

	// Direct Page Indirect Long Indexed - [d],Y
	INLINE static Addr am_dily()
	{
		Byte disp = getByte(join(pbr, pc));

		BYTES(1);
		cycles += 4;
		return (getAddr(bank(0) | (Word)(dp.w + disp)) + y.w);
	}

	// Implied/Stack
	INLINE static Addr am_impl()
	{
		BYTES(0);
		return (0);
	}

	// Accumulator
	INLINE static Addr am_acc()
	{
		BYTES(0);
		return (0);
	}

	// Immediate Byte
	INLINE static Addr am_immb()
	{
		Addr ea = bank(pbr) | pc;

		BYTES(1);
		cycles += 0;
		return (ea);
	}

	// Immediate Word
	INLINE static Addr am_immw()
	{
		Addr ea = bank(pbr) | pc;

		BYTES(2);
		cycles += 1;
		return (ea);
	}

	// Immediate based on size of A/M
	INLINE static Addr am_immm()
	{
		Addr ea = join (pbr, pc);
		unsigned int size = (e || p.f_m) ? 1 : 2;

		BYTES(size);
		cycles += size - 1;
		return (ea);
	}

	// Immediate based on size of X/Y
	INLINE static Addr am_immx()
	{
		Addr ea = join(pbr, pc);
		unsigned int size = (e || p.f_x) ? 1 : 2;

		BYTES(size);
		cycles += size - 1;
		return (ea);
	}

	// Long Relative - d
	INLINE static Addr am_lrel()
	{
		Word disp = getWord(join(pbr, pc));

		BYTES(2);
		cycles += 2;
		return (bank(pbr) | (Word)(pc + (signed short)disp));
	}

	// Relative - d
	INLINE static Addr am_rela()
	{
		Byte disp = getByte(join(pbr, pc));

		BYTES(1);
		cycles += 1;
		return (bank(pbr) | (Word)(pc + (signed char)disp));
	}

	// Stack Relative - d,S
	INLINE static Addr am_srel()
	{
		Byte disp = getByte(join(pbr, pc));

		BYTES(1);
		cycles += 1;

		if (e)
			return((bank(0) | join(sp.b + disp, hi(sp.w))));
		else
			return (bank(0) | (Word)(sp.w + disp));
	}

	// Stack Relative Indirect Indexed Y - (d,S),Y
	INLINE static Addr am_sriy()
	{
		Byte disp = getByte(join(pbr, pc));
		register Word ia;

		BYTES(1);
		cycles += 3;

		if (e)
			ia = getWord(join(sp.b + disp, hi(sp.w)));
		else
			ia = getWord(bank(0) | (sp.w + disp));

		return (bank(dbr) | (Word)(ia + y.w));
	}

	// Set the Negative flag
	INLINE static void setn(unsigned int flag)
	{
		p.f_n = flag ? 1 : 0;
	}

	// Set the Overflow flag
	INLINE static void setv(unsigned int flag)
	{
		p.f_v = flag ? 1 : 0;
	}

	// Set the decimal flag
	INLINE static void setd(unsigned int flag)
	{
		p.f_d = flag ? 1 : 0;
	}

	// Set the Interrupt Disable flag
	INLINE static void seti(unsigned int flag)
	{
		p.f_i = flag ? 1 : 0;
	}

	// Set the Zero flag
	INLINE static void setz(unsigned int flag)
	{
		p.f_z = flag ? 1 : 0;
	}

	// Set the Carry flag
	INLINE static void setc(unsigned int flag)
	{
		p.f_c = flag ? 1 : 0;
	}

	// Set the Negative and Zero flags from a byte value
	INLINE static void setnz_b(Byte value)
	{
		setn(value & 0x80);
		setz(value == 0);
	}

	// Set the Negative and Zero flags from a word value
	INLINE static void setnz_w(Word value)
	{
		setn(value & 0x8000);
		setz(value == 0);
	}

	INLINE static void op_adc(Addr ea)
	{
		TRACE("ADC");

		if (e || p.f_m) {
			Byte	data = getByte(ea);
			Word	temp = a.b + data + p.f_c;
			
			if (p.f_d) {
				if ((temp & 0x0f) > 0x09) temp += 0x06;
				if ((temp & 0xf0) > 0x90) temp += 0x60;
			}

			setc(temp & 0x100);
			setv((~(a.b ^ data)) & (a.b ^ temp) & 0x80);
			setnz_b(a.b = lo(temp));
			cycles += 2;
		}
		else {
			Word	data = getWord(ea);
			int		temp = a.w + data + p.f_c;

			if (p.f_d) {
				if ((temp & 0x000f) > 0x0009) temp += 0x0006;
				if ((temp & 0x00f0) > 0x0090) temp += 0x0060;
				if ((temp & 0x0f00) > 0x0900) temp += 0x0600;
				if ((temp & 0xf000) > 0x9000) temp += 0x6000;
			}
			
			setc(temp & 0x10000);
			setv((~(a.w ^ data)) & (a.w ^ temp) & 0x8000);
			setnz_w(a.w = (Word)temp);
			cycles += 2;
		}
	}

	INLINE static void op_and(Addr ea)
	{
		TRACE("AND");

		if (e || p.f_m) {
			setnz_b(a.b &= getByte(ea));
			cycles += 2;
		}
		else {
			setnz_w(a.w &= getWord(ea));
			cycles += 3;
		}
	}

	INLINE static void op_asl(Addr ea)
	{
		TRACE("ASL");

		if (e || p.f_m) {
			register Byte data = getByte(ea);

			setc(data & 0x80);
			setnz_b(data <<= 1);
			setByte(ea, data);
			cycles += 4;
		}
		else {
			register Word data = getWord(ea);

			setc(data & 0x8000);
			setnz_w(data <<= 1);
			setWord(ea, data);
			cycles += 5;
		}
	}

	INLINE static void op_asla(Addr ea)
	{
		TRACE("ASL");

		if (e || p.f_m) {
			setc(a.b & 0x80);
			setnz_b(a.b <<= 1);
			//setByte(ea, a.b);
		}
		else {
			setc(a.w & 0x8000);
			setnz_w(a.w <<= 1);
			//setWord(ea, a.w);
		}
		cycles += 2;
	}

	INLINE static void op_bcc(Addr ea)
	{
		TRACE("BCC");

		if (p.f_c == 0) {
			if (e && ((pc ^ ea) & 0xff00)) ++cycles;
			pc = (Word)ea;
			cycles += 3;
		}
		else
			cycles += 2;
	}

	INLINE static void op_bcs(Addr ea)
	{
		TRACE("BCS");

		if (p.f_c == 1) {
			if (e && ((pc ^ ea) & 0xff00)) ++cycles;
			pc = (Word)ea;
			cycles += 3;
		}
		else
			cycles += 2;
	}

	INLINE static void op_beq(Addr ea)
	{
		TRACE("BEQ");

		if (p.f_z == 1) {
			if (e && ((pc ^ ea) & 0xff00)) ++cycles;
			pc = (Word)ea;
			cycles += 3;
		}
		else
			cycles += 2;
	}

	INLINE static void op_bit(Addr ea)
	{
		TRACE("BIT");

		if (e || p.f_m) {
			register Byte data = getByte(ea);

			setz((a.b & data) == 0);
			setn(data & 0x80);
			setv(data & 0x40);
			cycles += 2;
		}
		else {
			register Word data = getWord(ea);

			setz((a.w & data) == 0);
			setn(data & 0x8000);
			setv(data & 0x4000);

			cycles += 3;
		}
	}

	INLINE static void op_biti(Addr ea)
	{
		TRACE("BIT");

		if (e || p.f_m) {
			register Byte data = getByte(ea);

			setz((a.b & data) == 0);
		}
		else {
			register Word data = getWord(ea);

			setz((a.w & data) == 0);
		}
		cycles += 2;
	}

	INLINE static void op_bmi(Addr ea)
	{
		TRACE("BMI");

		if (p.f_n == 1) {
			if (e && ((pc ^ ea) & 0xff00)) ++cycles;
			pc = (Word)ea;
			cycles += 3;
		}
		else
			cycles += 2;
	}

	INLINE static void op_bne(Addr ea)
	{
		TRACE("BNE");

		if (p.f_z == 0) {
			if (e && ((pc ^ ea) & 0xff00)) ++cycles;
			pc = (Word)ea;
			cycles += 3;
		}
		else
			cycles += 2;
	}

	INLINE static void op_bpl(Addr ea)
	{
		TRACE("BPL");

		if (p.f_n == 0) {
			if (e && ((pc ^ ea) & 0xff00)) ++cycles;
			pc = (Word)ea;
			cycles += 3;
		}
		else
			cycles += 2;
	}

	INLINE static void op_bra(Addr ea)
	{
		TRACE("BRA");

		if (e && ((pc ^ ea) & 0xff00)) ++cycles;
		pc = (Word)ea;
		cycles += 3;
	}

	INLINE static void op_brk(Addr ea)
	{
		TRACE("BRK");

		if (e) {
			pushWord(pc);
			pushByte(p.b | 0x10);

			p.f_i = 1;
			p.f_d = 0;
			pbr = 0;

			pc = getWord(0xfffe);
			cycles += 7;
		}
		else {
			pushByte(pbr);
			pushWord(pc);
			pushByte(p.b);

			p.f_i = 1;
			p.f_d = 0;
			pbr = 0;

			pc = getWord(0xffe6);
			cycles += 8;
		}
	}

	INLINE static void op_brl(Addr ea)
	{
		TRACE("BRL");

		pc = (Word)ea;
		cycles += 3;
	}

	INLINE static void op_bvc(Addr ea)
	{
		TRACE("BVC");

		if (p.f_v == 0) {
			if (e && ((pc ^ ea) & 0xff00)) ++cycles;
			pc = (Word)ea;
			cycles += 3;
		}
		else
			cycles += 2;
	}

	INLINE static void op_bvs(Addr ea)
	{
		TRACE("BVS");

		if (p.f_v == 1) {
			if (e && ((pc ^ ea) & 0xff00)) ++cycles;
			pc = (Word)ea;
			cycles += 3;
		}
		else
			cycles += 2;
	}

	INLINE static void op_clc(Addr ea)
	{
		TRACE("CLC");

		setc(0);
		cycles += 2;
	}

	INLINE static void op_cld(Addr ea)
	{
		TRACE("CLD")

		setd(0);
		cycles += 2;
	}

	INLINE static void op_cli(Addr ea)
	{
		TRACE("CLI")

		seti(0);
		cycles += 2;
	}

	INLINE static void op_clv(Addr ea)
	{
		TRACE("CLD")

		setv(0);
		cycles += 2;
	}

	INLINE static void op_cmp(Addr ea)
	{
		TRACE("CMP");

		if (e || p.f_m) {
			Byte	data = getByte(ea);
			Word	temp = a.b - data;

			setc(a.b >= data);
			setnz_b(lo(temp));
			cycles += 2;
		}
		else {
			Word	data = getWord(ea);
			Addr	temp = a.w - data;

			setc(a.w >= data);
			setnz_w((Word)temp);
			cycles += 3;
		}
	}

	INLINE static void op_cop(Addr ea)
	{
		TRACE("COP");

		if (e) {
			pushWord(pc);
			pushByte(p.b);

			p.f_i = 1;
			p.f_d = 0;
			pbr = 0;

			pc = getWord(0xfff4);
			cycles += 7;
		}
		else {
			pushByte(pbr);
			pushWord(pc);
			pushByte(p.b);

			p.f_i = 1;
			p.f_d = 0;
			pbr = 0;

			pc = getWord(0xffe4);
			cycles += 8;
		}
	}

	INLINE static void op_cpx(Addr ea)
	{
		TRACE("CPX");

		if (e || p.f_x) {
			Byte	data = getByte(ea);
			Word	temp = x.b - data;

			setc(x.b >= data);
			setnz_b(lo(temp));
			cycles += 2;
		}
		else {
			Word	data = getWord(ea);
			Addr	temp = x.w - data;

			setc(x.w >= data);
			setnz_w((Word) temp);
			cycles += 3;
		}
	}

	INLINE static void op_cpy(Addr ea)
	{
		TRACE("CPY");

		if (e || p.f_x) {
			Byte	data = getByte(ea);
			Word	temp = y.b - data;

			setc(y.b >= data);
			setnz_b(lo(temp));
			cycles += 2;
		}
		else {
			Word	data = getWord(ea);
			Addr	temp = y.w - data;

			setc(y.w >= data);
			setnz_w((Word) temp);
			cycles += 3;
		}
	}

	INLINE static void op_dec(Addr ea)
	{
		TRACE("DEC");

		if (e || p.f_m) {
			register Byte data = getByte(ea);

			setByte(ea, --data);
			setnz_b(data);
			cycles += 4;
		}
		else {
			register Word data = getWord(ea);

			setWord(ea, --data);
			setnz_w(data);
			cycles += 5;
		}
	}

	INLINE static void op_deca(Addr ea)
	{
		TRACE("DEC");

		if (e || p.f_m)
			setnz_b(--a.b);
		else
			setnz_w(--a.w);

		cycles += 2;
	}

	INLINE static void op_dex(Addr ea)
	{
		TRACE("DEX");

		if (e || p.f_x)
			setnz_b(x.b -= 1);
		else
			setnz_w(x.w -= 1);

		cycles += 2;
	}

	INLINE static void op_dey(Addr ea)
	{
		TRACE("DEY");

		if (e || p.f_x)
			setnz_b(y.b -= 1);
		else
			setnz_w(y.w -= 1);

		cycles += 2;
	}

	INLINE static void op_eor(Addr ea)
	{
		TRACE("EOR");

		if (e || p.f_m) {
			setnz_b(a.b ^= getByte(ea));
			cycles += 2;
		}
		else {
			setnz_w(a.w ^= getWord(ea));
			cycles += 3;
		}
	}

	INLINE static void op_inc(Addr ea)
	{
		TRACE("INC");

		if (e || p.f_m) {
			register Byte data = getByte(ea);

			setByte(ea, ++data);
			setnz_b(data);
			cycles += 4;
		}
		else {
			register Word data = getWord(ea);

			setWord(ea, ++data);
			setnz_w(data);
			cycles += 5;
		}
	}

	INLINE static void op_inca(Addr ea)
	{
		TRACE("INC");

		if (e || p.f_m)
			setnz_b(++a.b);
		else
			setnz_w(++a.w);

		cycles += 2;
	}

	INLINE static void op_inx(Addr ea)
	{
		TRACE("INX");

		if (e || p.f_x)
			setnz_b(++x.b);
		else
			setnz_w(++x.w);

		cycles += 2;
	}

	INLINE static void op_iny(Addr ea)
	{
		TRACE("INY");

		if (e || p.f_x)
			setnz_b(++y.b);
		else
			setnz_w(++y.w);

		cycles += 2;
	}

	INLINE static void op_jmp(Addr ea)
	{
		TRACE("JMP");

		pbr = lo(ea >> 16);
		pc = (Word)ea;
		cycles += 1;
	}

	INLINE static void op_jsl(Addr ea)
	{
		TRACE("JSL");

		pushByte(pbr);
		pushWord(pc - 1);

		pbr = lo(ea >> 16);
		pc = (Word)ea;
		cycles += 5;
	}

	INLINE static void op_jsr(Addr ea)
	{
		TRACE("JSR");

		pushWord(pc - 1);

		pc = (Word)ea;
		cycles += 4;
	}

	INLINE static void op_lda(Addr ea)
	{
		TRACE("LDA");

		if (e || p.f_m) {
			setnz_b(a.b = getByte(ea));
			cycles += 2;
		}
		else {
			setnz_w(a.w = getWord(ea));
			cycles += 3;
		}
	}

	INLINE static void op_ldx(Addr ea)
	{
		TRACE("LDX");

		if (e || p.f_x) {
			setnz_b(lo(x.w = getByte(ea)));
			cycles += 2;
		}
		else {
			setnz_w(x.w = getWord(ea));
			cycles += 3;
		}
	}

	INLINE static void op_ldy(Addr ea)
	{
		TRACE("LDY");

		if (e || p.f_x) {
			setnz_b(lo(y.w = getByte(ea)));
			cycles += 2;
		}
		else {
			setnz_w(y.w = getWord(ea));
			cycles += 3;
		}
	}

	INLINE static void op_lsr(Addr ea)
	{
		TRACE("LSR");

		if (e || p.f_m) {
			register Byte data = getByte(ea);

			setc(data & 0x01);
			setnz_b(data >>= 1);
			setByte(ea, data);
			cycles += 4;
		}
		else {
			register Word data = getWord(ea);

			setc(data & 0x0001);
			setnz_w(data >>= 1);
			setWord(ea, data);
			cycles += 5;
		}
	}

	INLINE static void op_lsra(Addr ea)
	{
		TRACE("LSR");

		if (e || p.f_m) {
			setc(a.b & 0x01);
			setnz_b(a.b >>= 1);
			setByte(ea, a.b);
		}
		else {
			setc(a.w & 0x0001);
			setnz_w(a.w >>= 1);
			setWord(ea, a.w);
		}
		cycles += 2;
	}

	INLINE static void op_mvn(Addr ea)
	{
		TRACE("MVN");

		Byte src = getByte(ea + 1);
		Byte dst = getByte(ea + 0);

		setByte(join(dbr = dst, y.w++), getByte(join(src, x.w++)));
		if (--a.w != 0xffff) pc -= 3;
		cycles += 7;
	}

	INLINE static void op_mvp(Addr ea)
	{
		TRACE("MVP");

		Byte src = getByte(ea + 1);
		Byte dst = getByte(ea + 0);

		setByte(join(dbr = dst, y.w--), getByte(join(src, x.w--)));
		if (--a.w != 0xffff) pc -= 3;
		cycles += 7;
	}

	INLINE static void op_nop(Addr ea)
	{
		TRACE("NOP");

		cycles += 2;
	}

	INLINE static void op_ora(Addr ea)
	{
		TRACE("ORA");

		if (e || p.f_m) {
			setnz_b(a.b |= getByte(ea));
			cycles += 2;
		}
		else {
			setnz_w(a.w |= getWord(ea));
			cycles += 3;
		}
	}

	INLINE static void op_pea(Addr ea)
	{
		TRACE("PEA");

		pushWord(getWord(ea));
		cycles += 5;
	}

	INLINE static void op_pei(Addr ea)
	{
		TRACE("PEI");

		pushWord(getWord(ea));
		cycles += 6;
	}

	INLINE static void op_per(Addr ea)
	{
		TRACE("PER");

		pushWord((Word) ea);
		cycles += 6;
	}

	INLINE static void op_pha(Addr ea)
	{
		TRACE("PHA");

		if (e || p.f_m) {
			pushByte(a.b);
			cycles += 3;
		}
		else {
			pushWord(a.w);
			cycles += 4;
		}
	}

	INLINE static void op_phb(Addr ea)
	{
		TRACE("PHB");

		pushByte(dbr);
		cycles += 3;
	}

	INLINE static void op_phd(Addr ea)
	{
		TRACE("PHD");

		pushWord(dp.w);
		cycles += 4;
	}

	INLINE static void op_phk(Addr ea)
	{
		TRACE("PHK");

		pushByte(pbr);
		cycles += 3;
	}

	INLINE static void op_php(Addr ea)
	{
		TRACE("PHP");

		pushByte(p.b);
		cycles += 3;
	}

	INLINE static void op_phx(Addr ea)
	{
		TRACE("PHX");

		if (e || p.f_x) {
			pushByte(x.b);
			cycles += 3;
		}
		else {
			pushWord(x.w);
			cycles += 4;
		}
	}

	INLINE static void op_phy(Addr ea)
	{
		TRACE("PHY");

		if (e || p.f_x) {
			pushByte(y.b);
			cycles += 3;
		}
		else {
			pushWord(y.w);
			cycles += 4;
		}
	}

	INLINE static void op_pla(Addr ea)
	{
		TRACE("PLA");

		if (e || p.f_m) {
			setnz_b(a.b = pullByte());
			cycles += 4;
		}
		else {
			setnz_w(a.w = pullWord());
			cycles += 5;
		}
	}

	INLINE static void op_plb(Addr ea)
	{
		TRACE("PLB");

		setnz_b(dbr = pullByte());
		cycles += 4;
	}

	INLINE static void op_pld(Addr ea)
	{
		TRACE("PLD");

		setnz_w(dp.w = pullWord());
		cycles += 5;
	}

	INLINE static void op_plk(Addr ea)
	{
		TRACE("PLK");

		setnz_b(dbr = pullByte());
		cycles += 4;
	}

	INLINE static void op_plp(Addr ea)
	{
		TRACE("PLP");

		if (e)
			p.b = pullByte() | 0x30;
		else {
			p.b = pullByte();

			if (p.f_x) {
				x.w = x.b;
				y.w = y.b;
			}
		}
		cycles += 4;
	}

	INLINE static void op_plx(Addr ea)
	{
		TRACE("PLX");

		if (e || p.f_x) {
			setnz_b(lo(x.w = pullByte()));
			cycles += 4;
		}
		else {
			setnz_w(x.w = pullWord());
			cycles += 5;
		}
	}

	INLINE static void op_ply(Addr ea)
	{
		TRACE("PLY");

		if (e || p.f_x) {
			setnz_b(lo(y.w = pullByte()));
			cycles += 4;
		}
		else {
			setnz_w(y.w = pullWord());
			cycles += 5;
		}
	}

	INLINE static void op_rep(Addr ea)
	{
		TRACE("REP");

		p.b &= ~getByte(ea);
		if (e) p.f_m = p.f_x = 1;
		cycles += 3;
	}

	INLINE static void op_rol(Addr ea)
	{
		TRACE("ROL");

		if (e || p.f_m) {
			register Byte data = getByte(ea);
			register Byte carry = p.f_c ? 0x01 : 0x00;

			setc(data & 0x80);
			setnz_b(data = (data << 1) | carry);
			setByte(ea, data);
			cycles += 4;
		}
		else {
			register Word data = getWord(ea);
			register Word carry = p.f_c ? 0x0001 : 0x0000;

			setc(data & 0x8000);
			setnz_w(data = (data << 1) | carry);
			setWord(ea, data);
			cycles += 5;
		}
	}

	INLINE static void op_rola(Addr ea)
	{
		TRACE("ROL");

		if (e || p.f_m) {
			register Byte carry = p.f_c ? 0x01 : 0x00;

			setc(a.b & 0x80);
			setnz_b(a.b = (a.b << 1) | carry);
		}
		else {
			register Word carry = p.f_c ? 0x0001 : 0x0000;

			setc(a.w & 0x8000);
			setnz_w(a.w = (a.w << 1) | carry);
		}
		cycles += 2;
	}

	INLINE static void op_ror(Addr ea)
	{
		TRACE("ROR");

		if (e || p.f_m) {
			register Byte data = getByte(ea);
			register Byte carry = p.f_c ? 0x80 : 0x00;

			setc(data & 0x01);
			setnz_b(data = (data >> 1) | carry);
			setByte(ea, data);
			cycles += 4;
		}
		else {
			register Word data = getWord(ea);
			register Word carry = p.f_c ? 0x8000 : 0x0000;

			setc(data & 0x0001);
			setnz_w(data = (data >> 1) | carry);
			setWord(ea, data);
			cycles += 5;
		}
	}

	INLINE static void op_rora(Addr ea)
	{
		TRACE("ROR");

		if (e || p.f_m) {
			register Byte carry = p.f_c ? 0x80 : 0x00;

			setc(a.b & 0x01);
			setnz_b(a.b = (a.b >> 1) | carry);
		}
		else {
			register Word carry = p.f_c ? 0x8000 : 0x0000;

			setc(a.w & 0x0001);
			setnz_w(a.w = (a.w >> 1) | carry);
		}
		cycles += 2;
	}

	INLINE static void op_rti(Addr ea)
	{
		TRACE("RTI");

		if (e) {
			p.b = pullByte();
			pc = pullWord();
			cycles += 6;
		}
		else {
			p.b = pullByte();
			pc = pullWord();
			pbr = pullByte();
			cycles += 7;
		}
	}

	INLINE static void op_rtl(Addr ea)
	{
		TRACE("RTL");

		pc = pullWord() + 1;
		pbr = pullByte();
		cycles += 6;
	}

	INLINE static void op_rts(Addr ea)
	{
		TRACE("RTS");

		pc = pullWord() + 1;
		cycles += 6;
	}

	INLINE static void op_sbc(Addr ea)
	{
		TRACE("SBC");

		if (e || p.f_m) {
			Byte	data = ~getByte(ea);
			Word	temp = a.b + data + p.f_c;
			
			if (p.f_d) {
				if ((temp & 0x0f) > 0x09) temp += 0x06;
				if ((temp & 0xf0) > 0x90) temp += 0x60;
			}

			setc(temp & 0x100);
			setv((~(a.b ^ data)) & (a.b ^ temp) & 0x80);
			setnz_b(a.b = lo(temp));
			cycles += 2;
		}
		else {
			Word	data = ~getWord(ea);
			int		temp = a.w + data + p.f_c;

			if (p.f_d) {
				if ((temp & 0x000f) > 0x0009) temp += 0x0006;
				if ((temp & 0x00f0) > 0x0090) temp += 0x0060;
				if ((temp & 0x0f00) > 0x0900) temp += 0x0600;
				if ((temp & 0xf000) > 0x9000) temp += 0x6000;
			}

			setc(temp & 0x10000);
			setv((~(a.w ^ data)) & (a.w ^ temp) & 0x8000);
			setnz_w(a.w = (Word)temp);
			cycles += 3;
		}
	}

	INLINE static void op_sec(Addr ea)
	{
		TRACE("SEC");

		setc(1);
		cycles += 2;
	}

	INLINE static void op_sed(Addr ea)
	{
		TRACE("SED");

		setd(1);
		cycles += 2;
	}

	INLINE static void op_sei(Addr ea)
	{
		TRACE("SEI");

		seti(1);
		cycles += 2;
	}

	INLINE static void op_sep(Addr ea)
	{
		TRACE("SEP");

		p.b |= getByte(ea);
		if (e) p.f_m = p.f_x = 1;

		if (p.f_x) {
			x.w = x.b;
			y.w = y.b;
		}
		cycles += 3;
	}

	INLINE static void op_sta(Addr ea)
	{
		TRACE("STA");

		if (e || p.f_m) {
			setByte(ea, a.b);
			cycles += 2;
		}
		else {
			setWord(ea, a.w);
			cycles += 3;
		}
	}

	INLINE static void op_stp(Addr ea)
	{
		TRACE("STP");

		if (!interrupted) {
			pc -= 1;
		}
		else
			interrupted = false;

		cycles += 3;
	}

	INLINE static void op_stx(Addr ea)
	{
		TRACE("STX");

		if (e || p.f_x) {
			setByte(ea, x.b);
			cycles += 2;
		}
		else {
			setWord(ea, x.w);
			cycles += 3;
		}
	}

	INLINE static void op_sty(Addr ea)
	{
		TRACE("STY");

		if (e || p.f_x) {
			setByte(ea, y.b);
			cycles += 2;
		}
		else {
			setWord(ea, y.w);
			cycles += 3;
		}
	}

	INLINE static void op_stz(Addr ea)
	{
		TRACE("STZ");

		if (e || p.f_m) {
			setByte(ea, 0);
			cycles += 2;
		}
		else {
			setWord(ea, 0);
			cycles += 3;
		}
	}

	INLINE static void op_tax(Addr ea)
	{
		TRACE("TAX");

		if (e || p.f_x)
			setnz_b(lo(x.w = a.b));
		else
			setnz_w(x.w = a.w);

		cycles += 2;
	}

	INLINE static void op_tay(Addr ea)
	{
		TRACE("TAY");

		if (e || p.f_x)
			setnz_b(lo(y.w = a.b));
		else
			setnz_w(y.w = a.w);

		cycles += 2;
	}

	INLINE static void op_tcd(Addr ea)
	{
		TRACE("TCD");

		dp.w = a.w;
		cycles += 2;
	}

	INLINE static void op_tdc(Addr ea)
	{
		TRACE("TDC");

		if (e || p.f_m)
			setnz_b(lo(a.w = dp.w));
		else
			setnz_w(a.w = dp.w);

		cycles += 2;
	}

	INLINE static void op_tcs(Addr ea)
	{
		TRACE("TCS");

		sp.w = e ? (0x0100 | a.b) : a.w;
		cycles += 2;
	}

	INLINE static void op_trb(Addr ea)
	{
		TRACE("TRB");

		if (e || p.f_m) {
			register Byte data = getByte(ea);

			setByte(ea, data & ~a.b);
			setz((a.b & data) == 0);
			cycles += 4;
		}
		else {
			register Word data = getWord(ea);

			setWord(ea, data & ~a.w);
			setz((a.w & data) == 0);
			cycles += 5;
		}
	}

	INLINE static void op_tsb(Addr ea)
	{
		TRACE("TSB");

		if (e || p.f_m) {
			register Byte data = getByte(ea);

			setByte(ea, data | a.b);
			setz((a.b & data) == 0);
			cycles += 4;
		}
		else {
			register Word data = getWord(ea);

			setWord(ea, data | a.w);
			setz((a.w & data) == 0);
			cycles += 5;
		}
	}

	INLINE static void op_tsc(Addr ea)
	{
		TRACE("TSC");

		if (e || p.f_m)
			setnz_b(lo(a.w = sp.w));
		else
			setnz_w(a.w = sp.w);

		cycles += 2;
	}

	INLINE static void op_tsx(Addr ea)
	{
		TRACE("TSX");

		if (e)
			setnz_b(x.b = sp.b);
		else
			setnz_w(x.w = sp.w);

		cycles += 2;
	}

	INLINE static void op_txa(Addr ea)
	{
		TRACE("TXA");

		if (e || p.f_m)
			setnz_b(a.b = x.b);
		else
			setnz_w(a.w = x.w);

		cycles += 2;
	}

	INLINE static void op_txs(Addr ea)
	{
		TRACE("TXS");

		if (e)
			sp.w = 0x0100 | x.b;
		else
			sp.w = x.w;

		cycles += 2;
	}

	INLINE static void op_txy(Addr ea)
	{
		TRACE("TXY");

		if (e || p.f_x)
			setnz_b(lo(y.w = x.w));
		else
			setnz_w(y.w = x.w);

		cycles += 2;
	}

	INLINE static void op_tya(Addr ea)
	{
		TRACE("TYA");

		if (e || p.f_m)
			setnz_b(a.b = y.b);
		else
			setnz_w(a.w = y.w);

		cycles += 2;
	}

	INLINE static void op_tyx(Addr ea)
	{
		TRACE("TYX");

		if (e || p.f_x)
			setnz_b(lo(x.w = y.w));
		else
			setnz_w(x.w = y.w);

		cycles += 2;
	}

	INLINE static void op_wai(Addr ea)
	{
		TRACE("WAI");

		if (!interrupted) {
			pc -= 1;
		}
		else
			interrupted = false;

		cycles += 3;
	}

	INLINE static void op_wdm(Addr ea)
	{
		TRACE("WDM");

		switch (getByte(ea)) {
		case 0x01:	cout << (char) a.b; break;
		case 0x02:  cin >> a.b; break;
		case 0xff:	stopped = true;  break;
		}
		cycles += 3;
	}

	INLINE static void op_xba(Addr ea)
	{
		TRACE("XBA");

		a.w = swap(a.w);
		setnz_b(a.b);
		cycles += 3;
	}

	INLINE static void op_xce(Addr ea)
	{
		TRACE("XCE");

		unsigned char	oe = e;

		e = p.f_c;
		p.f_c = oe;

		if (e) {
			p.b |= 0x30;
			sp.w = 0x0100 | sp.b;
		}
		cycles += 2;
	}
};
#endif
