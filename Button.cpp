#include "Button.h"

Button::Button(SDL_Texture* tex, int xPos, int yPos, int w, int h) {
    texture = tex;
    x = xPos;
    y = yPos;
    width = w;
    height = h;
}

bool Button::isClicked(int mouseX, int mouseY) const {
    return (mouseX >= x && mouseX <= x + width && mouseY >= y && mouseY <= y + height);
}

void Button::render(SDL_Renderer* renderer) const {
    SDL_Rect rect = {x, y, width, height};
    SDL_RenderCopy(renderer, texture, nullptr, &rect);
}
