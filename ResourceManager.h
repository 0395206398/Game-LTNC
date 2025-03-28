#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <iostream>

class ResourceManager {
public:
    SDL_Texture* backgroundTexture;
    SDL_Texture* gameStartTexture;
    SDL_Texture* gameOverTexture;
    SDL_Texture* obstacleTexture;
    SDL_Texture* flyingObstacleTexture;
    SDL_Texture* speedUpItemTexture;
    SDL_Texture* slowDownItemTexture;
    SDL_Texture* invincibilityItemTexture; // Thêm texture cho item bất tử
    SDL_Texture* runTexture1;
    SDL_Texture* runTexture2;
    SDL_Texture* jumpTexture;
    SDL_Texture* restartButtonTexture;
    SDL_Texture* quitButtonTexture;
    SDL_Texture* startButtonTexture;
    Mix_Chunk* jumpSound;
    Mix_Chunk* gameOverSound;

    ResourceManager(SDL_Renderer* renderer);
    ~ResourceManager();
};

#endif
