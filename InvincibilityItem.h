#ifndef INVINCIBILITY_ITEM_H
#define INVINCIBILITY_ITEM_H

#include <SDL.h>

class InvincibilityItem {
public:
    InvincibilityItem(SDL_Texture* texture);
    void update();
    SDL_Rect getRect() const;
    bool isOffScreen() const;

    float x, y;
    float speed;

private:
    SDL_Texture* texture;
    static const int WIDTH = 40;
    static const int HEIGHT = 40;
};

#endif
