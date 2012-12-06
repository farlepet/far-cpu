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
	u8int mem_add = 1; //amount to add to memory to get to next OpCode
	u32int PC = cpu->regs.PC + 1; //just skip the opcode, to make things easier
	char *memory = cpu->memory;
	if(op == EXOP) return process_extended_opcode(cpu);

	switch(op)
	{
		case NOP:
			asm("nop"); break; //might just remove the asm statement sometime, just fo-sho right now

		//Arithmatic:
		case INC:
			set_register(cpu, mem_get8(memory, PC), get_register(cpu, mem_get8(memory, PC)) + 1); mem_add++; break;

		case DEC:
			set_register(cpu, mem_get8(memory, PC), get_register(cpu, mem_get8(memory, PC)) - 1); mem_add++; break;

		case ADD:
			set_register(cpu, AL, process_in_loc(cpu, memory, PC, &mem_add) + process_in_loc(cpu, memory, PC + mem_add, &mem_add)); break;

		case SUB:
			set_register(cpu, AL, process_in_loc(cpu, memory, PC, &mem_add) - process_in_loc(cpu, memory, PC + mem_add, &mem_add)); break;

		case MUL: //TODO:OPTIMIZE
			set_register(cpu, AL, process_in_loc(cpu, memory, PC, &mem_add) * process_in_loc(cpu, memory, PC + mem_add, &mem_add)); break;

		case DIV: //TODO:OPTIMIZE
			set_register(cpu, AL, process_in_loc(cpu, memory, PC, &mem_add) / process_in_loc(cpu, memory, PC + mem_add, &mem_add)); break;

		//Moving data around:     TODO:ADD CONTENT!!!
		case MOVNM:
			break;

		case MOVRM:
			break;

		case MOVIM:
			break;

		case MOVMM:
			break;

		case MOVMR:
			break;

		case MOVNR:
			break;

		case MOVIR:
			break;

		//bit-minipulating:
		case SHL:
			break;

		case SHR:
			break;

		case AND:
			break;

		case OR:
			break;

		case XOR:
			break;

		case NOT:
			break;

		//Low-Level:
		case HWU:
			break;

		//branching:
		case JMP:
			break;

		case JZ:
			break;

		case JNZ:
			break;

		case JFE:
			break;

		case JNE:
			break;

		case JGT:
			break;

		case JNG:
			break;

		case JGE:
			break;

		case JNGE:
			break;

		case JLT:
			break;

		case JNL:
			break;

		case JLE:
			break;

		case JNLE:
			break;

	}
	cpu->regs.PC += mem_add;
	return 0;
}

u32int process_in_loc(farcpu *cpu, char *mem, u32int loc, u8int *madd)
{
	input_location in = mem_get8(mem, loc);
	switch(in)
	{
		case REGISTER:
			*madd = *madd + 2;
			return get_register(cpu, mem_get8(mem, loc + 1));

		case NUMBER:
			*madd = *madd + 3;
			return mem_get16(mem, loc + 1);

		case IO:
			*madd = *madd + 1;
			return cpu->IO;

		case MEMORY:
			*madd = *madd + 5;
			return mem_get8(mem, mem_get32(mem, loc + 1));

		default:
			*madd = 0;
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

void set_register(farcpu *cpu, Register reg, u32int val)
{
	switch(reg)
	{
		case AB: cpu->regs.AB = val; return;
		case BB: cpu->regs.BB = val; return;
		case CB: cpu->regs.CB = val; return;
		case DB: cpu->regs.DB = val; return;

		case AS: cpu->regs.AS = val; return;
		case BS: cpu->regs.BS = val; return;
		case CS: cpu->regs.CS = val; return;
		case DS: cpu->regs.DS = val; return;

		case AL: cpu->regs.AL = val; return;
		case BL: cpu->regs.BL = val; return;
		case CL: cpu->regs.CL = val; return;
		case DL: cpu->regs.DL = val; return;

		case PC: cpu->regs.SP = val; return;
		case IR: cpu->regs.IR = val; return;
		case SP: cpu->regs.SP = val; return;

		default: return;
	}
}

u32int process_extended_opcode(farcpu *cpu)
{
	return 0;
}