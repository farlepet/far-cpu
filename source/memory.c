#include <memory.h>

#define MAX_MEM_ALLOCS 64

u32int memory_allocations[MAX_MEM_ALLOCS];

int n_allocs;

int setup_memory(char *mem, u32int size)
{
	if(n_allocs >= MAX_MEM_ALLOCS) return -1;
	mem = (char *)malloc(size);
	memset(mem, 0, size);
	memory_allocations[n_allocs++] = (u32int)mem;
	return n_allocs-1;
}

void remove_memory(int mem)
{
	if(memory_allocations[mem] == 0) return;
	free((char *)memory_allocations[mem]);
	int i = mem;
	for(; i < n_allocs--; i++)
		memory_allocations[i] = memory_allocations[i-1];
}

void end_memory()
{
	int i = 0;
	for(; i < n_allocs; i++)
	{
		remove_memory(i);
	}
}

u8int mem_read8(char *mem, u32int loc){ return *(u8int *)(mem + loc); }
u16int mem_read16(char *mem, u32int loc){ return *(u16int *)(mem + loc); }
u32int mem_read32(char *mem, u32int loc){ return *(u32int *)(mem + loc); }
u64int mem_read64(char *mem, u32int loc){ return *(u64int *)(mem + loc); }

void mem_write8(char *mem, u32int loc, u8int in){ *(u8int *)(mem + loc) = in; }
void mem_write16(char *mem, u32int loc, u16int in){ *(u16int *)(mem + loc) = in; }
void mem_write32(char *mem, u32int loc, u32int in){ *(u32int *)(mem + loc) = in; }
void mem_write64(char *mem, u32int loc, u64int in){ *(u64int *)(mem + loc) = in; }

u8int reg_sizes[16] = 
{
	1,1,1,1, //-|
	2,2,2,2, // |-> General Purpose
	4,4,4,4, //-|
	
	4,1,4,2
};
