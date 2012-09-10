#include <cstring>

#include <iostream>
#include <map>
using namespace std;

#define TILE_WIDTH 94
#define TILE_HEIGHT 94

#include <SDL/SDL.h>

class Tile
{
  private:
    SDL_Surface *_screen;
    const char *_filepath;
    SDL_Surface *_loaded_image;
    SDL_Surface *_display_image;
    SDL_Rect _rectangle;
    unsigned int _x;
    unsigned int _y;
  public:
    Tile(SDL_Surface *screen, const char *filepath,
        unsigned int x, unsigned int y)
    {
      _screen = screen;
      _filepath = strdup(filepath);
      _loaded_image = SDL_LoadBMP(_filepath);
      if (!_loaded_image) {
        throw "Image was not successfully loaded.";
      }
      _display_image = SDL_DisplayFormat(_loaded_image);
      if (!_display_image) {
        throw "Image was not successfully converted to display format.";
      }
      _x = x;
      _y = y;
      _rectangle.x = _x * TILE_WIDTH;
      _rectangle.y = _y * TILE_HEIGHT;
    }
    ~Tile(void)
    {
      if (_loaded_image) SDL_FreeSurface(_loaded_image);
      if (_display_image) SDL_FreeSurface(_display_image);
    }
    void blit(void)
    {
      SDL_BlitSurface(_display_image, NULL, _screen, &_rectangle);
    }
};

int main(int argc, char **argv)
{
  if (!! SDL_Init(SDL_INIT_EVERYTHING)) {
    throw "SDL_Init failed.";
  }
  SDL_Surface *screen = SDL_SetVideoMode(640, 480, 32, SDL_SWSURFACE);
  if (!screen) {
    throw "Failed to set the screen video mode.";
  }
  SDL_WM_SetCaption("Tiles", NULL);

  Tile aqua = Tile(screen, "aqua.bmp", 0, 0);
  Tile lawn = Tile(screen, "lawn.bmp", 1, 0);
  Tile marble = Tile(screen, "marble_dark.bmp", 2, 0);
  Tile wall = Tile(screen, "wall-grey.bmp", 3, 0);
  Tile wood = Tile(screen, "wood.bmp", 4, 0);
  
  SDL_Delay(100);

  aqua.blit();
  lawn.blit();
  marble.blit();
  wall.blit();
  wood.blit();

  SDL_Flip(screen);
  SDL_Delay(5000);

  SDL_Quit();
  return 0;
}
