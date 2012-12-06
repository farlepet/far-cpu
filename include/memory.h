#include <common.h>

#ifndef MEMORY_H
#define MEMORY_H

typedef struct
{
	u8int AB;  //------|----------------------|
	u8int BB;  //      |--> byte regs         |
	u8int CB;  //      |                      |
	u8int DB;  //------|                      |
	u16int AS; //-----|                       |
	u16int BS; //     |                       |-----> General Purpose Registers (Math, MOV's, etc...)
	u16int CS; //     |--> word regs          |
	u16int DS; //-----|                       |
	u32int AL; //----|                        |
	u32int BL; //    |                        |
	u32int CL; //    |-----> double word regs |
	u32int DL; //----|------------------------|

	u32int PC; //Program Counter/Instruction Pointer/Where the Instruction (IR) Is
	u8int IR; //Instruction
	u32int SP; //Stack Pointer
	u16int CSP; //Current Stack Position
	
} registers;

typedef enum
{
	AB = 0,
	BB,
	CB,
	DB,
	AS,
	BS,
	CS,
	DS,
	AL,
	BL,
	CL,
	DL,

	PC,
	IR,
	SP,
	CSP
} Register;

u8int reg_sizes[16];

typedef enum
{
	REGISTER,
	NUMBER,  //DWORD
	IO,
	MEMORY
} input_location;

int setup_memory(char *mem, u32int size);

void remove_memory(int mem);

void end_memory();

#define mem_get8(mem, loc) *(u8int *)(mem + loc)
#define mem_get16(mem, loc) *(u16int *)(mem + loc)
#define mem_get32(mem, loc) *(u32int *)(mem + loc)
#define mem_get64(mem, loc) *(u64int *)(mem + loc)

u8int mem_read8(char *mem, u32int loc);
u16int mem_read16(char *mem, u32int loc);
u32int mem_read32(char *mem, u32int loc);
u64int mem_read64(char *mem, u32int loc);

void mem_write8(char *mem, u32int loc, u8int in);
void mem_write16(char *mem, u32int loc, u16int in);
void mem_write32(char *mem, u32int loc, u32int in);
void mem_write64(char *mem, u32int loc, u64int in);

#endif