#ifndef BUTTON_H
#define BUTTON_H

#include <SDL.h>

class Button {
public:
    int x, y;
    int width, height;
    SDL_Texture* texture;

    Button(SDL_Texture* tex, int xPos, int yPos, int w, int h);
    bool isClicked(int mouseX, int mouseY) const;
    void render(SDL_Renderer* renderer) const;
};

#endif
