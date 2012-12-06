#include <cpu.h>

const char *n_to_instruction[256] = 
{
	"NOP", "INC", "DEC", "ADD", "SUB", "MUL", "DIV",
	"MOVNM", "MOVRM", "MOVIM", "MOVMM",
	"MOVMR", "MOVNR", "MOVIR", "MOVRR"
};

void cpu_reset(farcpu *cpu)
{
	//memset((cpu->memory), 0, cpu->memory_size);
	memset(&(cpu->regs), 0, sizeof(registers));
	cpu->IO = 0;
	cpu->inOut = false; //true means IN
	cpu->overflow = 0;
}

int init_cpu(farcpu *cpu, u32int mem_size)
{
	char *tcpum = malloc(1); //XXX: HACK to get rid of little warning message
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

		/*
		 * Aritmatic functions
		 * TODO: OVERFLOW, EXCEPTION Handeling
		 */
		case INC:
			set_register(cpu, mem_get8(memory, PC), get_register(cpu, mem_get8(memory, PC)) + 1); mem_add++; break;

		case DEC:
			set_register(cpu, mem_get8(memory, PC), get_register(cpu, mem_get8(memory, PC)) - 1); mem_add++; break;

		case ADD:
			set_register(cpu, AL, process_in_loc(cpu, memory, PC, &mem_add) + process_in_loc(cpu, memory, PC + mem_add-1 , &mem_add)); break;

		case SUB:
			set_register(cpu, AL, process_in_loc(cpu, memory, PC, &mem_add) - process_in_loc(cpu, memory, PC + mem_add-1, &mem_add)); break;

		case MUL: //TODO:OPTIMIZE
			set_register(cpu, AL, process_in_loc(cpu, memory, PC, &mem_add) * process_in_loc(cpu, memory, PC + mem_add-1, &mem_add)); break;

		case DIV: //TODO:OPTIMIZE, DIV BY 0
			set_register(cpu, AL, (u32int)(process_in_loc(cpu, memory, PC, &mem_add) / process_in_loc(cpu, memory, PC + mem_add-1, &mem_add))); break;

		//Moving data around:     TODO:ADD CONTENT!!!
		case MOVNM:
			switch(mem_get8(cpu->memory, PC)){
				case 0: mem_write8(cpu->memory, mem_read32(cpu->memory, PC + 2),mem_read8(cpu->memory, PC + 1)); mem_add+=6; break;
				case 1: mem_write16(cpu->memory, mem_read32(cpu->memory, PC + 3), mem_read16(cpu->memory, PC + 1)); mem_add+=7; break;
				case 2: mem_write32(cpu->memory, mem_read32(cpu->memory, PC + 5), mem_read32(cpu->memory, PC + 1)); mem_add+=9; break; }
			break;

		case MOVRM:
			switch(reg_sizes[mem_read8(cpu->memory, PC)]){
				case 1: mem_write8(cpu->memory, mem_read32(cpu->memory, PC + 1), get_register(cpu, mem_read8(cpu->memory, PC))); break;
				case 2: mem_write16(cpu->memory, mem_read32(cpu->memory, PC + 1), get_register(cpu, mem_read8(cpu->memory, PC))); break;
				case 4: mem_write32(cpu->memory, mem_read32(cpu->memory, PC + 1), get_register(cpu, mem_read8(cpu->memory, PC))); break;
			}
			mem_add += 5;
			break;

		case MOVIM:
			mem_write8(cpu->memory, mem_read32(cpu->memory, PC), cpu->IO); mem_add += 4;
			break;

		case MOVMM:
			break;

		case MOVMR:
			break;

		case MOVNR:
			break;

		case MOVIR:
			break;

		case MOVRR:
			set_register(cpu, mem_read8(cpu->memory, PC + 1), get_register(cpu, mem_read8(cpu->memory, PC))); mem_add += 2;
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
			cpu->regs.PC = mem_read32(cpu->memory, PC);
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
	cpu->regs.PC += mem_add + 0;
	return 0;
}

u32int process_in_loc(farcpu *cpu, char *mem, u32int loc, u8int *madd)
{
	input_location in = mem_get8(mem, loc);
	switch(in)
	{
		case REGISTER:
			*madd = *madd + 2;
			printf("REGISTER, ");
			return get_register(cpu, mem_get8(mem, loc + 1));

		case NUMBER:
			*madd = *madd + 3;
			printf("NUMBER(%X), ", mem_get8(mem, loc + 1) << 8 | mem_get8(mem, loc + 2));
			return mem_get8(mem, loc + 1) << 8 | mem_get8(mem, loc + 2);

		case IO:
			*madd = *madd + 1;
			printf("IO, ");
			return cpu->IO;

		case MEMORY:
			*madd = *madd + 5;
			printf("MEMORY, ");
			return mem_get8(mem, mem_get32(mem, loc + 1));

		default:
			*madd = 1;
			printf("ERR:DEFAULT\n");
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

u32int cpu_do_div(u32int a, u32int b)
{
	return (u32int)(a/b);
}