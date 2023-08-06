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


bool video {false};
bool coloured { false };
std::mutex gMutex;
const int WIDTH { 1920 };   // Width of the window 1200
const int HEIGHT { 1080 };  // Height of the window 800
const int MAX_ITERATIONS { 80 }; // Maximum number of iterations for Mandelbrot algorithm

const int BYTES_PER_PIXEL { 3 };

const double REAL_MIN { -1.6 * 2 };
const double REAL_MAX { 1.6 * 2 };

const double IMAGINARY_MIN { -0.9 * 2 };
const double IMAGINARY_MAX { 0.9 * 2 };

// const double REAL_MIN { -2 };
// const double REAL_MAX { 1 };

// const double IMAGINARY_MIN { -1 };
// const double IMAGINARY_MAX { 1 };

double mandelbrot(Complex c){
    Complex zn(0.0, 0.0);
    int n = 0;
    double returnValue;
    while (n < MAX_ITERATIONS){
        n++;
        // zn.imaginary /= std::log10(zn.absolute());
        // zn.imaginary /= std::sin(zn.real);
        zn = (c - zn.square()).powerN(10);
        // zn = zn + c.square();
        // if (zn.absolute() > std::sin(c.absolute())){
        if (zn.absolute() > 2.0){
            if (n == MAX_ITERATIONS){
                return MAX_ITERATIONS;
            }
            return n + 1 - std::log10(std::log2(zn.absolute()));
        }
    }
    return n;
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
    std::string pathToCheck { folder + "\\frame" + std::to_string(fileCount) + ".bmp" };

    if (video){
        folder = "frames\\video";
    }
    else if (coloured){
        folder = "frames\\coloured";
    }
    else{
        folder = "frames\\black_white";
    }

    while (std::filesystem::is_regular_file(pathToCheck)){
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
        for (int y = x0; y < y1; y++){
            xValue = x * static_cast<double>(rMax - rMin) / WIDTH + rMin;
            yValue = y * static_cast<double>(imMax - imMin) / HEIGHT + imMin;

            pixelPointMap[std::make_pair(x, y)] = std::make_pair(xValue, yValue);

            c = Complex(xValue, yValue);
            mandelbrotValue = mandelbrot(c);
            colour = mandelbrotValue * 255 / MAX_ITERATIONS;
            
            hue = mandelbrotValue * 360 / MAX_ITERATIONS;
            key = std::to_string(x) + ":" + std::to_string(y);
            value = mandelbrotValue < MAX_ITERATIONS ? 100 : 0;
            if (coloured){
                currentRGB = RGB(HSVtoRGB(HSV(hue, saturation, value)));
            }
            else{
                currentRGB = RGB(colour - 255, colour - 255, colour - 255);
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

void calculateMandelbrotByPixel(
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
    std::vector<int> xs, ys;
    std::vector<std::pair<int, int>> pairs;
    BMP image;
    image.SetSize(WIDTH, HEIGHT);
    RGBApixel pixel;
    pixel.Alpha = 0; 

    for (int x = x0; x < x1; x++){
        xs.push_back(x);
    }

    for (int y = y0; y < y1; y++){
        ys.push_back(y);
    }

    std::random_device rd;
    std::mt19937 generator(rd());

    // std::shuffle(xs.begin(), xs.end(), generator);
    // std::shuffle(ys.begin(), ys.end(), generator);

    for (int x : xs){
        for (int y : ys){
            pairs.push_back(std::make_pair(x, y));
        }
    }

    // std::shuffle(pairs.begin(), pairs.end(), generator);

    for (std::pair<int, int> pair : pairs){
        // for (int y : ys){
            // std::cout << "x: " << x << ", y: " << y << std::endl;
            auto [x, y] = pair;
            xValue = x * static_cast<double>(rMax - rMin) / WIDTH + rMin;
            yValue = y * static_cast<double>(imMax - imMin) / HEIGHT + imMin;

            pixelPointMap[std::make_pair(x, y)] = std::make_pair(xValue, yValue);

            c = Complex(xValue, yValue);
            mandelbrotValue = mandelbrot(c);
            colour = mandelbrotValue * 255 / MAX_ITERATIONS;
            
            hue = mandelbrotValue * 360 / MAX_ITERATIONS;
            // key = std::to_string(x) + ":" + std::to_string(y);
            value = mandelbrotValue < MAX_ITERATIONS ? 100 : 0;
            if (coloured){
                currentRGB = RGB(HSVtoRGB(HSV(hue, saturation, value)));
            }
            else{
                currentRGB = RGB(colour, colour, colour);
            }

            gMutex.lock();
            pixel.Red = currentRGB.getRed();
            pixel.Green = currentRGB.getGreen();
            pixel.Blue = currentRGB.getBlue();
            // pixelColourMap[key] = currentRGB;
            image.SetPixel(x, y, pixel);
            // SDL_SetRenderDrawColor(renderer, currentRGB.getRed(), currentRGB.getGreen(), currentRGB.getBlue(), 255);
            // SDL_RenderDrawPoint(renderer, x, y);
            image.WriteToFile(getFrameName().c_str());
            // saveBMP(WIDTH, HEIGHT, pixelColourMap, getFrameName().c_str());
            // SDL_RenderPresent(renderer);
            gMutex.unlock();
        // }
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
    // for (int i = 0; i < numberOfThreads; i++){
    //     x0 = i * xStep;
    //     x1 = (i + 1) * xStep;
    //     threads.push_back(std::thread(calculateMandelbrot, renderer, std::ref(pixelPointMap), std::ref(pixelColourMap), rMin, rMax, imMin, imMax, x0, x1, 0, HEIGHT));
    //     // for (int j = 0; j < numberOfThreads; j++){
    //     //     y0 += j * yStep;
    //     //     y1 += (j + 1) * yStep;
            
    //     //     threads.push_back(std::thread(calculateMandelbrot, renderer, std::ref(pixelPointMap), std::ref(pixelColourMap), rMin, rMax, imMin, imMax, x0, x1, y0, y1));
    //     // }
    // }
    // for (std::thread & thread : threads){
    //     thread.join();
    // }

    
    calculateMandelbrot(renderer, pixelPointMap, pixelColourMap, rMin, rMax, imMin, imMax, 0, WIDTH, 0, HEIGHT);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "Execution time: " << duration << " milliseconds." << std::endl;
    // saveBMP(WIDTH, HEIGHT, pixelColourMap, getFrameName().c_str());
    SDL_RenderPresent(renderer);
    auto s = pixelColourMap.size();
    std::cout << "Calculation finished" << std::endl << std::endl;
}

int main(){
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

    // std::cin.get();
    return 0;

}

