/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * main.c
 * Copyright (C) 2012 Peter Farley <far.peter1@gmail.com>
 * 
 */

#define USAGE "Usage:\n\
   farcpu [mem_size] <program>\n\
      mem_size: size of the virtal CPU's RAM\n\
	  program: assembled farcpu program file\n\
   ex: farcpu 64 test1.o\n"

#define ROMMEMLOC
		
int ver[3] = {0,2,0};

#include <stdio.h>
#include <common.h>
#include <memory.h>
#include <cpu.h>
#include <gfx.h>
#include <math.h>

char test_program[] = 
{
	INC, AB, INC, AB, //Sub 2 from AB, the long way, will loob from 0 to 255 when subtracted
	DEC, BL,
	INC, BB,
	DIV, NUMBER, 1, 0, NUMBER, 0, 4,//Add 2 to AL, the long way
	INC, BB, INC, BB, INC, BB,
	MOVRR, AL, BL,
	MOVNM, 0, 255, 0, 0, 0, 0,
	JMP, 0, 0, 0, 0 //run main program
};
#define prgm_sz (u32int)(sizeof(test_program) / sizeof(test_program[0]))

int main(int argc, char *argv[])
{
	

	printf("far-cpu v%d.%d.%d\n", ver[0], ver[1], ver[2]);
	if(argc < 2){ printf(USAGE); return -1; }
	D("This is a build with DEBUG features included.\n");

	double smem; char *type = malloc(1); u32int numbytes = 0;
	init_gfx();
	
	printf("initilizing cpu 1...\n");
	farcpu cpu1; init_cpu(&cpu1, atoi(argv[1]));
	printf("\tMemory location: 0x%lX\n", (u32int)(cpu1.memory));
	smem = makeSmall(cpu1.memory_size, type);
	printf("\tMemory size: %.2f%c\n", smem, *type);
	smem = makeSmall(prgm_sz, type);
	printf("\tSystem ROM Size: %.2f%c\n", smem, *type);
	u32int i;
	D("opening file\n");FLS();
	
	if(argc > 2)
	{
		FILE *f;
		if((f = fopen(argv[2], "r")) == NULL) {	printf("ERR:FILE %s COULD NOT BE OPENED!\n", argv[2]); return -1;	}
		fseek(f, 0L, SEEK_END);
		numbytes = ftell(f);
		fseek(f, 0L, SEEK_SET);	
		smem = makeSmall(numbytes, type);
		if(cpu1.memory_size <  numbytes){ printf("\nERR:NOT ENOUGH ALLOCATED CPU MEMORY TO RUN PROGRAM!!! NEEDS %lf%c\n\n", smem, *type); return -3; }
		D("copying\n");FLS();
		fread(cpu1.memory, 1, numbytes, f);
		D("copied\n");FLS();
		fclose(f);
	}
	
	else {
		if(cpu1.memory_size < prgm_sz){ printf("\nERR:NOT ENOUGH ALLOCATED CPU MEMORY TO RUN PROGRAM!!!\n\n"); return -2; } 
		for(i = 0; i < prgm_sz; i++)
		{
			cpu1.memory[i] = test_program[i];
			printf("%d:%d, ", test_program[i], cpu1.memory[i]);
		}
	}
	//memcpy(cpu1.memory, test_program, 24);
	printf("\nExecuting Program:\n");
	while(cpu1.regs.PC < (argc < 3) ? prgm_sz : numbytes)
	{
		printf("%lX:%s, ", cpu1.regs.PC, n_to_instruction[mem_get8(cpu1.memory, cpu1.regs.PC)]); fflush(stdout);
		cpu1.regs.IR = mem_read8(cpu1.memory, cpu1.regs.PC);
		if(cpu1.regs.IR == RET) { printf("\n"); break; } //temporary only, as it will probably be used when functions are implemented
		process_opcode(&cpu1);
		gfx_upd();
	}
	scanf("lol");
	printf("%d", mem_read8(cpu1.memory, 256));
	printf("\nAL:0x%lX BL:0x%lX AB:0x%X BB:0x%X\n", cpu1.regs.AL, cpu1.regs.BL, cpu1.regs.AB, cpu1.regs.BB);
	printf("-------------------------------------------------------------------------------\n");
	for(i = 0; i < 32; i++)
		printf("%lX:%X ", i, mem_read8(cpu1.memory, i));

	printf("\n-------------------------------------------------------------------------------\n");

	
	
	return cpu1.regs.AL;
}

