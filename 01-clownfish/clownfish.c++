#include <iostream>
#include <map>
using namespace std;

#include <SDL/SDL.h>

int main(int argc, char **argv)
{
  SDL_Init(SDL_INIT_EVERYTHING);
  SDL_Surface *screen = SDL_SetVideoMode(640, 480, 32, SDL_SWSURFACE);
  SDL_Surface *image = SDL_LoadBMP("clownfish.bmp");
  SDL_BlitSurface(image, NULL, screen, NULL);
  SDL_Flip(screen);
  SDL_Delay(2000);
  SDL_FreeSurface(image);
  SDL_Quit();
  return 0;
}
