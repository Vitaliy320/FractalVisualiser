#pragma once

#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_pixels.h>
#include <map>
#include <string>

#include "constants.h"
#include "pixel.h"
#include "utilities.h"

class SmoothRenderer{
public:
    void init();
    void renderFrames();

private:
    SDL_Event _event;
    SDL_Renderer* _renderer;
    SDL_Window* _window;
    double _rMin;
    double _rMax;
    double _imMin;
    double _imMax;
    int _maxIterations;
    std::map<std::string, Pixel> _pixelMap;
};