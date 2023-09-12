
#include "smoothRenderer.h"
std::mutex mutexLock;

std::chrono::_V2::system_clock::time_point start; //= std::chrono::high_resolution_clock::now();
std::chrono::_V2::system_clock::time_point end;
int64_t duration;


void SmoothRenderer::init(){
    SDL_Init(SDL_INIT_VIDEO);
    std::shared_ptr<Pixel> currentPixel;

    SDL_CreateWindowAndRenderer(WIDTH, HEIGHT, 0, &_window, &_renderer);

    _rMin = REAL_MIN;
    _rMax = REAL_MAX;
    _imMin = IMAGINARY_MIN;
    _imMax = IMAGINARY_MAX;
    _maxIterations = MAX_ITERATIONS;

    _width = WIDTH;
    _height = HEIGHT;

    double realPart;
    double imaginaryPart;

    int pixelCount {0};
    for (int x = 0; x < WIDTH; x++){
        for (int y = 0; y < HEIGHT; y++){
            realPart = x * static_cast<double>(_rMax - _rMin) / WIDTH + _rMin;
            imaginaryPart = y * static_cast<double>(_imMax - _imMin) / HEIGHT + _imMin;

            currentPixel = std::make_shared<Pixel>(Pixel(x, y, realPart, imaginaryPart, RGB(0, 0, 0), 0, MAX_ITERATIONS));
            _pixelMap[coordinatesToStringKey(x, y)] = currentPixel;
            _pixels.push_back(currentPixel);
        }
    }
}

void SmoothRenderer::functionTest(int & iteration, int & threadID){
    std::cout << "Thread ID: " << threadID << "Iteration: " << iteration << std::endl;
}

void SmoothRenderer::_calculatePixels(int firstIndex, int lastIndex, int& iteration, int& threadId){
    std::pair<int, int> pixelCoordinates;
    double mandelbrotValue;
    double hue;
    double saturation {100};
    double value;
    RGB currentRGB;
    double colour;
    std::shared_ptr<Pixel> pixel;
    
    
    // mutexLock.lock();
    // std::cout << "DEBUG INFO: thread " << threadId << ", iteration " << iteration << std::endl;
    // mutexLock.unlock();

    for (int i = firstIndex; i < lastIndex - 1; i++){
        pixel = _pixels[i];
        if (!pixel->getEscaped()){
            pixelCoordinates = pixel->getCoordinates();
            // std::cout << "Pixel coordinates: " << pixelCoordinates.first << ", " << pixelCoordinates.second << std::endl;
            mandelbrotValue = mandelbrotAtIteration(pixel);

            colour = /*std::sin(mandelbrotValue) **/ mandelbrotValue * 255 / MAX_ITERATIONS;
            hue = mandelbrotValue * 255 / MAX_ITERATIONS;

            value = mandelbrotValue < MAX_ITERATIONS ? 100 : 0;
            
            if (coloured){
                currentRGB = RGB(HSVtoRGB(HSV(hue, saturation, value)));
            }
            else{
                currentRGB = RGB(colour, colour, colour);
            }

            pixel->setRGB(currentRGB);

            
            mutexLock.lock();
            SDL_SetRenderDrawColor(this->_renderer, currentRGB.getRed(), currentRGB.getGreen(), currentRGB.getBlue(), 255);
            // mutexLock.lock();
            SDL_RenderDrawPoint(this->_renderer, pixelCoordinates.first, pixelCoordinates.second);
            mutexLock.unlock();
        }
    }
    // mutexLock.unlock();
}

void SmoothRenderer::renderFramesOpenMP(){
    std::cout << "Calculation started" << std::endl;
    std::string key;
    std::shared_ptr<Pixel> pixel;
    
    int numberOfThreads { 2 };
    
    // std::thread thread_;
    int firstIndex { 0 };
    int lastIndex { 0 };

    std::vector<Pixel>::size_type vectorSizeType { _pixels.size() };
    std::vector<Pixel>::size_type threadSize { _pixels.size() / static_cast<std::vector<Pixel>::size_type>(numberOfThreads) };

    int currentDepth{0};
    int currentIteration{0};
    int threadId{0};

    for (int iteration = 0; iteration < _maxIterations; iteration++){
        #pragma omp parallel for
        for (int i = 0; i < numberOfThreads; i++){
            firstIndex = i * threadSize;
            lastIndex = (i + 1) * threadSize;
            _calculatePixels(firstIndex, lastIndex, iteration, i);
        }

        SDL_RenderPresent(_renderer);
    }

    std::cout << "Calculation finished" << std::endl;
}


void SmoothRenderer::renderFrames(){
    std::cout << "Calculation started" << std::endl;
    std::string key;
    std::shared_ptr<Pixel> pixel;
    
    int numberOfThreads { 4 };
    
    // std::thread thread_;
    int firstIndex { 0 };
    int lastIndex { 0 };

    std::vector<Pixel>::size_type vectorSizeType { _pixels.size() };
    std::vector<Pixel>::size_type threadSize { _pixels.size() / static_cast<std::vector<Pixel>::size_type>(numberOfThreads) };

    int currentDepth{0};
    int currentIteration{0};
    int threadId{0};

    for (int iteration = 0; iteration < _maxIterations; iteration++){
        std::vector<std::thread> threads;
        for (int i = 0; i < numberOfThreads; i++){
            firstIndex = i * threadSize;
            lastIndex = (i + 1) * threadSize;
            threads.push_back(
                std::thread(
                    &SmoothRenderer::_calculatePixels, 
                    this, 
                    firstIndex,
                    lastIndex,
                    std::ref(iteration),
                    std::ref(i)
                    // &SmoothRenderer::functionTest,
                    // this,
                    // std::ref(iteration),
                    // std::ref(i)
                )
            );
            
            // threads.push_back(std::thread(
            //     &SmoothRenderer::_calculatePixels, 
            //     this, 
            //     std::ref(firstIndex), 
            //     std::ref(lastIndex),
            //     std::ref(iteration),
            //     std::ref(i)
            //     ));
            // threads.push_back(thread_);
        }

        for (int i = 0; i < numberOfThreads; i++){
            // if (threads[i].joinable()){
                threads[i].join();
            // }
        }
        
        // for (std::shared_ptr<Pixel> pixel : _pixels){
        //     SDL_SetRenderDrawColor(this->_renderer, pixel->getRGB().getRed(), pixel->getRGB().getGreen(), pixel->getRGB().getBlue(), 255);
        //     SDL_RenderDrawPoint(this->_renderer, pixel->getCoordinates().first, pixel->getCoordinates().second);
        // }

        SDL_RenderPresent(_renderer);
    }

    std::cout << "Calculation finished" << std::endl;
}


void SmoothRenderer::renderFramesSequentially(){
    std::cout << "Calculation started" << std::endl;
    std::string key;
    std::shared_ptr<Pixel> pixel;
    double mandelbrotValue;
    double hue;
    double saturation {100};
    double value;
    RGB currentRGB;
    double colour;
    std::pair<int, int> pixelCoordinates;

    for (int iteration = 0; iteration < _maxIterations; iteration++){

        start = std::chrono::high_resolution_clock::now();
        for (std::shared_ptr<Pixel> pixel : _pixels){
            // std::cout << pixel->getX() << ", " << pixel->getY() << std::endl;
            // if (pixel->getX() == 1920/2 && pixel->getY() == 1080/2){
            //     std::cout << "we're here" << std::endl;
            // }
            if (!pixel->getEscaped()){
                pixelCoordinates = pixel->getCoordinates();

                // start = std::chrono::high_resolution_clock::now();
                mandelbrotValue = mandelbrotAtIteration(pixel);
                // end = std::chrono::high_resolution_clock::now();
                // duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
                // std::cout << "Mandelbrot execution time: " << duration << " microseconds." << std::endl;

                colour = /*std::sin(mandelbrotValue) **/ mandelbrotValue * 255 / MAX_ITERATIONS;
                hue = mandelbrotValue * 255 / MAX_ITERATIONS;

                value = mandelbrotValue < MAX_ITERATIONS ? 100 : 0;
                
                // start = std::chrono::high_resolution_clock::now();
                if (coloured){
                    currentRGB = RGB(HSVtoRGB(HSV(hue, saturation, value)));
                }
                else{
                    currentRGB = RGB(colour, colour, colour);
                }
                // end = std::chrono::high_resolution_clock::now();
                // duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
                // std::cout << "Colouring execution time: " << duration << " microseconds." << std::endl;

                pixel->setRGB(currentRGB);

                // start = std::chrono::high_resolution_clock::now();
                SDL_SetRenderDrawColor(_renderer, currentRGB.getRed(), currentRGB.getGreen(), currentRGB.getBlue(), 255);
                SDL_RenderDrawPoint(_renderer, pixelCoordinates.first, pixelCoordinates.second);
                // end = std::chrono::high_resolution_clock::now();
                // duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
                // std::cout << "Pixel colouring execution time: " << duration << " microseconds." << std::endl;
            }
        }

        end = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        std::cout << "Iteration execution time: " << duration << " microseconds." << std::endl;

        start = std::chrono::high_resolution_clock::now();
        SDL_RenderPresent(_renderer);
        end = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        // std::cout << "Rendering execution time: " << duration << " microseconds." << std::endl;
    }

    auto currRGB = _pixelMap[coordinatesToStringKey(0, 0)]->getRGB();

    // std::cout << "RGB: " << currRGB.getRed() << ", " << currRGB.getGreen() << ", " << currRGB.getBlue() << ", " << std::endl;
    std::cout << "Calculation finished" << std::endl;
}

void SmoothRenderer::_calculationThread(){
    std::cout << "Calculation started" << std::endl;
    std::string key;
    std::shared_ptr<Pixel> pixel;
    double mandelbrotValue;
    double hue;
    double saturation {100};
    double value;
    RGB currentRGB;
    double colour;
    std::pair<int, int> pixelCoordinates;
    

    for (int iteration = 0; iteration < _maxIterations; iteration++){
        std::vector<Pixel> pixels;

        for (std::shared_ptr<Pixel> pixel : _pixels){
            if (!pixel->getEscaped()){
                pixelCoordinates = pixel->getCoordinates();

                mandelbrotValue = mandelbrotAtIteration(pixel);
                colour = mandelbrotValue * 255 / MAX_ITERATIONS;
                hue = mandelbrotValue * 255 / MAX_ITERATIONS;

                value = mandelbrotValue < MAX_ITERATIONS ? 100 : 0;
                
                if (coloured){
                    currentRGB = RGB(HSVtoRGB(HSV(hue, saturation, value)));
                }
                else{
                    currentRGB = RGB(colour, colour, colour);
                }
                pixel->setRGB(currentRGB);
                pixels.push_back(*(pixel));
            }
        }
        _queue.push(pixels);
        
    }

    std::cout << "Calculation finished" << std::endl;
}

void SmoothRenderer::_rendererThread(){
    std::condition_variable cv;
    std::vector<Pixel> pixels;
    RGB rgb;

    while (true){
        // mutexLock.lock();
        // std::unique_lock<std::mutex> lock(mutexLock);
        // std::unique_lock<std::mutex> lock(mtx);
        
        // cv.wait(lock, [this]{ return !_queue.empty(); });

        if (!_queue.empty()){
            pixels = _queue.front();
            _queue.pop();
            for (auto pixel : pixels){
                mutexLock.lock();
                rgb = pixel.getRGB();
                SDL_SetRenderDrawColor(_renderer, rgb.getRed(), rgb.getGreen(), rgb.getBlue(), 255);
                SDL_RenderDrawPoint(_renderer, pixel.getCoordinates().first, pixel.getCoordinates().second);
                mutexLock.unlock();
            }

            SDL_RenderPresent(_renderer);    
        }
    }
}

void SmoothRenderer::draw(){
    std::thread calculationThread(&SmoothRenderer::_calculationThread, this);
    std::thread rendererThread(&SmoothRenderer::_rendererThread, this);

    calculationThread.join();
    rendererThread.join();
}