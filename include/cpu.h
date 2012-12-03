#include <common.h>
#include <memory.h>

#ifndef CPU_H
#define CPU_H

typedef enum
{
	OP1S,
	OP2S,
	OP4S,
	OP8S,
	OP16S,
	OP32S,
	OP64S,
	OP128S,
	OP256S,
	OP512S,
	OP1024S,
	OP2048S,
	OP4096S,
	UNLIMITED
} cpu_speed; //maximum operations per second

typedef struct
{
	char *memory;
	u32int memory_size;
	registers regs;
	u8int IO;
	bool inOut; //wether IO is an input or an output (true = input)
	bool overflow;
	cpu_speed speed; //not yet inplimented, it will be a while
} farcpu;

void cpu_reset(farcpu *cpu);

int init_cpu(farcpu *cpu, u32int mem_size);

typedef enum
{
	NOP, //No operation
	INC, //Increment, takes 1 byte: registers
	DEC, //Opposite of INC
	ADD, //Add 2 numbers and put into AL, takes 4 inputs: (INPUT, (dependion on input))*2
	SUB, //Opposite of ADD
	MUL, //Same as ADD, but multiplication
	DIV, //Same as ADD, but division
	MOVNM, //Move Number to Memory, byte, dword
	MOVRM, //Move Register to Memory, Register, dword
	MOVIM, //Move Input into Memory
	MOVMM, //Move Memory into Memory
	SHL,
	SHR,
	HWU, //Hardware update
} cpu_opcode;

#endif