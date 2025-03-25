#include "FlyingObstacle.h"
#include "Constants.h"

FlyingObstacle::FlyingObstacle(SDL_Texture* tex) {
    x = SCREEN_WIDTH;
    y = SCREEN_HEIGHT / 1.4;
    speed = 7.0f;
    passed = false;
    texture = tex;
}

void FlyingObstacle::update() {
    x -= static_cast<int>(speed);
}

SDL_Rect FlyingObstacle::getRect() const {
    return {static_cast<int>(x), static_cast<int>(y), FLYING_OBSTACLE_WIDTH, FLYING_OBSTACLE_HEIGHT};
}

bool FlyingObstacle::isOffScreen() const {
    return x < -FLYING_OBSTACLE_WIDTH;
}
