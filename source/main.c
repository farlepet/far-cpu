/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * main.c
 * Copyright (C) 2012 Peter Farley <far.peter1@gmail.com>
 * 
 */

#include <stdio.h>
#include <common.h>
#include <memory.h>
#include <math.h>

int main()
{
	char *temp;
	printf("Hello world\n");
	printf("Set up main memory of size %d at: 0x%lX\n", 2048, init_main_memory(2048));
	setup_memory(temp, pow(2,16));
	printf("Set up test memory of size %ld at: 0x%lX\n", (u32int)pow(2, 16), (u32int)&temp);
	end_memory();
	return (0);
}

