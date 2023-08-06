// #include <iostream>
// #define SDL_MAIN_HANDLED
// #include "SDL2/SDL.h"

// const int WIDTH{ 800 };
// const int HEIGHT{ 800 };

// long double MIN { -2.0 };
// long double MAX { 2.0 };

// const int MAX_ITERATIONS { 200 };

// long double map(long double value, long double in_min, long double in_max, long double out_min, long double out_max){
//     return (value - in_min) * (out_max - out_min) / (in_max - in_min) * out_min;
// }

// int main(){
//     SDL_Init(SDL_INIT_EVERYTHING);

//     SDL_Window* window;
//     SDL_Renderer* renderer;
//     SDL_Event event;
    
//     SDL_CreateWindowAndRenderer(1440, 900, 0, &window, &renderer);
//     SDL_RenderSetLogicalSize(renderer, WIDTH, HEIGHT);

//     long double a;
//     long double b;

//     long double ai;
//     long double bi;

//     long double a1;
//     long double b1;

//     int n;

//     while(true){
//         SDL_RenderPresent(renderer);

//         for (int x = 0; x < WIDTH; x++){
//             for (int y = 0; y < HEIGHT; y++){
//                 ai = a = map(x, 0, WIDTH, MIN, MAX);
//                 bi = b = map(y, 0, HEIGHT, MIN, MAX);
//                 n = 0;

//                 for (int i = 0; i < MAX_ITERATIONS; i++){
                    

//                     a1 = a * a - b * b;
//                     b1 = 2 * a * b;

//                     a = a1 + ai;
//                     b = b1 + bi;

//                     if ((a + b) > 2){
//                         break;
//                     }

//                     n++;
//                 }

//                 if (n == MAX_ITERATIONS){
//                     SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
//                     SDL_RenderDrawPoint(renderer, x, y);
//                     std::cout << "x: " << x << ", y: " << y << std::endl;
//                 }
//             }
//         }
//     }

//     return 0;
// }