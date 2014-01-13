// Copyright (c) 2012, Christopher Mark Gore,
// All rights reserved.
//
// 2317 South River Road, Saint Charles, Missouri 63303 USA.
// Web: http://cgore.com
// Email: cgore@cgore.com
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
//   * Redistributions of source code must retain the above copyright
//     notice, this list of conditions and the following disclaimer.
//  
//   * Redistributions in binary form must reproduce the above copyright
//     notice, this list of conditions and the following disclaimer in the
//     documentation and/or other materials provided with the distribution.
//  
//   * Neither the name of Christopher Mark Gore nor the names of other
//     contributors may be used to endorse or promote products derived from
//     this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.


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
#include <SDL/SDL_ttf.h>


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
      _rectangle.x = x * TILE_WIDTH;
      return _x = x;
    }

    unsigned int y(unsigned int y)
    {
      _rectangle.y = y * TILE_HEIGHT;
      return _y = y;
    }
};


class Player
{
  private:

    static TileType *_tile_type;

    SDL_Surface *_screen;
    unsigned int _x;
    unsigned int _y;
    Tile _tile;

  public:

    static void tile_type(const char *filepath)
    {
      _tile_type = new TileType(filepath);
    }

    Player(SDL_Surface *screen)
    {

      _screen = screen;
      _x = 0;
      _y = 0;
      _tile = Tile(_screen, _tile_type, _x, _y);
    }

    unsigned int x(void) const
    {
      return _x;
    }

    unsigned int y(void) const
    {
      return _y;
    }

    void blit(void)
    {
      _tile.blit();
    }

    void move_left(void)
    {
      if (0 < _x) {
        _tile.x(--_x);
      }
    }

    void move_right(void)
    {
      if (_x < TILES_WIDTH_COUNT - 1) {
        _tile.x(++_x);
      }
    }

    void move_up(void)
    {
      if (0 < _y) {
        _tile.y(--_y);
      }
    }

    void move_down(void)
    {
      if (_y < TILES_HEIGHT_COUNT - 1) {
        _tile.y(++_y);
      }
    }
};

TileType *Player::_tile_type;

int main(int argc, char **argv)
{
  bool quit = false;

  if (!! SDL_Init(SDL_INIT_EVERYTHING)) {
    throw "SDL_Init failed.";
  }

  SDL_Surface *screen = SDL_SetVideoMode(
      SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_COLOR_DEPTH,
      SDL_SWSURFACE|SDL_DOUBLEBUF);
  if (!screen) {
    throw "Failed to set the screen video mode.";
  }

  SDL_WM_SetCaption("SDL C++ Text Experiment", NULL);

  if (!! TTF_Init()) {
    throw "TTF_Init failed.";
  }

  TileType aqua = TileType("aqua.jpg");
  TileType lawn = TileType("lawn.jpg");
  TileType marble = TileType("marble_dark.jpg");
  TileType wall = TileType("wall-grey.jpg");
  TileType wood = TileType("wood.jpg");
  Player::tile_type("player.png");
  SDL_Delay(100); // XXX: Need a delay for the images to load for some reason.
  
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

  Player player = Player(screen);

  TTF_Font *font = TTF_OpenFont("LucidaTypewriterRegular.ttf", 12);
  if (!font) {
    throw "Failed to load the font.";
  }
  SDL_Color text_color = {0xff, 0xff, 0xff};
  SDL_Surface *message;
  
  SDL_Event event;
  while (!quit) {

    for (vector<Tile>::iterator lawn = all_lawn.begin();
        lawn != all_lawn.end();
        lawn++) {
      lawn->blit();
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
    player.blit();
    if (message) SDL_BlitSurface(message, NULL, screen, NULL);
    SDL_Flip(screen);

    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        message = TTF_RenderText_Solid(font, "SDL_QUIT", text_color);
        quit = true;
      } else if (event.type == SDL_KEYDOWN) {
        SDLKey key = event.key.keysym.sym;
        if (key == SDLK_LEFT) {
          message = TTF_RenderText_Solid(
              font, "SDL_KEYDOWN SDLK_LEFT", text_color);
          player.move_left();
        } else if (key == SDLK_RIGHT) {
          message = TTF_RenderText_Solid(
              font, "SDL_KEYDOWN SDLK_RIGHT", text_color);
          player.move_right();
        } else if (key == SDLK_UP) {
          message = TTF_RenderText_Solid(
              font, "SDL_KEYDOWN SDLK_UP", text_color);
          player.move_up();
        } else if (key == SDLK_DOWN) {
          message = TTF_RenderText_Solid(
              font, "SDL_KEYDOWN SDLK_DOWN", text_color);
          player.move_down();
        } else if (key == SDLK_q) {
          message = TTF_RenderText_Solid(
              font, "SDL_KEYDOWN SDLK_q", text_color);
          quit = true;
        }
      }
    }
  }

  if (message) SDL_FreeSurface(message);

  SDL_Quit();
  return 0;
}
