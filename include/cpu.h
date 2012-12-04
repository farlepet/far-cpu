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
	NOP = 0, //No operation
	INC, //Increment, takes 1 byte: registers
	DEC, //Opposite of INC
	ADD, //Add 2 numbers and put into AL, takes 4 inputs: (INPUT, (depending on input))*2
	SUB, //Opposite of ADD
	MUL, //Same as ADD, but multiplication
	DIV, //Same as ADD, but division
	
	MOVNM, //Move Number to Memory: byte(size of number: 0:byte 1:word 2:dword), (byte/word/dword), dword
	MOVRM, //Move Register to Memory: Register, dword
	MOVIM, //Move Input into Memory: dword
	MOVMM, //Move Memory into Memory: dword, dword

	MOVMR, //Move Memory into Register: dword, Register
	MOVNR, //Move Number into Register: byte(size of number: 0:byte 1:word 2:dword), (byte/word/dword), Register
	MOVIR, //Move Input into Register: Register
	
	SHL, //Shifts left [1] [3] times: (INPUT, (depending on input)), (INPUT, (depending on input))
	SHR, //Opposite of SHL
	HWU, //Hardware update: word(for future expansion capabilities)

	JMP, //Jump to specified point in program: dword
	
	JZ,  //Jump only if AL == 0
	JNZ, //Jump only if AL != 0
	
	JFE, //Jump only if AL == BL
	JNE, //Jump only if AL != BL
	
	JGT, //Jump only if AL > BL
	JNG, //Jump only if AL !> BL
	JGE, //Jump only if AL >= BL
	JNGE, //Jump only if AL !>= BL
	
	JLT, //Jump only if AL < BL
	JNL, //Jump only if AL !< BL
	JLE, //Jump only if AL <= BL
	JNLE, //Jump only if AL !<= BL
	
} cpu_opcode;

#endif