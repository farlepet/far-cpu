#include <gfx.h>

u32int *gfx_buff;

SDL_Surface *sdl_screen;
SDL_Event sdl_event;

void setscreenpixel(SDL_Surface *screen, int x, int y, Uint8 r, Uint8 g, Uint8 b)
{
    Uint32 *pixmem32;
    Uint32 colour;  
 
    colour = SDL_MapRGB( screen->format, r, g, b );
  
    pixmem32 = (Uint32*) screen->pixels  + y*screen->w + x;
    *pixmem32 = colour;
}

void UpdateScreen(SDL_Surface* screen)
{ 
    if(SDL_MUSTLOCK(screen)) 
    {
        if(SDL_LockSurface(screen) < 0) return;
    }

	memcpy(screen->pixels, gfx_buff, screen->w * screen->h);

    if(SDL_MUSTLOCK(screen)) SDL_UnlockSurface(screen);
  
    SDL_Flip(screen); 
}


int init_gfx()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0 ) return 1;
   
    if (!(sdl_screen = SDL_SetVideoMode(WIDTH, HEIGHT, DEPTH, SDL_HWSURFACE)))
    {
        SDL_Quit();
        return 1;
    }
	/*
    while(!keypress) 
    {
         DrawScreen(screen,h++);
         while(SDL_PollEvent(&event)) 
         {      
              switch (event.type) 
              {
                  case SDL_QUIT:
	              keypress = 1;
	              break;
                  case SDL_KEYDOWN:
                       keypress = 1;
                       break;
              }
         }
    }
	*/
    //SDL_Quit();
  
    return 0;
}