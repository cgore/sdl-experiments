#include <cstring>

#include <iostream>
#include <vector>
using namespace std;

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define SCREEN_COLOR_DEPTH 32

#define TILE_WIDTH 94
#define TILE_HEIGHT 94
#define TILES_WIDTH_COUNT (SCREEN_WIDTH / TILE_WIDTH)
#define TILES_HEIGHT_COUNT (SCREEN_HEIGHT / TILE_HEIGHT)
#define TILES_COUNT (TILES_WIDTH_COUNT * TILES_HEIGHT_COUNT)

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>


class TileType
{
  private:

    const char *_filepath;
    SDL_Surface *_loaded_image;
    SDL_Surface *_display_image;

  public:

    TileType(const char *filepath)
    {
      _filepath = strdup(filepath);
      _loaded_image = IMG_Load(_filepath);
      if (!_loaded_image) {
        throw "Image was not successfully loaded.";
      }
      _display_image = SDL_DisplayFormatAlpha(_loaded_image);
      if (!_display_image) {
        throw "Image was not successfully converted to display format.";
      }
    }

    ~TileType(void)
    {
      if (_loaded_image) SDL_FreeSurface(_loaded_image);
      if (_display_image) SDL_FreeSurface(_display_image);
    }

    const SDL_Surface *image(void) const
    {
      return _display_image;
    }
};


class Tile
{
  private:

    SDL_Surface *_screen;
    SDL_Rect _rectangle;
    const TileType *_tile_type;
    unsigned int _x;
    unsigned int _y;

  public:

    Tile(SDL_Surface *screen = NULL, const TileType *tile_type = NULL,
        unsigned int x = 0, unsigned int y = 0)
    {
      _screen = screen;
      _tile_type = tile_type;
      _x = x;
      _y = y;
      _rectangle.x = _x * TILE_WIDTH;
      _rectangle.y = _y * TILE_HEIGHT;
    }

    void blit(void)
    {
      if (_screen && _tile_type) {
        SDL_BlitSurface(
            (SDL_Surface *) _tile_type->image(),
            NULL, _screen, &_rectangle);
      }
    }

    void noisy_blit(void)
    {
      cout << "Blitting " 
        << _x << " " << _y
        << " (" << _rectangle.x << "," << _rectangle.y << ")" << endl;
      blit();
    }

    unsigned int x(void) const
    {
      return _x;
    }

    unsigned int y(void) const
    {
      return _y;
    }

    unsigned int x(unsigned int x)
    {
      _x = x;
      _rectangle.x = _x * TILE_WIDTH;
      return _x;
    }

    unsigned int y(unsigned int y)
    {
      _y = y;
      _rectangle.y = _y * TILE_HEIGHT;
      return _y;
    }
};


int main(int argc, char **argv)
{
  bool quit = false;
  unsigned int player_x = 0;
  unsigned int player_y = 0;

  if (!! SDL_Init(SDL_INIT_EVERYTHING)) {
    throw "SDL_Init failed.";
  } else {
    cout << "SDL_Init succeeded." << endl;
  }

  SDL_Surface *screen = SDL_SetVideoMode(
      SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_COLOR_DEPTH,
      SDL_SWSURFACE|SDL_DOUBLEBUF);
  if (!screen) {
    throw "Failed to set the screen video mode.";
  } else {
    cout << "SDL_SetVideoMode succeeded." << endl;
  }

  SDL_WM_SetCaption("C++ SDL Movement Experiment", NULL);

  TileType aqua = TileType("aqua.jpg");
  TileType lawn = TileType("lawn.jpg");
  TileType marble = TileType("marble_dark.jpg");
  TileType wall = TileType("wall-grey.jpg");
  TileType wood = TileType("wood.jpg");
  TileType player = TileType("player.png");
  SDL_Delay(100);
  
  Tile aqua1 = Tile(screen, &aqua, 0, 0);
  Tile lawn1 = Tile(screen, &lawn, 1, 0);
  Tile marble1 = Tile(screen, &marble, 2, 0);
  Tile wall1 = Tile(screen, &wall, 3, 0);
  Tile wood1 = Tile(screen, &wood, 0, 1);
  Tile wood2 = Tile(screen, &wood, 1, 1);
  Tile wood3 = Tile(screen, &wood, 2, 1);
  Tile wood4 = Tile(screen, &wood, 3, 1);
  Tile wood5 = Tile(screen, &wood, 4, 1);
  
  vector<Tile> all_lawn(0);
  for (int i = 0; i < TILES_WIDTH_COUNT; i++) {
    for (int j = 0; j < TILES_HEIGHT_COUNT; j++) {
      all_lawn.push_back(Tile(screen, &lawn, i, j));
    }
  }

  Tile player1 = Tile(screen, &player, player_x, player_y);
  
  SDL_Event event;
  while (!quit) {

    for (int i = 0; i < TILES_COUNT; i++) {
      all_lawn[i].blit();
    }
    aqua1.blit();
    lawn1.blit();
    marble1.blit();
    wall1.blit();
    wood1.blit();
    wood2.blit();
    wood3.blit();
    wood4.blit();
    wood5.blit();
    player1.noisy_blit();
    SDL_Flip(screen);

    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        cout << "Received SDL_QUIT event." << endl;
        quit = true;
      } else if (event.type == SDL_KEYDOWN) {
        SDLKey key = event.key.keysym.sym;
        cout << "Received SDL_KEYDOWN event." << endl;
        if (key == SDLK_LEFT) {
          cout << "Received a SDLK_LEFT keypress." << endl;
          if (0 < player_x) {
            player1.x(--player_x);
            cout << "player1.x() = " << player1.x() << endl;
          }
        } else if (key == SDLK_RIGHT) {
          cout << "Received a SDLK_RIGHT keypress." << endl;
          if (player_x < TILES_WIDTH_COUNT) {
            player1.x(++player_x);
            cout << "player1.x() = " << player1.x() << endl;
          }
        } else if (key == SDLK_UP) {
          cout << "Received a SDLK_UP keypress." << endl;
          if (0 < player_y) {
            player1.y(--player_y);
            cout << "player1.y() = " << player1.y() << endl;
          }
        } else if (key == SDLK_DOWN) {
          cout << "Received a SDLK_DOWN keypress." << endl;
          if (player_y < TILES_HEIGHT_COUNT) {
            player1.y(++player_y);
            cout << "player1.y() = " << player1.y() << endl;
          }
        } else if (key == SDLK_q) {
          cout << "Received a SDLK_q keypress, quitting." << endl;
          quit = true;
        }
      }
    }
  }

  SDL_Quit();
  return 0;
}
