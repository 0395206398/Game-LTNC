#include <iostream>
#include <SDL.h>
#include <SDL_image.h>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const char* WINDOW_TITLE = "SDL2 Image Loading";

void logErrorAndExit(const std::string &msg, const std::string &error) {
    std::cerr << msg << " " << error << std::endl;
    SDL_Quit();
    exit(1);
}

SDL_Window* initSDL(int width, int height, const char* title, SDL_Renderer*& renderer) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        logErrorAndExit("SDL_Init error:", SDL_GetError());
    }

    SDL_Window* window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
    if (!window) {
        logErrorAndExit("Create window error:", SDL_GetError());
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        logErrorAndExit("Create renderer error:", SDL_GetError());
    }

    if (!(IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) & (IMG_INIT_PNG | IMG_INIT_JPG))) {
        logErrorAndExit("SDL_image error:", IMG_GetError());
    }

    return window;
}

void quitSDL(SDL_Window* window, SDL_Renderer* renderer) {
    IMG_Quit();
    if (renderer) SDL_DestroyRenderer(renderer);
    if (window) SDL_DestroyWindow(window);
    SDL_Quit();
}

SDL_Texture* loadTexture(const char* filename, SDL_Renderer* renderer) {
    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Loading %s", filename);
    SDL_Texture* texture = IMG_LoadTexture(renderer, filename);
    if (!texture) {
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "Load texture %s", IMG_GetError());
    }
    return texture;
}

void waitUntilKeyPressed() {
    SDL_Event e;
    while (true) {
        if (SDL_WaitEvent(&e) != 0 && e.type == SDL_KEYDOWN) {
            return;
        }
    }
}

int main(int argc, char* argv[]) {
    SDL_Renderer* renderer = nullptr;
    SDL_Window* window = initSDL(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE, renderer);

    SDL_Texture* background = loadTexture("bikiniBottom.jpg", renderer);
    if (background) {
        SDL_RenderCopy(renderer, background, NULL, NULL);
        SDL_RenderPresent(renderer);
    }

    waitUntilKeyPressed();

    SDL_DestroyTexture(background);
    background = NULL;
    quitSDL(window, renderer);
    return 0;
}
