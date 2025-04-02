#include "Obstacle.h"
#include "Constants.h"

Obstacle::Obstacle(SDL_Texture* tex) {
    x = SCREEN_WIDTH;
    y = SCREEN_HEIGHT - OBSTACLE_HEIGHT;
    speed = 6.0f;
    passed = false;
    texture = tex;
}

void Obstacle::update() {
    x -= static_cast<int>(speed);
}

SDL_Rect Obstacle::getRect() const {
    return {x, y, OBSTACLE_WIDTH, OBSTACLE_HEIGHT};
}

bool Obstacle::isOffScreen() const {
    return x < -OBSTACLE_WIDTH;
}
