#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <SDL.h>

class Obstacle {
public:
    int x, y;
    float speed;
    bool passed;
    SDL_Texture* texture;

    Obstacle(SDL_Texture* tex);
    void update();
    SDL_Rect getRect() const;
    bool isOffScreen() const;
};

#endif // OBSTACLE_H
