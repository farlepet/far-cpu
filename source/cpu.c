#include <cpu.h>

void cpu_reset(farcpu *cpu)
{
	//memset((cpu->memory), 0, cpu->memory_size);
	memset(&(cpu->regs), 0, sizeof(registers));
	cpu->IO = 0;
	cpu->inOut = false;
	cpu->overflow = 0;
}

int init_cpu(farcpu *cpu, u32int mem_size)
{
	char *tcpum = malloc(1); //XXX: HACK to get rid of nasty warning message
	setup_memory(tcpum, mem_size);
	cpu_reset(cpu);
	cpu->memory = tcpum;
	cpu->memory_size = mem_size;
	cpu->speed = OP4096S; //might as well set it for when it is actually used
	return 1;
}