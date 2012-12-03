#include <common.h>

#ifndef MEMORY_H
#define MEMORY_H

typedef struct
{
	u8int AB;  //------|----------------------|
	u8int BB;  //      |                      |
	u8int CB;  //      |--> byte regs         |
	u8int DB;  //------|                      |
	u16int AS; //-----|                       |
	u16int BS; //     |                       |-----> General Purpose Registers (Math, MOV's, etc...)
	u16int CS; //     |--> word regs          |
	u16int DS; //-----|                       |
	u32int AL; //----|                        |
	u32int BL; //    |                        |
	u32int CL; //    |-----> double word regs |
	u32int DL; //----|------------------------|

	u32int 
} registers;

char *main_memory;

int setup_memory(char *mem, u32int size);

u32int init_main_memory(u32int size);

void remove_memory(int mem);

void end_memory();

#endif