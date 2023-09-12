#pragma once

#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_pixels.h>
#include <map>
#include <string>
#include <memory>
#include <vector>
#include <tuple>
#include <utility>
#include <cmath>
#include <mutex>
#include <thread>
#include <queue>
#include <omp.h>
#include <condition_variable>

#include "constants.h"
#include "pixel.h"
#include "utilities.h"

class SmoothRenderer{
public:
    void init();
    void renderFramesOpenMP();
    void renderFrames();
    void renderFramesSequentially();
    void draw();
    void functionTest(int & iteration, int & threadID);

private:
    SDL_Event _event;
    SDL_Renderer* _renderer;
    SDL_Window* _window;
    double _rMin;
    double _rMax;
    double _imMin;
    double _imMax;
    int _maxIterations;
    int _width;
    int _height;
    std::vector<std::shared_ptr<Pixel>> _pixels;
    std::map<std::string, std::shared_ptr<Pixel>> _pixelMap;
    std::queue<std::vector<Pixel>> _queue;

    void _calculatePixels(int firstIndex, int lastIndex, int& iteration, int& threadId);
    void _calculationThread();
    void _rendererThread();
};