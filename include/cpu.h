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

const char *n_to_instruction[256];

typedef enum
{
	NOP = 0, //No operation
	INC, //Increment, takes 1 byte: registers
	DEC, //Opposite of INC
	ADD, //Add 2 numbers and put into AL, takes 4 inputs: (INPUT, (depending on input))*2
	SUB, //Opposite of ADD
	MUL, //Same as ADD, but multiplication
	DIV, //Same as ADD, but division
	
	MOVNM, //Move Number to Memory: byte(size of number: 0:byte 1:word 2:dword), (byte/word/dword), qword(memory loc)
	MOVRM, //Move Register to Memory: Register, qword
	MOVIM, //Move Input into Memory: qword
	MOVMM, //Move Memory into Memory: qword, qword

	MOVMR, //Move Memory into Register: qword, Register
	MOVNR, //Move Number into Register: byte(size of number: 0:byte 1:word 2:dword), (byte/word/dword), Register
	MOVIR, //Move Input into Register: Register
	MOVRR, //Move Register[0] into Register[1]: Register, Register
	
	SHL, //Shifts left [1,2] [3] times and stores into AL: (INPUT, (depending on input)), byte
	SHR, //Opposite of SHL
	AND, //AND and store into AL: (INPUT, (depending on input))*2
	OR,  //Similar to AND
	XOR, //Similar to AND
	NOT, //nots value and stores in AL
	
	HWU, //Hardware update: word(not byte, for future expansion capabilities)

	JMP, //Jump to specified point in program: dword

	SJP, //Set Jump Point - used for the following J*** comparisons: long
	
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


	RET, //Return with value of AL

	EXOP = 255 //if this requires an extra byte, so this CPU allows up to 512 opcodes, this probably wont be used for quite some time........ if ever.................................
} cpu_opcode;

u32int process_opcode(farcpu *cpu);

u32int process_extended_opcode(farcpu *cpu);

u32int process_in_loc(farcpu *cpu, char *mem, u32int loc, u8int *madd);

u32int get_register(farcpu *cpu, Register reg);

void set_register(farcpu *cpu, Register reg, u32int val);

u32int cpu_do_div(u32int a, u32int b);

#endif