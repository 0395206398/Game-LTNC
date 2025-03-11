#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <string>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 400;
const int DINO_WIDTH = 40;
const int DINO_HEIGHT = 40;
const int OBSTACLE_WIDTH = 20;
const int OBSTACLE_HEIGHT = 40;

void drawNumber(SDL_Renderer* renderer, int number, int x, int y, int size) {
    std::string numStr = std::to_string(number);
    int offset = 0;
    const int thickness = size / 10;

    for (char digit : numStr) {
        int digitX = x + offset;
        int digitY = y;
        int w = size / 2;
        int h = size;

        // Tạo SDL_Rect trước và lấy địa chỉ của nó
        SDL_Rect rect = {digitX, digitY, w, h};

        switch (digit - '0') {
            case 0:
                SDL_RenderDrawRect(renderer, &rect); // Sử dụng địa chỉ của rect
                break;
            case 1:
                SDL_RenderDrawLine(renderer, digitX + w/2, digitY, digitX + w/2, digitY + h);
                break;
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
                SDL_RenderDrawRect(renderer, &rect); // Sử dụng địa chỉ của rect
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

class Dino {
public:
    int x, y;
    int velocity;
    bool isJumping;
    int jumpCount;

    Dino() {
        x = 50;
        y = SCREEN_HEIGHT - DINO_HEIGHT;
        velocity = 0;
        isJumping = false;
        jumpCount = 0;
    }

    void jump() {
        if (jumpCount < 2) {
            velocity = -15;
            isJumping = true;
            jumpCount++;
        }
    }

    void update() {
        y += velocity;
        velocity += 1;

        if (y >= SCREEN_HEIGHT - DINO_HEIGHT) {
            y = SCREEN_HEIGHT - DINO_HEIGHT;
            velocity = 0;
            isJumping = false;
            jumpCount = 0;
        }
    }

    SDL_Rect getRect() {
        return {x, y, DINO_WIDTH, DINO_HEIGHT};
    }
};

class Obstacle {
public:
    int x, y;
    float speed;
    bool passed;

    Obstacle() {
        x = SCREEN_WIDTH;
        y = SCREEN_HEIGHT - OBSTACLE_HEIGHT;
        speed = 7.0f;
        passed = false;
    }

    void update() {
        x -= static_cast<int>(speed);
    }

    SDL_Rect getRect() {
        return {x, y, OBSTACLE_WIDTH, OBSTACLE_HEIGHT};
    }

    bool isOffScreen() {
        return x < -OBSTACLE_WIDTH;
    }
};

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    if (!(IMG_Init(IMG_INIT_JPG) & IMG_INIT_JPG)) {
        std::cout << "SDL_image could not initialize! IMG_Error: " << IMG_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Dino Game",
                                        SDL_WINDOWPOS_UNDEFINED,
                                        SDL_WINDOWPOS_UNDEFINED,
                                        SCREEN_WIDTH,
                                        SCREEN_HEIGHT,
                                        SDL_WINDOW_SHOWN);
    if (!window) return 1;

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) return 1;

    SDL_Surface* gameOverSurface = IMG_Load("gameover.jpg");
    if (!gameOverSurface) return 1;
    SDL_Texture* gameOverTexture = SDL_CreateTextureFromSurface(renderer, gameOverSurface);
    SDL_FreeSurface(gameOverSurface);
    if (!gameOverTexture) return 1;

    Dino dino;
    Obstacle obstacle;
    bool quit = false;
    bool gameOver = false;
    SDL_Event e;
    Uint32 lastObstacleTime = 0;
    const Uint32 obstacleInterval = 1500;
    int score = 0;

    float baseSpeed = 7.0f;
    float speedMultiplier = 1.0f;
    const float speedIncreaseRate = 0.001f;
    const float maxSpeedMultiplier = 2.0f;

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
            if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_SPACE) {
                if (!gameOver) {
                    dino.jump();
                } else {
                    dino = Dino();
                    obstacle = Obstacle();
                    gameOver = false;
                    speedMultiplier = 1.0f;
                    score = 0;
                }
            }
        }

        if (!gameOver) {
            speedMultiplier += speedIncreaseRate;
            if (speedMultiplier > maxSpeedMultiplier) {
                speedMultiplier = maxSpeedMultiplier;
            }
            obstacle.speed = baseSpeed * speedMultiplier;

            dino.update();
            obstacle.update();

            if (obstacle.x + OBSTACLE_WIDTH < dino.x && !obstacle.passed) {
                score += 10;
                obstacle.passed = true;
            }

            Uint32 currentTime = SDL_GetTicks();
            if (currentTime - lastObstacleTime > obstacleInterval && obstacle.isOffScreen()) {
                obstacle = Obstacle();
                obstacle.speed = baseSpeed * speedMultiplier;
                lastObstacleTime = currentTime;
            }

            SDL_Rect dinoRect = dino.getRect();
            SDL_Rect obstacleRect = obstacle.getRect();
            if (SDL_HasIntersection(&dinoRect, &obstacleRect)) {
                gameOver = true;
            }
        }

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        if (!gameOver) {
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_Rect dinoRect = dino.getRect();
            SDL_RenderFillRect(renderer, &dinoRect);

            SDL_Rect obstacleRect = obstacle.getRect();
            SDL_RenderFillRect(renderer, &obstacleRect);

            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            drawNumber(renderer, score, 10, 10, 20);
        } else {
            SDL_Rect gameOverRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
            SDL_RenderCopy(renderer, gameOverTexture, NULL, &gameOverRect);

            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            drawNumber(renderer, score, SCREEN_WIDTH/2 - 20, SCREEN_HEIGHT/2 - 10, 20);
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    SDL_DestroyTexture(gameOverTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();

    return 0;
}
