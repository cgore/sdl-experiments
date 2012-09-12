#include <cstring>

#include <iostream>
#include <map>
using namespace std;

#define TILE_WIDTH 94
#define TILE_HEIGHT 94

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

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
      _loaded_image = IMG_Load(_filepath);
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
  bool quit = false;

  if (!! SDL_Init(SDL_INIT_EVERYTHING)) {
    throw "SDL_Init failed.";
  } else {
    cout << "SDL_Init succeeded." << endl;
  }

  SDL_Surface *screen = SDL_SetVideoMode(640, 480, 32, SDL_SWSURFACE);
  if (!screen) {
    throw "Failed to set the screen video mode.";
  } else {
    cout << "SDL_SetVideoMode succeeded." << endl;
  }

  SDL_WM_SetCaption("Events", NULL);

  Tile aqua = Tile(screen, "aqua.jpg", 0, 0);
  Tile lawn = Tile(screen, "lawn.jpg", 1, 0);
  Tile marble = Tile(screen, "marble_dark.jpg", 2, 0);
  Tile wall = Tile(screen, "wall-grey.jpg", 3, 0);
  Tile wood = Tile(screen, "wood.jpg", 4, 0);
  
  SDL_Delay(100);

  aqua.blit();
  lawn.blit();
  marble.blit();
  wall.blit();
  wood.blit();

  SDL_Flip(screen);

  SDL_Event event;
  while (!quit) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        cout << "Received SDL_QUIT event." << endl;
        quit = true;
      } else if (event.type == SDL_KEYDOWN
          ||     event.type == SDL_KEYUP) {
        cout << "Received SDL_KEYDOWN/UP event." << endl;
        if (event.key.keysym.sym == SDLK_LEFT) {
          cout << "Received a SDLK_LEFT keypress." << endl;
        }
      }
    }
  }

  SDL_Quit();
  return 0;
}
