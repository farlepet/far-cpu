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

		case ADD:
			return 0;
	}
	return 0;
}

u8int process_in_loc(farcpu *cpu, char *mem, u32int loc, u32int *out)
{
	input_location in = mem_get8(mem, loc);
	switch(in)
	{
		case REGISTER:
			*out = get_register(cpu, mem_get8(mem, loc + 1));
			return 2;

		case NUMBER:
			*out = mem_get16(mem, loc + 1);
			return 3;

		case IO:
			*out = cpu->IO;
			return 1;

		case MEMORY:
			*out = mem_get8(mem, mem_get32(mem, loc + 1));
			return 5;

		default:
			*out = 0;
			return 0;
	}
}

u32int get_register(farcpu *cpu, Register reg)
{
	switch(reg)
	{
		case AB: return cpu->regs.AB;
		case BB: return cpu->regs.BB;
		case CB: return cpu->regs.CB;
		case DB: return cpu->regs.DB;

		case AS: return cpu->regs.AS;
		case BS: return cpu->regs.BS;
		case CS: return cpu->regs.CS;
		case DS: return cpu->regs.DS;

		case AL: return cpu->regs.AL;
		case BL: return cpu->regs.BL;
		case CL: return cpu->regs.CL;
		case DL: return cpu->regs.DL;

		case PC: return cpu->regs.SP;
		case IR: return cpu->regs.IR;
		case SP: return cpu->regs.SP;

		default: return 0;
	}
}

u32int process_extended_opcode(farcpu *cpu)
{
	return 0;
}