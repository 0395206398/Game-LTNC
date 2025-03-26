#include "Dino.h"
#include "Constants.h"

Dino::Dino(SDL_Texture* tex1, SDL_Texture* tex2, SDL_Texture* jumpTex) {
    x = 50;
    y = SCREEN_HEIGHT - DINO_HEIGHT;
    velocity = 0;
    isJumping = false;
    jumpCount = 0;
    runTexture1 = tex1;
    runTexture2 = tex2;
    jumpTexture = jumpTex;
    lastFrameTime = 0;
    currentRunFrame = false;
    speedMultiplier = 1.0f;
    speedEffectEndTime = 0;
    isInvincible = false;       // Khởi tạo bất tử
    invincibilityEndTime = 0;   // Khởi tạo thời gian bất tử
}

void Dino::reset(SDL_Texture* tex1, SDL_Texture* tex2, SDL_Texture* jumpTex) {
    x = 50;
    y = SCREEN_HEIGHT - DINO_HEIGHT;
    velocity = 0;
    isJumping = false;
    jumpCount = 0;
    runTexture1 = tex1;
    runTexture2 = tex2;
    jumpTexture = jumpTex;
    lastFrameTime = 0;
    currentRunFrame = false;
    speedMultiplier = 1.0f;
    speedEffectEndTime = 0;
    isInvincible = false;       // Reset bất tử
    invincibilityEndTime = 0;   // Reset thời gian bất tử
}

void Dino::jump() {
    if (jumpCount < 2) {
        velocity = -15;
        isJumping = true;
        jumpCount++;
    }
}

void Dino::applySpeedEffect(float multiplier) {
    speedMultiplier = multiplier;
    speedEffectEndTime = SDL_GetTicks() + SPEED_EFFECT_DURATION;
}

void Dino::applyInvincibility() { // Triển khai phương thức bất tử
    isInvincible = true;
    invincibilityEndTime = SDL_GetTicks() + 5000; // 5 giây
}

bool Dino::checkInvincibility() const { // Kiểm tra bất tử
    if (isInvincible && SDL_GetTicks() > invincibilityEndTime) {
        return false; // Hết thời gian bất tử
    }
    return isInvincible;
}

void Dino::update() {
    y += velocity;
    velocity += 1;

    if (y >= SCREEN_HEIGHT - DINO_HEIGHT) {
        y = SCREEN_HEIGHT - DINO_HEIGHT;
        velocity = 0;
        isJumping = false;
        jumpCount = 0;
    }

    if (speedEffectEndTime > 0 && SDL_GetTicks() > speedEffectEndTime) {
        speedMultiplier = 1.0f;
        speedEffectEndTime = 0;
    }
}

SDL_Rect Dino::getRect() {
    return {x, y, DINO_WIDTH, DINO_HEIGHT};
}

SDL_Rect Dino::getCollisionRect() const {
    return {x, y, DINO_WIDTH, DINO_HEIGHT};
}

SDL_Texture* Dino::getCurrentTexture() {
    if (isJumping) {
        return jumpTexture;
    } else {
        Uint32 currentTime = SDL_GetTicks();
        if (currentTime - lastFrameTime >= frameDelay) {
            currentRunFrame = !currentRunFrame;
            lastFrameTime = currentTime;
        }
        return currentRunFrame ? runTexture2 : runTexture1;
    }
}
