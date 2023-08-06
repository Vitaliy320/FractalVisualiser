// #define SDL_MAIN_HANDLED
// #include <iostream>
// #include <SDL2/SDL.h>

// const int WIDTH = 800;   // Width of the window
// const int HEIGHT = 600;  // Height of the window
// const int MAX_ITER = 10; // Maximum number of iterations for Mandelbrot algorithm

// // Function to calculate the color of a pixel using the Mandelbrot algorithm
// SDL_Color calculateMandelbrotColor(int x, int y) {
//     double real = (x - WIDTH / 2.0) * 4.0 / WIDTH;
//     double imag = (y - HEIGHT / 2.0) * 4.0 / HEIGHT;

//     double realTemp, imagTemp, real2, imag2;
//     int iter;
//     for (iter = 0, real2 = real * real, imag2 = imag * imag;
//          iter < MAX_ITER && real2 + imag2 <= 4.0;
//          iter++, imag = imagTemp, real = realTemp) {
//         realTemp = real2 - imag2 + real;
//         imagTemp = 2 * real * imag + imag;
//         real2 = realTemp * realTemp;
//         imag2 = imagTemp * imagTemp;
//     }

//     SDL_Color color;
//     if (iter == MAX_ITER) {
//         color.r = 0;
//         color.g = 0;
//         color.b = 0;
//     } else {
//         // Color mapping based on the number of iterations
//         // Adjust these formulas to change the colors as desired
//         color.r = (iter * 8) % 256;       // Red component
//         color.g = (iter * 16) % 256;      // Green component
//         color.b = (iter * 32) % 256;      // Blue component
//     }

//     return color;
// }

// int main() {
//     SDL_Init(SDL_INIT_VIDEO);

//     // Create the window
//     SDL_Window* window = SDL_CreateWindow("Mandelbrot Set", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
//     if (!window) {
//         std::cerr << "SDL window creation failed: " << SDL_GetError() << std::endl;
//         return 1;
//     }

//     // Create the renderer
//     SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
//     if (!renderer) {
//         std::cerr << "SDL renderer creation failed: " << SDL_GetError() << std::endl;
//         return 1;
//     }

//     // Create a texture to draw on
//     SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STATIC, WIDTH, HEIGHT);
//     if (!texture) {
//         std::cerr << "SDL texture creation failed: " << SDL_GetError() << std::endl;
//         return 1;
//     }

//     // Array to store pixel colors
//     Uint32* pixels = new Uint32[WIDTH * HEIGHT];

//     // Calculate colors for each pixel
//     for (int y = 0; y < HEIGHT; y++) {
//         for (int x = 0; x < WIDTH; x++) {
//             SDL_Color color = calculateMandelbrotColor(x, y);
//             pixels[y * WIDTH + x] = SDL_MapRGBA(SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888), color.r, color.g, color.b, 255);
//         }
//     }

//     // Update the texture with the calculated pixel colors
//     SDL_UpdateTexture(texture, nullptr, pixels, WIDTH * sizeof(Uint32));

//     // Clear the renderer and draw the texture
//     SDL_RenderClear(renderer);
//     SDL_RenderCopy(renderer, texture, nullptr, nullptr);
//     SDL_RenderPresent(renderer);

//     // Main loop
//     bool quit = false;
//     SDL_Event event;
//     while (!quit) {
//         while (SDL_PollEvent(&event)) {
//             if (event.type == SDL_QUIT) {
//                 quit = true;
//             }
//         }
//     }

//     // Clean up
//     delete[] pixels;
//     // SDL_DestroyTexture(texture);
//     SDL_DestroyRenderer(renderer);
//     SDL_DestroyWindow(window);
//     SDL_Quit();

//     return 0;
// }
