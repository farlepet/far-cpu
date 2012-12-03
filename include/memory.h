#include <common.h>

#ifndef MEMORY_H
#define MEMORY_H

char *main_memory;

int setup_memory(char *mem, u32int size);

u32int init_main_memory(u32int size);

void remove_memory(int mem);

void end_memory();

#endif