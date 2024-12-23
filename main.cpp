#include <iostream>
#include <SDL2/SDL.h>
#include <stdio.h>
#include <cmath>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const int FPS = 60;  // Added frame rate control
const int FRAME_DELAY = 1000 / FPS;

const int GRID_CELLS = 20;
const int CELL_SIZE = std::min(WINDOW_WIDTH, WINDOW_HEIGHT) / GRID_CELLS;
const int GRID_OFFSET_X = (WINDOW_WIDTH - (CELL_SIZE * GRID_CELLS)) / 2;
const int GRID_OFFSET_Y = (WINDOW_HEIGHT - (CELL_SIZE * GRID_CELLS)) / 2;

class Ray {
public:
    float startX, startY;
    float dirX, dirY;
    float currentLength;
    float maxLength;
    float speed;
    
    Ray(float x, float y) 
        : startX(x), startY(y), 
          dirX(1), dirY(1),     // Diagonal direction
          currentLength(0),
          speed(2.0) {          // Slowed down the speed
        calculateMaxLength();
    }
    
    void calculateMaxLength() {
        float gridLeft = GRID_OFFSET_X;
        float gridRight = GRID_OFFSET_X + (GRID_CELLS * CELL_SIZE);
        float gridTop = GRID_OFFSET_Y;
        float gridBottom = GRID_OFFSET_Y + (GRID_CELLS * CELL_SIZE);
        
        float tHor = (dirY > 0) ? (gridBottom - startY) / dirY : (gridTop - startY) / dirY;
        float tVer = (dirX > 0) ? (gridRight - startX) / dirX : (gridLeft - startX) / dirX;
        
        maxLength = std::min(tHor, tVer);
    }
    
    void update() {
        if (currentLength < maxLength) {
            currentLength += speed;
            if (currentLength > maxLength) {
                currentLength = 0;  // Reset to start again
            }
        }
    }
    
    void render(SDL_Renderer* renderer) {
        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);  // Yellow ray
        float endX = startX + dirX * currentLength;
        float endY = startY + dirY * currentLength;
        SDL_RenderDrawLine(renderer, startX, startY, endX, endY);
    }
};

int main(int argc, char* argv[]) {
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL couldn't initialize, error: " << SDL_GetError() << std::endl;
        return 1;
    }

    window = SDL_CreateWindow("Ray Caster", 
                        SDL_WINDOWPOS_CENTERED, 
                        SDL_WINDOWPOS_CENTERED,
                        WINDOW_WIDTH, 
                        WINDOW_HEIGHT, 
                        SDL_WINDOW_SHOWN);

    if (window == nullptr) {
        std::cerr << "Window couldn't be created, error: " << SDL_GetError() << std::endl;
        return 1;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        std::cerr << "Renderer couldn't be created, error: " << SDL_GetError() << std::endl;
        return 1;
    }

    float centerX = GRID_OFFSET_X + (GRID_CELLS * CELL_SIZE) / 2;
    float centerY = GRID_OFFSET_Y + (GRID_CELLS * CELL_SIZE) / 2;
    Ray ray(centerX, centerY);

    bool quit = false;
    Uint32 frameStart;
    int frameTime;

    while (!quit) {
        frameStart = SDL_GetTicks();

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
        }

        ray.update();

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Draw grid
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        
        for (int x = 0; x <= GRID_CELLS; x++) {
            int xPos = GRID_OFFSET_X + (x * CELL_SIZE);
            SDL_RenderDrawLine(renderer, 
                            xPos, 
                            GRID_OFFSET_Y, 
                            xPos, 
                            GRID_OFFSET_Y + (GRID_CELLS * CELL_SIZE));
        }

        for (int y = 0; y <= GRID_CELLS; y++) {
            int yPos = GRID_OFFSET_Y + (y * CELL_SIZE);
            SDL_RenderDrawLine(renderer, 
                            GRID_OFFSET_X, 
                            yPos, 
                            GRID_OFFSET_X + (GRID_CELLS * CELL_SIZE), 
                            yPos);
        }

        ray.render(renderer);
        SDL_RenderPresent(renderer);

        // Frame rate control
        frameTime = SDL_GetTicks() - frameStart;
        if (frameTime < FRAME_DELAY) {
            SDL_Delay(FRAME_DELAY - frameTime);
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}