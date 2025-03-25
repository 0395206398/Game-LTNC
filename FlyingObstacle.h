#ifndef FLYING_OBSTACLE_H
#define FLYING_OBSTACLE_H

#include <SDL.h>

class FlyingObstacle {
public:
    double x, y;
    float speed;
    bool passed;
    SDL_Texture* texture;

    FlyingObstacle(SDL_Texture* tex);
    void update();
    SDL_Rect getRect() const;
    bool isOffScreen() const;
};

#endif // FLYING_OBSTACLE_H
