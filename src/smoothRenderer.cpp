
#include "smoothRenderer.h"


void SmoothRenderer::init(){
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Event event;
    SDL_Renderer *renderer;
    SDL_Window *window;

    SDL_CreateWindowAndRenderer(WIDTH, HEIGHT, 0, &window, &renderer);

    _rMin = REAL_MIN;
    _rMax = REAL_MAX;
    _imMin = IMAGINARY_MIN;
    _imMax = IMAGINARY_MAX;
    _maxIterations = MAX_ITERATIONS;

    double realPart;
    double imaginaryPart;

    for (int x = 0; x < WIDTH; x++){
        for (int y = 0; y < HEIGHT; y++){
            realPart = x * static_cast<double>(_rMax - _rMin) / WIDTH + _rMin;
            imaginaryPart = y * static_cast<double>(_imMax - _imMin) / HEIGHT + _imMin;

            _pixelMap[coordinatesToStringKey(x, y)] = Pixel(realPart, imaginaryPart, RGB(0, 0, 0), 0, MAX_ITERATIONS);
        }
    }
}

void SmoothRenderer::renderFrames(){
    std::string key;
    Pixel pixel;

    for (int i = 0; i < _maxIterations; i++){
        for (std::pair<std::string, Pixel> keyValue : _pixelMap){
            auto [key, pixel] = keyValue;
            
        }
    }
}

