#ifndef SLOW_DOWN_ITEM_H
#define SLOW_DOWN_ITEM_H

#include <SDL.h>

class SlowDownItem {
public:
    int x, y;
    float speed;
    SDL_Texture* texture;

    SlowDownItem(SDL_Texture* tex);
    void update();
    SDL_Rect getRect() const;
    bool isOffScreen() const;
};

#endif // SLOW_DOWN_ITEM_H
