#include "saveImage.h"
#include <stdio.h>

void saveToBMPOld(int width, int height, std::vector<std::string> pixels, const char* path){
    int row { 0 };
    int column;

    FILE* file = fopen(path, "wb");
    std::string text {""};

    // Write the header to the file.
    fwrite("BM", 1, 2, file);
    fwrite(&width, sizeof(int), 1, file);
    fwrite(&height, sizeof(int), 1, file);
    fwrite(0, 1, 4, file);
    for (int i = 0; i < pixels.size(); i++){
        if (i + 1 == width){
            row++;
        }
        fwrite(&pixels[i], sizeof(int), 1, file);
    }
    
    // for (int i = 0; i < width; i++) {
    //     for (int j = 0; j < height; j++) {
    //         fwrite(&pixels[i][j], sizeof(int), 1, file);
    //     }
    // }
}



const int BYTES_PER_PIXEL = 3; /// red, green, & blue
const int FILE_HEADER_SIZE = 14;
const int INFO_HEADER_SIZE = 40;


int _stringToInt(const std::string& str) {
    // Use std::istringstream to convert string to integer
    std::istringstream iss(str);
    int result = 0;
    iss >> result;
    return result;
}

void _separateStringToInts(const std::string& str, int& firstInt, int& secondInt) {
    // Use std::istringstream to extract integers from the string
    std::istringstream iss(str);
    char delimiter;
    iss >> firstInt >> delimiter >> secondInt;
}

void saveToBMP(const int width, const int height, std::vector<const char*> rgbs , const char* path)
{   
    // std::vector<std::vector<std::vector<char>>> image;
    // unsigned char*** image = new unsigned char**[3];
    const int height_ = 361;
    const int width_ = 867;
    // std::string image[height_][width_][BYTES_PER_PIXEL];
    char* imageFileName = (char*) path;
    int xCoordinate;
    int yCoordinate;
    RGB currentRGB;

    // int i, j;
    // for (i = 0; i < height_; i++) {
    //     for (j = 0; j < width_; j++) {
            // currentRGB = rgbs[std::to_string(j) + ":" + std::to_string(i)];
            // image[i][j][2] = std::to_string(currentRGB.getRed());             ///red
            // image[i][j][1] = std::to_string(currentRGB.getGreen());              ///green
            // image[i][j][0] = std::to_string(currentRGB.getBlue()); ///blue
    //     }
    // }

    // generateBitmapImage(rgbs, height_, width_, imageFileName);
    // generateBitmapImage(rgbs, height, width_, imageFileName);
    printf("Image generated!!");
}

RGB HSVtoRGB(HSV hsv){
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
    float X = C*(1-abs(fmod(H/60.0, 2)-1));
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

void saveBMP(int width, int height, std::map<std::string, RGB>& pixelMap, const char* outputFile){
    BMP image;
    RGB rgb;
    image.SetSize(width, height);

    // Set pixel colors
    for (int x = 0; x < width; x++){
        for (int y = 0; y < height; y++) {
            rgb = pixelMap[std::to_string(x) + ":" + std::to_string(y)];
            RGBApixel pixel;
            pixel.Red = rgb.getRed();     // Vary the red component with the x-coordinate
            pixel.Green = rgb.getGreen();   // Vary the green component with the y-coordinate
            pixel.Blue = rgb.getBlue(); // Vary the blue component with the sum of x and y
            pixel.Alpha = 0;         // Alpha channel (transparency), set to 0 (fully opaque)

            // Set the pixel color at the current (x, y) position
            image.SetPixel(x, y, pixel);
        }
    }

    // Save the image to a BMP file named "output.bmp"
    image.WriteToFile(outputFile);
}

void generateBitmapImage(std::vector<const char*> rgbs, int height, int width, const char* imageFileName)
{
    int widthInBytes = width * BYTES_PER_PIXEL;

    unsigned char padding[3] = {0, 0, 0};
    int paddingSize = (4 - (widthInBytes) % 4) % 4;

    int stride = (widthInBytes) + paddingSize;

    FILE* imageFile = fopen(imageFileName, "wb");

    unsigned char* fileHeader = createBitmapFileHeader(height, stride);
    fwrite(fileHeader, 1, FILE_HEADER_SIZE, imageFile);

    unsigned char* infoHeader = createBitmapInfoHeader(height, width);
    fwrite(infoHeader, 1, INFO_HEADER_SIZE, imageFile);

    int i;
    std::string colour;
    // const char * colourChar;
    std::cout<< "DEBUG MESSAGE: " << rgbs[0] << std::endl;
    for (i = 0; i < height; i++) {
        // colour = std::to_string(rgbs[i]);
        // std::cout << "DEBUG MESSAGE: " << image << std::endl << image + (i*widthInBytes) << std::endl;
        // colourChar = colour.c_str();
        fwrite(rgbs[i], BYTES_PER_PIXEL, width, imageFile);
        fwrite(padding, 1, paddingSize, imageFile);
    }

    fclose(imageFile);
}

unsigned char* createBitmapFileHeader (int height, int stride)
{
    int fileSize = FILE_HEADER_SIZE + INFO_HEADER_SIZE + (stride * height);

    static unsigned char fileHeader[] = {
        0,0,     /// signature
        0,0,0,0, /// image file size in bytes
        0,0,0,0, /// reserved
        0,0,0,0, /// start of pixel array
    };

    fileHeader[ 0] = (unsigned char)('B');
    fileHeader[ 1] = (unsigned char)('M');
    fileHeader[ 2] = (unsigned char)(fileSize      );
    fileHeader[ 3] = (unsigned char)(fileSize >>  8);
    fileHeader[ 4] = (unsigned char)(fileSize >> 16);
    fileHeader[ 5] = (unsigned char)(fileSize >> 24);
    fileHeader[10] = (unsigned char)(FILE_HEADER_SIZE + INFO_HEADER_SIZE);

    return fileHeader;
}

unsigned char* createBitmapInfoHeader (int height, int width)

{
    static unsigned char infoHeader[] = {
        0,0,0,0, /// header size
        0,0,0,0, /// image width
        0,0,0,0, /// image height
        0,0,     /// number of color planes
        0,0,     /// bits per pixel
        0,0,0,0, /// compression
        0,0,0,0, /// image size
        0,0,0,0, /// horizontal resolution
        0,0,0,0, /// vertical resolution
        0,0,0,0, /// colors in color table
        0,0,0,0, /// important color count
    };

    infoHeader[ 0] = (unsigned char)(INFO_HEADER_SIZE);
    infoHeader[ 4] = (unsigned char)(width      );
    infoHeader[ 5] = (unsigned char)(width >>  8);
    infoHeader[ 6] = (unsigned char)(width >> 16);
    infoHeader[ 7] = (unsigned char)(width >> 24);
    infoHeader[ 8] = (unsigned char)(height      );
    infoHeader[ 9] = (unsigned char)(height >>  8);
    infoHeader[10] = (unsigned char)(height >> 16);
    infoHeader[11] = (unsigned char)(height >> 24);
    infoHeader[12] = (unsigned char)(1);
    infoHeader[14] = (unsigned char)(BYTES_PER_PIXEL*8);

    return infoHeader;
}


// void saveToPNG(std::vector<RGB> rgbs, int width, int height, const char* path){
//     png::image<png::rgb_pixel> image(100, 100);
//     std::vector<png::rgb_pixel> colours;

//     for (RGB rgb: rgbs){
//         colours.push_back(png::rgb_pixel(rgb.getRed(), rgb.getGreen(), rgb.getBlue()));

//         for (int i = 0; i < width; i++){
//             for (int j = 0; j < height; j++){
//                 image.set_pixel(i, j, colours[i % colours.size()]);
//             }
//         }
//     }

//     image.write(path);
// }
