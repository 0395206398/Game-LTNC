#include "ResourceManager.h"

ResourceManager::ResourceManager(SDL_Renderer* renderer) {
    SDL_Surface* surface;

    surface = IMG_Load("background.png");
    if (!surface) {
        std::cout << "Failed to load background.png! IMG_Error: " << IMG_GetError() << std::endl;
    }
    backgroundTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("gamestart.png");
    if (!surface) {
        std::cout << "Failed to load gamestart.png! IMG_Error: " << IMG_GetError() << std::endl;
    }
    gameStartTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("gameover.jpg");
    if (!surface) {
        std::cout << "Failed to load gameover.jpg! IMG_Error: " << IMG_GetError() << std::endl;
    }
    gameOverTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("13.png");
    if (!surface) {
        std::cout << "Failed to load 13.png! IMG_Error: " << IMG_GetError() << std::endl;
    }
    obstacleTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("flying_obstacle.png");
    if (!surface) {
        std::cout << "Failed to load flying_obstacle.png! IMG_Error: " << IMG_GetError() << std::endl;
    }
    flyingObstacleTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("speedup.png");
    if (!surface) {
        std::cout << "Failed to load speedup.png! IMG_Error: " << IMG_GetError() << std::endl;
    }
    speedUpItemTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("slowdown.png");
    if (!surface) {
        std::cout << "Failed to load slowdown.png! IMG_Error: " << IMG_GetError() << std::endl;
    }
    slowDownItemTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("invincibility.png"); // Tải texture cho item bất tử
    if (!surface) {
        std::cout << "Failed to load invincibility.png! IMG_Error: " << IMG_GetError() << std::endl;
    }
    invincibilityItemTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("vegitorun1.png");
    if (!surface) {
        std::cout << "Failed to load vegitorun1.png! IMG_Error: " << IMG_GetError() << std::endl;
    }
    runTexture1 = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("vegitorun2.png");
    if (!surface) {
        std::cout << "Failed to load vegitorun2.png! IMG_Error: " << IMG_GetError() << std::endl;
    }
    runTexture2 = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("vegitojump.png");
    if (!surface) {
        std::cout << "Failed to load vegitojump.png! IMG_Error: " << IMG_GetError() << std::endl;
    }
    jumpTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    jumpSound = Mix_LoadWAV("jump.wav");
    if (!jumpSound) {
        std::cout << "Failed to load jump.wav! Mix_Error: " << Mix_GetError() << std::endl;
    }

    gameOverSound = Mix_LoadWAV("gameover.wav");
    if (!gameOverSound) {
        std::cout << "Failed to load gameover.wav! Mix_Error: " << Mix_GetError() << std::endl;
    }

    surface = IMG_Load("restart_button.png");
    if (!surface) {
        std::cout << "Failed to load restart_button.png! IMG_Error: " << IMG_GetError() << std::endl;
    }
    restartButtonTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("quit_button.png");
    if (!surface) {
        std::cout << "Failed to load quit_button.png! IMG_Error: " << IMG_GetError() << std::endl;
    }
    quitButtonTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("start_button.png");
    if (!surface) {
        std::cout << "Failed to load start_button.png! IMG_Error: " << IMG_GetError() << std::endl;
    }
    startButtonTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
}

ResourceManager::~ResourceManager() {
    SDL_DestroyTexture(backgroundTexture);
    SDL_DestroyTexture(gameStartTexture);
    SDL_DestroyTexture(gameOverTexture);
    SDL_DestroyTexture(obstacleTexture);
    SDL_DestroyTexture(flyingObstacleTexture);
    SDL_DestroyTexture(speedUpItemTexture);
    SDL_DestroyTexture(slowDownItemTexture);
    SDL_DestroyTexture(invincibilityItemTexture); // Giải phóng texture bất tử
    SDL_DestroyTexture(runTexture1);
    SDL_DestroyTexture(runTexture2);
    SDL_DestroyTexture(jumpTexture);
    SDL_DestroyTexture(restartButtonTexture);
    SDL_DestroyTexture(quitButtonTexture);
    SDL_DestroyTexture(startButtonTexture);
    Mix_FreeChunk(jumpSound);
    Mix_FreeChunk(gameOverSound);
}
