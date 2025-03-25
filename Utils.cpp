#include "Utils.h"
#include <string>

void drawNumber(SDL_Renderer* renderer, int number, int x, int y, int size) {
    std::string numStr = std::to_string(number);
    int offset = 0;
    const int thickness = size / 10;

    for (char digit : numStr) {
        int digitX = x + offset;
        int digitY = y;
        int w = size / 2;
        int h = size;

        SDL_Rect rect = {digitX, digitY, w, h};

        switch (digit - '0') {
            case 0: SDL_RenderDrawRect(renderer, &rect); break;
            case 1: SDL_RenderDrawLine(renderer, digitX + w/2, digitY, digitX + w/2, digitY + h); break;
            case 2:
                SDL_RenderDrawLine(renderer, digitX, digitY, digitX + w, digitY);
                SDL_RenderDrawLine(renderer, digitX + w, digitY, digitX + w, digitY + h/2);
                SDL_RenderDrawLine(renderer, digitX + w, digitY + h/2, digitX, digitY + h/2);
                SDL_RenderDrawLine(renderer, digitX, digitY + h/2, digitX, digitY + h);
                SDL_RenderDrawLine(renderer, digitX, digitY + h, digitX + w, digitY + h);
                break;
            case 3:
                SDL_RenderDrawLine(renderer, digitX, digitY, digitX + w, digitY);
                SDL_RenderDrawLine(renderer, digitX + w, digitY, digitX + w, digitY + h);
                SDL_RenderDrawLine(renderer, digitX + w, digitY + h/2, digitX, digitY + h/2);
                SDL_RenderDrawLine(renderer, digitX, digitY + h, digitX + w, digitY + h);
                break;
            case 4:
                SDL_RenderDrawLine(renderer, digitX, digitY, digitX, digitY + h/2);
                SDL_RenderDrawLine(renderer, digitX, digitY + h/2, digitX + w, digitY + h/2);
                SDL_RenderDrawLine(renderer, digitX + w, digitY, digitX + w, digitY + h);
                break;
            case 5:
                SDL_RenderDrawLine(renderer, digitX, digitY, digitX + w, digitY);
                SDL_RenderDrawLine(renderer, digitX, digitY, digitX, digitY + h/2);
                SDL_RenderDrawLine(renderer, digitX, digitY + h/2, digitX + w, digitY + h/2);
                SDL_RenderDrawLine(renderer, digitX + w, digitY + h/2, digitX + w, digitY + h);
                SDL_RenderDrawLine(renderer, digitX, digitY + h, digitX + w, digitY + h);
                break;
            case 6:
                SDL_RenderDrawLine(renderer, digitX, digitY, digitX + w, digitY);
                SDL_RenderDrawLine(renderer, digitX, digitY, digitX, digitY + h);
                SDL_RenderDrawLine(renderer, digitX, digitY + h/2, digitX + w, digitY + h/2);
                SDL_RenderDrawLine(renderer, digitX + w, digitY + h/2, digitX + w, digitY + h);
                SDL_RenderDrawLine(renderer, digitX, digitY + h, digitX + w, digitY + h);
                break;
            case 7:
                SDL_RenderDrawLine(renderer, digitX, digitY, digitX + w, digitY);
                SDL_RenderDrawLine(renderer, digitX + w, digitY, digitX + w, digitY + h);
                break;
            case 8:
                SDL_RenderDrawRect(renderer, &rect);
                SDL_RenderDrawLine(renderer, digitX, digitY + h/2, digitX + w, digitY + h/2);
                break;
            case 9:
                SDL_RenderDrawLine(renderer, digitX, digitY, digitX + w, digitY);
                SDL_RenderDrawLine(renderer, digitX, digitY, digitX, digitY + h/2);
                SDL_RenderDrawLine(renderer, digitX, digitY + h/2, digitX + w, digitY + h/2);
                SDL_RenderDrawLine(renderer, digitX + w, digitY, digitX + w, digitY + h);
                break;
        }
        offset += w + 5;
    }
}
