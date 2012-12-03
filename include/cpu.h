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
} cpu_speed; //operations per second

typedef struct
{
	char *memory;
	u32int memory_size;
	registers regs;
	cpu_speed speed; //not yet inplimented, will be a while
} farcpu;

int init_cpu(farcpu *cpu);

#endif