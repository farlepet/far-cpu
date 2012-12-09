#include <common.h>
#include <SDL/SDL.h>
#include <math.h>

#ifndef GFX_H
#define GFX_H

#define WIDTH   640
#define HEIGHT  480
#define BPP     4
#define DEPTH   32

#define CMEMLOC 0x100 //location of character memory (byte 256) will have to be moved later to allow for bigger programs obviously
#define CWIDTH  80
#define CHEIGHT 60
#define CMEMSZ  CWIDTH*CHEIGHT //80*60 = 4800 (4KB memory for on-screen info alone!)
//2KB will grow to 6848(6KB) when I transition to placing character bitmaps in CPU memory

u8int g_8x8_font[2048];
int init_gfx();
int gfx_upd();
void drawChar(u8int ch, u32int x, u32int y, u32int c);

#endif