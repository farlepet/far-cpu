#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <stdbool.h>

#ifndef COMMON_H
#define COMMON_H

#define DEBUG

#ifdef DEBUG
#define D printf
#define FLS() fflush(stdout)
#else
#define D(str, ...)
#endif

typedef unsigned char      u8int;
typedef unsigned short     u16int;
typedef unsigned long      u32int;
typedef unsigned long long u64int;

typedef char      s8int;
typedef short     s16int;
typedef long      s32int;
typedef long long s64int;

double makeSmall(u32int InBytes, char *out);

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
	u32int JP; //Jump Pointer used for comparison jumping
	
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
	CSP,
	JP
} Register;

typedef struct
{
	char *memory;
	u32int memory_size;
	registers regs;
	u8int IO;
	bool inOut; //wether IO is an input or an output (true = input)
	bool overflow;
	u16int speed; //not yet inplimented, it will be a while
} farcpu;

#endif