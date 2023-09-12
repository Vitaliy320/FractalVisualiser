#pragma once

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <map>
#include <sstream>

#include "rgb.h"
#include "EasyBMP.h"
#include "hsv.h"
#include "utilities.h"
// #include "png.hpp"


void saveBMP(int width, int height, std::map<std::string, RGB> &pixelMap, const char* outputFile);
void saveToBMPOld(int width, int height, std::map<std::string, RGB> pixels, const char* path);
void saveToBMP(const int width, const int height, std::vector<const char *> rgbs, const char* path);
void generateBitmapImage(std::vector<const char*> rgbs, int height, int width, const char* imageFileName);
unsigned char* createBitmapFileHeader(int height, int stride);
unsigned char* createBitmapInfoHeader(int height, int width);
void _separateStringToInts(const std::string& str, int& firstInt, int& secondInt);
// void saveToPNG(std::vector<RGB> rgbs, int width, int height, const char* path);