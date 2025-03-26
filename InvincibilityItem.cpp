#include "InvincibilityItem.h"
#include "Constants.h"

InvincibilityItem::InvincibilityItem(SDL_Texture* texture)
    : x(SCREEN_WIDTH), y(SCREEN_HEIGHT - GROUND_HEIGHT - HEIGHT - 50), speed(5.0f), texture(texture) {}

void InvincibilityItem::update() {
    x -= speed;
}

SDL_Rect InvincibilityItem::getRect() const {
    SDL_Rect rect = { static_cast<int>(x), static_cast<int>(y), WIDTH, HEIGHT };
    return rect;
}

bool InvincibilityItem::isOffScreen() const {
    return x + WIDTH < 0;
}
