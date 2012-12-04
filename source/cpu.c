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

u32int process_opcode(farcpu *cpu)
{
	u8int op = cpu->regs.IR;
	u8int mem_add = 0; //amount to add to memory to get to next OpCode
	u32int PC = cpu->regs.PC;
	char *memory = cpu->memory;
	if(op == EXOP) return process_extended_opcode(cpu);

	switch(op)
	{
		case NOP:
			asm("nop"); break; //might just remove the asm statement sometime, just fo-sho right now

		case INC:
			mem_get8(memory, PC) = mem_get8(memory, PC) + 1; mem_add++; break;

		case DEC:
			mem_get8(memory, PC) = mem_get8(memory, PC) - 1; mem_add++; break;
	}
	return 0;
}

u32int process_extended_opcode(farcpu *cpu)
{
	return 0;
}