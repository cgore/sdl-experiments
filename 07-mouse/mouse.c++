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
#include <string>
#include <vector>
using namespace std;

#include <boost/lexical_cast.hpp>
using namespace boost;

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


class Text
{
  private:

    static TTF_Font *_font;
    static SDL_Color *_color;

    SDL_Surface *_screen;
    SDL_Surface *_message;

  public:

    Text(SDL_Surface *screen, string text)
    {
      _screen = screen;
      if (!_color) {
        _color = new SDL_Color({0xff, 0xff, 0xff});
      }
      if (!_font) {
        _font = TTF_OpenFont("LucidaTypewriterRegular.ttf", 12);
        if (!_font) {
          throw "Failed to load the font.";
        }
      }
      _message = TTF_RenderText_Solid(_font, text.c_str(), *_color);
    }

    ~Text(void)
    {
      if (_message) SDL_FreeSurface(_message);
    }

    void blit(void)
    {
      SDL_BlitSurface(_message, NULL, _screen, NULL);
    }
};

TTF_Font *Text::_font;
SDL_Color *Text::_color;


class TileType
{
  private:

    string _filepath;
    SDL_Surface *_loaded_image;
    SDL_Surface *_display_image;

  public:

    TileType(string filepath)
    {
      _filepath = filepath;
      _loaded_image = IMG_Load(_filepath.c_str());
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

    static void tile_type(string filepath)
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


class Game
{
  private:

    bool _quit;
    SDL_Surface *_screen;

    TileType *_aqua;
    TileType *_lawn;
    TileType *_marble;
    TileType *_wall;
    TileType *_wood;

    Tile _aqua1;
    Tile _lawn1;
    Tile _marble1;
    Tile _wall1;
    Tile _wood1;
    Tile _wood2;
    Tile _wood3;
    Tile _wood4;
    Tile _wood5;
    vector<Tile> _all_lawn;

    Player *_player;

    Text *_message;

    SDL_Event _event;

    void _sdl_init(void)
    {
      if (!! SDL_Init(SDL_INIT_EVERYTHING)) {
        throw "SDL_Init failed.";
      }
      _screen = SDL_SetVideoMode(
          SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_COLOR_DEPTH,
          SDL_SWSURFACE|SDL_DOUBLEBUF);
      if (!_screen) {
        throw "Failed to set the screen video mode.";
      }
      SDL_WM_SetCaption("SDL C++ Mouse Experiment", NULL);
      if (!! TTF_Init()) {
        throw "TTF_Init failed.";
      }
    }

    void _load_images(void)
    {
      _aqua = new TileType("aqua.jpg");
      _lawn = new TileType("lawn.jpg");
      _marble = new TileType("marble_dark.jpg");
      _wall = new TileType("wall-grey.jpg");
      _wood = new TileType("wood.jpg");
      Player::tile_type("player.png");
      SDL_Delay(100); // XXX: Need a delay for the images to load for some reason.
    }

    void _setup_tiles(void)
    {
      _aqua1 = Tile(_screen, _aqua, 0, 0);
      _lawn1 = Tile(_screen, _lawn, 1, 0);
      _marble1 = Tile(_screen, _marble, 2, 0);
      _wall1 = Tile(_screen, _wall, 3, 0);
      _wood1 = Tile(_screen, _wood, 0, 1);
      _wood2 = Tile(_screen, _wood, 1, 1);
      _wood3 = Tile(_screen, _wood, 2, 1);
      _wood4 = Tile(_screen, _wood, 3, 1);
      _wood5 = Tile(_screen, _wood, 4, 1);
      _all_lawn = vector<Tile>(0);
      for (int i = 0; i < TILES_WIDTH_COUNT; i++) {
        for (int j = 0; j < TILES_HEIGHT_COUNT; j++) {
          _all_lawn.push_back(Tile(_screen, _lawn, i, j));
        }
      }
    }

    void _blit(void)
    {
      for (vector<Tile>::iterator lawn = _all_lawn.begin();
          lawn != _all_lawn.end();
          lawn++) {
        lawn->blit();
      }
      _aqua1.blit();
      _lawn1.blit();
      _marble1.blit();
      _wall1.blit();
      _wood1.blit();
      _wood2.blit();
      _wood3.blit();
      _wood4.blit();
      _wood5.blit();
      _player->blit();
      _message->blit();
      SDL_Flip(_screen);
    }

    void _handle_event(void)
    {
      string text;
      if (_event.type == SDL_QUIT) {
        text += "SDL_QUIT";
        _quit = true;
      } else if (_event.type == SDL_KEYDOWN) {
        text += "SDL_KEYDOWN ";
        SDLKey key = _event.key.keysym.sym;
        if (key == SDLK_LEFT) {
          text += "SDLK_LEFT";
          _player->move_left();
        } else if (key == SDLK_RIGHT) {
          text += "SDLK_RIGHT";
          _player->move_right();
        } else if (key == SDLK_UP) {
          text += "SDLK_UP";
          _player->move_up();
        } else if (key == SDLK_DOWN) {
          text += "SDLK_DOWN";
          _player->move_down();
        } else if (key == SDLK_q) {
          text += "SDLK_q";
          _quit = true;
        }
      } else if (_event.type == SDL_MOUSEBUTTONDOWN) {
        text += "SDL_MOUSEBUTTONDOWN ";
        SDL_MouseButtonEvent button = _event.button;
        if (button.button == SDL_BUTTON_LEFT) {
          text += "LEFT";
        } else if (button.button == SDL_BUTTON_MIDDLE) {
          text += "MIDDLE";
        } else if (button.button == SDL_BUTTON_RIGHT) {
          text += "RIGHT";
        }
        text += " x = " + lexical_cast<string>(button.x);
        text += " y = " + lexical_cast<string>(button.y);
      }
      _message = new Text(_screen, text);
    }

  public:

    Game(void)
    {
      _quit = false;
      _sdl_init();
      _load_images();
      _setup_tiles();
      _player = new Player(_screen);
    }

    ~Game(void)
    {
      SDL_Quit();
    }

    void main_loop(void)
    {
      _message = new Text(_screen, "Initialized");
      while (!_quit) {
        _blit();
        while (SDL_PollEvent(&_event)) {
          _handle_event();
        }
      }
    }
};


int main(int argc, char **argv)
{
  Game game = Game();
  game.main_loop();
  return 0;
}
