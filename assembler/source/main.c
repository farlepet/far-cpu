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
#define WR(val) fprintf(output, "%c", val)
#define D printf

FILE *input, *output;

int main(int argc, char *argv[])
{
	if(argc < 3)
	{
		printf(USAGE);
		return -1;
	}
	
	
	
	if((input = fopen(argv[1], "r")) == NULL)
	{	printf("ERR:FILE %s COULD NOT BE OPENED!\n", argv[1]); return -1;	}
	if((output = fopen(argv[2], "w+")) == NULL)
	{	printf("ERR:FILE %s COULD NOT BE OPENED!\n", argv[2]); return -2;	}
		
	char str[128]; u32int i;
	D("starting assemble...\n");
	while(fscanf(input, "%[^\n]\n", str) != EOF)
	{
		for(i = 0; i < strlen(str); i++) str[i] = toupper(str[i]);
		if(str[0] != '#')
		{
			switch(getOpcode(str))
			{
				case 0: D("NOP\n"); WR(0); break;                                                                    //NOP
				
				case 1: D("INC\n"); WR(1); WR(getRegister(strtok(NULL, " "))); break;                                //INC
				case 2: D("DEC\n"); WR(2); WR(getRegister(strtok(NULL, " "))); break;                                //DEC
				
				case 3: D("ADD\n"); WR(3); write_ASMD(strtok(NULL, " ")); write_ASMD(strtok(NULL, " ")); break;      //ADD
				
				default: D("WARN:%s NOT IMPLEMENTED YET!\n", strtok(str, " "));
			}
		}
		else D("#\n");
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
	"HWU", "JMP",
	"JZ", "JNZ", "JFE", "JNE",
	"JGT", "JNG", "JGE", "JNGE",
	"JLT", "JNL", "JLE", "JNLE"
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



char *reg_strs[16] = 
{
	"AB", "BB", "CB", "DB",
	"AS", "BS", "CS", "DS",
	"AL", "BL", "CL", "DL",

	"PC", "IR", "SP", "CSP"
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
	char *tmp = strtok(str, " ");
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
