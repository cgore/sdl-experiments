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
// * Redistributions of source code must retain the above copyright
//   notice, this list of conditions and the following disclaimer.
//
// * Redistributions in binary form must reproduce the above copyright
//   notice, this list of conditions and the following disclaimer in the
//   documentation and/or other materials provided with the distribution.
//
// * Neither the name of Christopher Mark Gore nor the names of other
//   contributors may be used to endorse or promote products derived from
//   this software without specific prior written permission.
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
  if (!! SDL_Init(SDL_INIT_EVERYTHING)) {
    throw "SDL_Init failed.";
  }
  SDL_Surface *screen = SDL_SetVideoMode(640, 480, 32, SDL_SWSURFACE);
  if (!screen) {
    throw "Failed to set the screen video mode.";
  }
  SDL_WM_SetCaption("C++ SDL Other Image Formats Experiment", NULL);

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
  SDL_Delay(5000);

  SDL_Quit();
  return 0;
}
