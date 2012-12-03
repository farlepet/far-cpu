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