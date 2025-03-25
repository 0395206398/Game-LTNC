#ifndef DINO_H
#define DINO_H

#include <SDL.h>

class Dino {
public:
    int x, y;
    int velocity;
    bool isJumping;
    int jumpCount;
    SDL_Texture* runTexture1;
    SDL_Texture* runTexture2;
    SDL_Texture* jumpTexture;
    Uint32 lastFrameTime;
    bool currentRunFrame;
    const Uint32 frameDelay = 200;
    float speedMultiplier;
    Uint32 speedEffectEndTime;

    Dino(SDL_Texture* tex1, SDL_Texture* tex2, SDL_Texture* jumpTex);
    Dino(const Dino&) = delete;
    Dino& operator=(const Dino&) = delete;

    void reset(SDL_Texture* tex1, SDL_Texture* tex2, SDL_Texture* jumpTex);
    void jump();
    void applySpeedEffect(float multiplier);
    void update();
    SDL_Rect getRect();
    SDL_Rect getCollisionRect() const;
    SDL_Texture* getCurrentTexture();
};

#endif // DINO_H
