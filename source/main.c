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
	return 0;
}

