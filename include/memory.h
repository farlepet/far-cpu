#include <common.h>

#ifndef MEMORY_H
#define MEMORY_H



char *reg_strs[17];

u8int reg_sizes[17];

typedef enum
{
	REGISTER,
	NUMBER,  //DWORD
	IO,
	MEMORY
} input_location;

int setup_memory(u32int size);

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