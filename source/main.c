/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * main.c
 * Copyright (C) 2012 Peter Farley <far.peter1@gmail.com>
 * 
 */

#define USAGE "Usage:\n\
    farcpu [mem_size]\n\
        mem_size: size of the virtal CPU's RAM\n"
int ver[3] = {0,0,1};

#include <stdio.h>
#include <common.h>
#include <memory.h>
#include <cpu.h>
#include <math.h>

char test_program[12] = 
{
	INC, AL, INC, AL, //Add 2 to AL, the long way
	DEC, AB, DEC, AB, //Sub 2 from AB, the long way, will loob from 0 to 255 when subtracted
	0,0,0,0
};

int main(int argc, char *argv[])
{
	

	printf("far-cpu v%d.%d.%d\n", ver[0], ver[1], ver[2]);
	if(argc < 2){ printf(USAGE); return -1; }

	double smem; char *type = malloc(1);
	
	
	printf("initilizing cpu 1...\n");
	farcpu cpu1; init_cpu(&cpu1, atoi(argv[1]));
	printf("\tMemory location: 0x%lX\n", (u32int)(cpu1.memory));
	smem = makeSmall(cpu1.memory_size, type);
	printf("\tMemory size: %.2f%c\n", smem, *type);
	printf("Copying program to memory at 0x0\n");
	int i;
	for(i = 0; i < 12; i++)
	{
		cpu1.memory[i] = test_program[i];
		printf("%d:%d, ", test_program[i], cpu1.memory[i]);
	}
	//memcpy(cpu1.memory, test_program, 24);
	printf("\nExecuting Test Program:\n");
	
	for(i = 0; i < 0x8; i++)
	{
		printf("0x%X, ", mem_get8(cpu1.memory, cpu1.regs.PC));
		cpu1.regs.IR = mem_get8(cpu1.memory, cpu1.regs.PC);
		process_opcode(&cpu1);
	}
	printf("\nAL:0x%lX BL:0x%lX AB:0x%X BB:0x%X\n", cpu1.regs.AL, cpu1.regs.BL, cpu1.regs.AB, cpu1.regs.BB);
	return 0;
}

