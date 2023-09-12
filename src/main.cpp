#define SDL_MAIN_HANDLED
// #include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_pixels.h>

#include <filesystem>
#include <thread>
#include <mutex>
#include <chrono>
#include <utility>
#include <algorithm>
#include <random>
#include <vector>
#include <map>
#include <string>
#include <fstream>

#include "complex.h"
#include "surface.h"
#include "saveImage.h"
#include "rgb.h"
#include "smoothRenderer.h"
#include "utilities.h"


std::mutex gMutex;

// const double REAL_MIN { -2 };
// const double REAL_MAX { 1 };

// const double IMAGINARY_MIN { -1 };
// const double IMAGINARY_MAX { 1 };

RGB HSVtoRGBMain(HSV hsv){
    float H {hsv.getHue()};
    float S {hsv.getSaturation()};
    float V {hsv.getValue()};

    if(H>360 || H<0 || S>100 || S<0 || V>100 || V<0){
        // std::cout << "The given HSV values are not in valid range" << std::endl;
        // return;
    }
    float s = S/100;
    float v = V/100;
    float C = s*v;
    float X = C*(1-std::abs(fmod(H/60.0, 2)-1));
    float m = v-C;
    float r,g,b;
    if(H >= 0 && H < 60){
        r = C,g = X,b = 0;
    }
    else if(H >= 60 && H < 120){
        r = X,g = C,b = 0;
    }
    else if(H >= 120 && H < 180){
        r = 0,g = C,b = X;
    }
    else if(H >= 180 && H < 240){
        r = 0,g = X,b = C;
    }
    else if(H >= 240 && H < 300){
        r = X,g = 0,b = C;
    }
    else{
        r = C,g = 0,b = X;
    }
    return RGB((r+m)*255, (g+m)*255, (b+m)*255);
}

std::string reverseString(const std::string& input) {
    std::string reversed = "";
    for (int i = input.length() - 1; i >= 0; i--) {
        reversed += input[i];
    }
    return reversed;
}

std::string toHexString(int value) {
    // Create a string to hold the hexadecimal value.
    std::string hexValue = "";

    // Loop while the value is greater than 0.
    while (value > 0) {
        // Get the remainder of the value when divided by 16.
        int remainder = value % 16;

        // Convert the remainder to a hexadecimal character.
        char hexChar = remainder >= 10 ? (char)(remainder + 55) : (char)(remainder + 48);
        // std::cout << "DEBUG: " << hexChar << std::endl;

        // Append the hexadecimal character to the string.
        hexValue = hexChar + hexValue;

        // Divide the value by 16.
        value = value / 16;
    }

    // Reverse the string.
    //   std::cout << "DEBUG: " << hexValue << std::endl;
    reverseString(hexValue);

    if (hexValue.size() == 1){
        hexValue = "0" + hexValue;
    }
    // std::cout << "DEBUG: " << hexValue << std::endl;
    return hexValue;
}


void writeToFile(int& iteration, std::string allValues){
    std::string fileName = "output" + std::to_string(iteration) + ".txt";

    // Create an output file stream object
    std::ofstream outputFile;

    // Open the file in binary mode to ensure proper handling of special characters
    outputFile.open(fileName, std::ios::binary);

    // Write the string content to the file
    outputFile << allValues;

    // Close the file
    outputFile.close();
}


std::string getFrameName(){
    std::string outputFile;
    std::string folder;
    int fileCount {0};

    if (video){
        folder = "..\\frames\\video";
    }
    else if (coloured){
        folder = "..\\frames\\coloured";
    }
    else{
        folder = "..\\frames\\black_white";
    }

    std::string pathToCheck { folder + "\\frame" + std::to_string(fileCount) + ".bmp" };

    while (std::filesystem::exists(pathToCheck) && std::filesystem::is_regular_file(pathToCheck)){
        fileCount++;
        pathToCheck = folder + "\\frame" + std::to_string(fileCount) + ".bmp";
    }

    return pathToCheck;
}

void calculateMandelbrot(
    SDL_Renderer* renderer,
    std::map<std::pair<int, int>, std::pair<double, double>>& pixelPointMap,
    std::map<std::string, RGB>& pixelColourMap,
    double rMin,
    double rMax,
    double imMin,
    double imMax,
    int x0,
    int x1,
    int y0,
    int y1
    ){
        
    Complex c;
    double xValue;
    double yValue;
    double mandelbrotValue;
    float colour;
    std::string key;
    double hue;
    double saturation {100};
    double value;
    RGB currentRGB;

    for (int x = x0; x < x1; x++){
        for (int y = y0; y < y1; y++){
            xValue = x * static_cast<double>(rMax - rMin) / WIDTH + rMin;
            yValue = y * static_cast<double>(imMax - imMin) / HEIGHT + imMin;

            pixelPointMap[std::make_pair(x, y)] = std::make_pair(xValue, yValue);

            c = Complex(xValue, yValue);
            mandelbrotValue = mandelbrot(c);
            // mandelbrotValue = aPowerZ(c);
            colour = mandelbrotValue * 255 / MAX_ITERATIONS;
            
            hue = mandelbrotValue * 255 / MAX_ITERATIONS;
            key = std::to_string(x) + ":" + std::to_string(y);
            value = mandelbrotValue < MAX_ITERATIONS ? 100 : 0;
            if (coloured){
                currentRGB = RGB(HSVtoRGBMain(HSV(hue, saturation, value)));
            }
            else{
                currentRGB = RGB(colour, colour, colour);
            }
            pixelColourMap[key] = currentRGB;
            gMutex.lock();
            SDL_SetRenderDrawColor(renderer, currentRGB.getRed(), currentRGB.getGreen(), currentRGB.getBlue(), 255);
            SDL_RenderDrawPoint(renderer, x, y);

            auto start = std::chrono::high_resolution_clock::now();
            auto end = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
            // std::cout << "Execution time: " << duration << " milliseconds." << std::endl;
            gMutex.unlock();
        }
    }
}


void renderFrame(SDL_Renderer* renderer, std::map<std::pair<int, int>, std::pair<double, double>>& pixelPointMap, std::map<std::string, RGB>& pixelColourMap, double rMin, double rMax, double imMin, double imMax, int& iteration){
    std::cout << "Calculating" << std::endl << std::endl;
    iteration++;

    SDL_Init(SDL_INIT_VIDEO);
    std::vector<int> colours;
    std::vector<int> pixels;
    std::string hexColour;
    std::vector<std::string> pixelsString;
    std::vector<RGB> rgbs;
    std::vector<const char*> rgbVector;
    
    int numberOfThreads { 4 };
    std::vector<std::thread> threads;
    int x0 { 0 };
    int x1 { 0 }; 
    int y0 { 0 };
    int y1 { 0 };
    int xStep {WIDTH / numberOfThreads};
    int yStep {HEIGHT / numberOfThreads};
    int a;
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < numberOfThreads; i++){
        x0 = i * xStep;
        x1 = (i + 1) * xStep;
        threads.push_back(std::thread(calculateMandelbrot, renderer, std::ref(pixelPointMap), std::ref(pixelColourMap), rMin, rMax, imMin, imMax, x0, x1, 0, HEIGHT));
        // for (int j = 0; j < numberOfThreads; j++){
        //     y0 += j * yStep;
        //     y1 += (j + 1) * yStep;
            
        //     threads.push_back(std::thread(calculateMandelbrot, renderer, std::ref(pixelPointMap), std::ref(pixelColourMap), rMin, rMax, imMin, imMax, x0, x1, y0, y1));
        // }
    }
    for (std::thread & thread : threads){
        thread.join();
    }

    
    calculateMandelbrot(renderer, pixelPointMap, pixelColourMap, rMin, rMax, imMin, imMax, 0, WIDTH, 0, HEIGHT);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "Execution time: " << duration << " milliseconds." << std::endl;
    // saveBMP(WIDTH, HEIGHT, pixelColourMap, getFrameName().c_str());
    SDL_RenderPresent(renderer);
    // auto s = pixelColourMap.size();
    std::cout << "Calculation finished" << std::endl << std::endl;
}

void run(){
    SDL_Event event;
    SDL_Renderer *renderer;
    SDL_Window *window;

    double rMin {REAL_MIN};
    double rMax {REAL_MAX};
    double imMin {IMAGINARY_MIN};
    double imMax {IMAGINARY_MAX};
    int iteration {0};

    std::map<std::string, RGB> pixelColourMap;
    std::map<std::pair<int, int>, std::pair<double, double>> pixelPointMap;

    std::string key;

    for (int x = 0; x < WIDTH; x++){
        for (int y = 0; y < HEIGHT; y++){
            key = std::to_string(x) + ":" + std::to_string(y);
            pixelColourMap[key] = RGB(0, 0, 0);
        }
    }

    SDL_CreateWindowAndRenderer(WIDTH, HEIGHT, 0, &window, &renderer);
    auto start = std::chrono::high_resolution_clock::now();
    renderFrame(renderer, pixelPointMap, pixelColourMap, rMin, rMax, imMin, imMax, iteration);
    std::cout << "RGB: " << pixelColourMap["0:0"].getRed() << ", " << pixelColourMap["0:0"].getGreen() << ", " << pixelColourMap["0:0"].getBlue() << ", " << std::endl;
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "Execution time: " << duration << " milliseconds." << std::endl;

    int x;
    int y;
    int x2;
    int y2;
    bool selecting;
    SDL_Rect rect;

    bool running = true;
    while (running) {
        // Handle events
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                running = false;
                break;
            case SDL_MOUSEBUTTONDOWN:
            // If the left mouse button is down, start selecting an area
                if (event.button.button == SDL_BUTTON_LEFT) {
                    // Store the mouse position
                    x = event.button.x;
                    y = event.button.y;
                    selecting = true;
                }
                if (event.button.button == SDL_BUTTON_RIGHT){
                    saveBMP(WIDTH, HEIGHT, pixelColourMap, getFrameName().c_str());
                }
                break;
            case SDL_MOUSEBUTTONUP:
                // If the left mouse button is up, stop selecting an area
                if (event.button.button == SDL_BUTTON_LEFT) {
                    selecting = false;
                    
                    std::cout << "New border values: " << std::endl;
                    rect.x = std::min(x, x2);
                    rect.w = std::abs(x2 - x);
                    rect.y = std::min(y, y2);
                    rect.h = (double)rect.w * 9 / 16; //std::abs(y2 - y);
                    std::cout << rect.x << " " << rect.x + rect.w << " " << rect.y << " " << rect.y + rect.h << std::endl;
                    
                    // rMin = rect.x * static_cast<double>(rMax - rMin) / WIDTH + rMin;
                    // rMax = (rect.x + rect.w) * static_cast<double>(rMax - rMin) / WIDTH + rMin;
                    // imMin = rect.y * static_cast<double>(imMax - imMin) / HEIGHT + imMin;
                    // imMax = (rect.y + (double)rect.w * 9 / 16) * static_cast<double>(imMax - imMin) / HEIGHT + imMin;
                    
                    auto [rMin, imMin] = pixelPointMap[std::make_pair(rect.x, rect.y)];
                    auto [rMax, imMax] = pixelPointMap[std::make_pair(rect.x + rect.w, rect.y + rect.h)];

                    start = std::chrono::high_resolution_clock::now();
                    renderFrame(renderer, pixelPointMap, pixelColourMap, rMin, rMax, imMin, imMax, iteration);
                    end = std::chrono::high_resolution_clock::now();
                    duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
                    std::cout << "Execution time: " << duration << " milliseconds." << std::endl;
                }
                break;
            case SDL_MOUSEMOTION:
                // If the user is selecting an area, update the selection rectangle
                if (selecting) {
                    // Get the new mouse position
                    x2 = event.motion.x;
                    y2 = event.motion.y;

                    // Update the selection rectangle
                    
                }
                break;
            }
        }
    }


    // bool quit = false;
    // while (!quit) {
    //     while (SDL_PollEvent(&event)) {
    //         if (event.type == SDL_QUIT) {
    //             quit = true;
    //         }
    //     }
    // }

    
    // SDL_SaveBMP(image, "C:\\Users\\vital\\Desktop\\texture.bmp");
    
    // Clean up
    // SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void openMPRun(){
    const int num_iterations = 10;
    int sum = 0;

    #pragma omp parallel for reduction(+:sum)
    for (int i = 0; i < num_iterations; ++i) {
        sum += i;
        std::cout << "Thread " << omp_get_thread_num() << ": i = " << i << std::endl;
    }

    std::cout << "Sum: " << sum << std::endl;

}

int main(){
    SDL_SetMainReady();

    // run();

    SmoothRenderer smoothRenderer;
    smoothRenderer.init();
    smoothRenderer.draw();
    // smoothRenderer.renderFramesSequentially();
    // smoothRenderer.renderFrames();
    // smoothRenderer.renderFramesOpenMP();

    // openMPRun();

    std::cin.get();
    return 0;

}

