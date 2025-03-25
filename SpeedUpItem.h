#ifndef SPEED_UP_ITEM_H
#define SPEED_UP_ITEM_H

#include <SDL.h>

class SpeedUpItem {
public:
    int x, y;
    float speed;
    SDL_Texture* texture;

    SpeedUpItem(SDL_Texture* tex);
    void update();
    SDL_Rect getRect() const;
    bool isOffScreen() const;
};

#endif // SPEED_UP_ITEM_H
