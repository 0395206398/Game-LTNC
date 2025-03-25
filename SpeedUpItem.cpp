#include "SpeedUpItem.h"
#include "Constants.h"

SpeedUpItem::SpeedUpItem(SDL_Texture* tex) {
    x = SCREEN_WIDTH;
    y = SCREEN_HEIGHT - ITEM_HEIGHT - 20;
    speed = 7.0f;
    texture = tex;
}

void SpeedUpItem::update() {
    x -= static_cast<int>(speed);
}

SDL_Rect SpeedUpItem::getRect() const {
    return {x, y, ITEM_WIDTH, ITEM_HEIGHT};
}

bool SpeedUpItem::isOffScreen() const {
    return x < -ITEM_WIDTH;
}
