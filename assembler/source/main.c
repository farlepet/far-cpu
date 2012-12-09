/*
 *  Peter Farley 2012
 *  Determining mem addr, num
 *  memory: &addr or @haxaddr ex: &64 or @40
 *  num: $num or %hexaddr ex: $128 or %80
 *
 */

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <common.h>

#define USAGE "USAGE:\
  farcpu-asm [input] [output]\n\
    input: input file to be assembled\n\
    output: file to be created during assembly\n\
  ex: farcpu-asm fast.fcsm fast.bin\n\n"
  
int getOpcode(char *str);
int getRegister(char *str);
int write_ASMD(char *str);
void write_mov_n(char *str);
#define WR(val) fprintf(output, "%c", (u8int)val)
#define WR16(val) fprintf(output, "%c%c", (u8int)val&0xFF, (u8int)(val&0xFF00)>>8)
#define WR32(val) fprintf(output, "%c%c%c%c\n", (u8int)((val&0xFF)), (u8int)((val&0xFF00)>>8), (u8int)((val&0xFF0000)>>16), (u8int)((val&0xFF000000)>>24))
#define D printf

FILE *input, *output;

int main(int argc, char *argv[])
{
	if(argc < 3)
	{
		printf(USAGE);
		return -1;
	}
	
	char *op1, *op2, *op3, *op4;
	u32int addr = 0;
	u32int tnum = 0;
	
	if((input = fopen(argv[1], "r")) == NULL)
	{	printf("ERR:FILE %s COULD NOT BE OPENED!\n", argv[1]); return -1;	}
	if((output = fopen(argv[2], "w+")) == NULL)
	{	printf("ERR:FILE %s COULD NOT BE OPENED!\n", argv[2]); return -2;	}
		
	char *str = malloc(1024); u32int i;
	D("starting assemble...\n");
	while(fscanf(input, "%[^\n]\n", str) != EOF)
	{
		for(i = 0; i < strlen(str); i++) str[i] = toupper(str[i]);
		if(str[0] != '#')
		{
			switch(getOpcode(str))
			{
				case 0: D("NOP\n"); WR(0); break;                                                                                                 //NOP
				
				case 1: D("INC\n"); WR(1); WR(getRegister(strtok(NULL, " "))); break;                                                             //INC
				case 2: D("DEC\n"); WR(2); WR(getRegister(strtok(NULL, " "))); break;                                                             //DEC
				
				case 3: D("ADD\n"); WR(3); op1 = strtok(NULL, " "); op2 = strtok(NULL, " "); write_ASMD(op1); write_ASMD(op2); break; //ADD
				case 4: D("SUB\n"); WR(4); op1 = strtok(NULL, " "); op2 = strtok(NULL, " "); write_ASMD(op1); write_ASMD(op2); break; //SUB
				case 5: D("MUL\n"); WR(5); op1 = strtok(NULL, " "); op2 = strtok(NULL, " "); write_ASMD(op1); write_ASMD(op2); break; //MUL
				case 6: D("DIV\n"); WR(6); op1 = strtok(NULL, " "); op2 = strtok(NULL, " "); write_ASMD(op1); write_ASMD(op2); break; //DIV
				
				case 7: D("MOVNM\n"); WR(7); op1 = strtok(NULL, " "); op2 = strtok(NULL, " "); write_mov_n(op1); 
					if(*op2 == '&') WR32(strtol(op2+1, NULL, 10)); else if(*op2 == '@') WR32(strtol(op2+1, NULL, 16)); else { D("ERR:UNKNOWN NUMBER TYPE %c\n", *op2); exit(0); } break;
					
				case 8: D("MOVRM\n"); WR(8); op1 = strtok(NULL, " "); op2 = strtok(NULL, " "); WR(getRegister(op1));
					if(*op2 == '&') addr = strtol(op2+1, NULL, 10); else if(*op2 == '@') addr = strtol(op2+1, NULL, 16); else { D("ERR:UNKNOWN NUMBER TYPE %c\n", *op2); exit(0); } WR32(addr); break;
					
				case 9: D("MOVIM\n"); WR(9); op1 = strtok(NULL, " "); if(*op1 == '&') addr = strtol(op1+1, NULL, 10); else if(*op1 == '@') addr = strtol(op1+1, NULL, 16); else { D("ERR:UNKNOWN NUMBER TYPE %c\n", *op1); exit(0); } WR32(addr);break;
				
				case 10: D("MOVMM\n"); WR(10); op1 = strtok(NULL, " "); op2 = strtok(NULL, " "); if(*op1 == '&') addr = strtol(op1+1, NULL, 10); else if(*op1 == '@') addr = strtol(op1+1, NULL, 16); else { D("ERR:UNKNOWN NUMBER TYPE %c\n", *op1); exit(0); } WR32(addr);
					if(*op2 == '&') addr = strtol(op2+1, NULL, 10); else if(*op2 == '@') addr = strtol(op2+1, NULL, 16); else { D("ERR:UNKNOWN NUMBER TYPE %c\n", *op2); exit(0); } WR32(addr); break;
					
				case 11: D("MOVMR\n"); WR(11); op1 = strtok(NULL, " "); op2 = strtok(NULL, " "); if(*op2 == '&') addr = strtol(op2+1, NULL, 10); else if(*op2 == '@') addr = strtol(op2+1, NULL, 16); else { D("ERR:UNKNOWN NUMBER TYPE %c\n", *op2); exit(0); } WR32(addr);
					WR(getRegister(op1)); break;
					
				case 12: D("MOVNR\n"); WR(12); op1 = strtok(NULL, " "); op2 = strtok(NULL, " "); write_mov_n(op1); WR(getRegister(op2)); break;
				
				case 13: D("MOVIR\n"); WR(13); op1 = strtok(NULL, " "); WR(getRegister(op1)); break;
				
				case 14: D("MOVRR\n"); WR(14); op1 = strtok(NULL, " "); op2 = strtok(NULL, " "); WR(getRegister(op1)); WR(getRegister(op2)); break;
				
				
				case 21: D("HWU\n"); WR(21); op1 = strtok(NULL, " "); if(*op1 == '$') tnum = strtol(op1+1, NULL, 10); else if(*op1 == '%') tnum = strtol(op1+1, NULL, 16); else { D("ERR:UNKNOWN NUMBER TYPE %c\n", *op1); exit(0); } WR16(tnum); break;
				
				case 22: D("JMP\n"); WR(22); op1 = strtok(NULL, " "); if(*op1 == '&') addr = strtol(op1+1, NULL, 10); else if(*op1 == '@') addr = strtol(op1+1, NULL, 16); else { D("ERR:UNKNOWN NUMBER TYPE %c\n", *op1); exit(0); } WR32(addr); break;
				
				case 23: D("SJP\n"); WR(23); op1 = strtok(NULL, " "); if(*op1 == '&') addr = strtol(op1+1, NULL, 10); else if(*op1 == '@') addr = strtol(op1+1, NULL, 16); else { D("ERR:UNKNOWN NUMBER TYPE %c\n", *op1); exit(0); } WR32(addr); break;
				
				case 24: D("JZ\n"); WR(24); break;
				case 25: D("JNZ\n"); WR(25); break;
				case 26: D("JFE\n"); WR(26); break;
				case 27: D("JNE\n"); WR(27); break;
				
				case 28: D("JGT\n"); WR(28); break;
				case 29: D("JNG\n"); WR(29); break;
				case 30: D("JGE\n"); WR(30); break;
				case 31: D("JNGE\n"); WR(31); break;
				
				case 32: D("JLT\n"); WR(32); break;
				case 33: D("JNL\n"); WR(33); break;
				case 34: D("JLE\n"); WR(34); break;
				case 35: D("JNLE\n"); WR(35); break;
				
				case 37: D("MOVRRM\n"); WR(37); op1 = strtok(NULL, " "); op2 = strtok(NULL, " "); WR(getRegister(op1)); WR(getRegister(op2)); break;
				
				case 40: D("RET\n"); WR(40); break;
				
				default: D("WARN:%s(%d) NOT IMPLEMENTED YET!\n", strtok(str, " "), getOpcode(str)); break;
			}
		}
		else D("#commented line\n");
	}
	
	//printf(str);
		
	fclose(input); fclose(output);
	return 0;
}

char *getUpToSpace(char *str)
{
	char *out = strtok(str, " ");
	return out;
}

const char *n_to_instruction[] = 
{
	"NOP", "INC", "DEC", "ADD", "SUB", "MUL", "DIV",
	"MOVNM", "MOVRM", "MOVIM", "MOVMM",
	"MOVMR", "MOVNR", "MOVIR", "MOVRR",
	"SHL", "SHR", "AND", "OR", "XOR", "NOT",
	"HWU", "JMP", "SJP",
	"JZ", "JNZ", "JFE", "JNE",
	"JGT", "JNG", "JGE", "JNGE",
	"JLT", "JNL", "JLE", "JNLE",
	"MOVNRM", "MOVRRM", "MOVIRM", "MOVMRM",
	"RET"
};

#define nins (sizeof(n_to_instruction) / sizeof(n_to_instruction[0]))

int getOpcode(char *str)
{
	char *OP = getUpToSpace(str);
	u16int i;
	for(i = 0; i < nins; i++)
	{
		if(!strcmp(OP, n_to_instruction[i])) return i;
	}
	printf("\nERR:INVALID OPCODE: %s\n\n", str);
	exit(0);
}



char *reg_strs[17] = 
{
	"AB", "BB", "CB", "DB",
	"AS", "BS", "CS", "DS",
	"AL", "BL", "CL", "DL",

	"PC", "IR", "SP", "CSP",
	"JP"
};

int getRegister(char *str)
{
	int i;
	for(i = 0; i < 16; i++)
	{
		if(!strncmp(str, reg_strs[i], strlen(reg_strs[i]))) return i;
	}
	printf("\nERR:INVALID REGISTER: %s\n\n", str);
	exit(0);
}

typedef enum
{
	REGISTER,
	NUMBER,  //DWORD
	IO,
	MEMORY
} input_location;

int write_ASMD(char *str)
{
	D("ASMD:%s\n", str);
	char *t = malloc(15);
	strcpy(t, str);
	char *tmp = strtok(t, " ");
	if(*tmp == '&')//dec mem addr
	{
		u32int addr = strtol(str+1, NULL, 10);
		WR(MEMORY);
		WR((int)(addr&0xFF000000)>>23);
		WR((int)(addr&0xFF0000)>>15);
		WR((int)(addr&0xFF00)>>7);
		WR((int)(addr&0xFF));
		return MEMORY;
	}
	
	if(*tmp == '@')//hex mem addr
	{
		u32int addr = strtol(str+1, NULL, 16);
		WR(MEMORY);
		WR((int)(addr&0xFF000000)>>23);
		WR((int)(addr&0xFF0000)>>15);
		WR((int)(addr&0xFF00)>>7);
		WR((int)(addr&0xFF));
		return MEMORY;
	}
	
	if(*tmp == '$')//dec num
	{
		u16int num = strtol(str+1, NULL, 10);
		WR(NUMBER);
		WR((num&0xFF00)>>7);
		WR(num&0xFF);
		return NUMBER;
	}
	
	if(*tmp == '%')//hex num
	{
		u16int num = strtol(str+1, NULL, 16);
		WR(NUMBER);
		WR((num&0xFF00)>>7);
		WR(num&0xFF);
		return NUMBER;
	}
	
	if(!strcmp(str, "IO"))//IO
	{
		WR(IO);
		return IO;
	}
	
	int i;
	for(i = 0; i < 16; i++)
	{
		if(!strcmp(str, reg_strs[i]))
		{
			WR(REGISTER);
			WR(i);
			return REGISTER;
		}
	}
	D("ERR:ADD/SUB/MUL/DIV ERROR: %s CANNOT BE CONERTED\n", str);
	exit(0);
}

u8int get_num_size(char *str)
{
	if(*str == '1') if(*(str+1) == '$' || *(str+1) == '%') return 0;
		
	if(*str == '2') if(*(str+1) == '$' || *(str+1) == '%') return 1;
		
	if(*str == '4') if(*(str+1) == '$' || *(str+1) == '%') return 2;
	
	D("ERR:UNDEFINED NUMBER SIZE: %s\n", str);
	exit(0);
}

void write_mov_n(char *str)
{
	u8int tmp = get_num_size(str++);
	WR(tmp);
	u32int addr;
	if(*str == '$') addr = strtol(str+1, NULL, 10);
	else if(*str == '%') addr = strtol(str+1, NULL, 16);
	else { D("ERR:UNKNOWN DATA TYPE:%c", *str); exit(0); }
	if(tmp == 2)
	{
		WR32(addr);
	}
	
	else if(tmp == 1)
	{
		WR16((int)addr);
	}
	
	else if(tmp == 0) WR((int)(addr&0xFF));
	
	D("N:0x%lX\n", addr);
}
