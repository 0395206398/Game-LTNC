#include "SlowDownItem.h"
#include "Constants.h"

SlowDownItem::SlowDownItem(SDL_Texture* tex) {
    x = SCREEN_WIDTH;
    y = SCREEN_HEIGHT - ITEM_HEIGHT - 20;
    speed = 6.0f;
    texture = tex;
}

void SlowDownItem::update() {
    x -= static_cast<int>(speed);
}

SDL_Rect SlowDownItem::getRect() const {
    return {x, y, ITEM_WIDTH, ITEM_HEIGHT};
}

bool SlowDownItem::isOffScreen() const {
    return x < -ITEM_WIDTH;
}
