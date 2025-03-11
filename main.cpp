#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <string>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 400;
const int DINO_WIDTH = 80;
const int DINO_HEIGHT = 80;
const int OBSTACLE_WIDTH = 40;
const int OBSTACLE_HEIGHT = 80;

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
            case 0:
                SDL_RenderDrawRect(renderer, &rect);
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

class Dino {
public:
    int x, y;
    int velocity;
    bool isJumping;
    int jumpCount;
    SDL_Texture* runTexture1;
    SDL_Texture* runTexture2;
    SDL_Texture* jumpTexture;
    Uint32 lastFrameTime;
    bool currentRunFrame;
    const Uint32 frameDelay = 200;

    // Constructor
    Dino(SDL_Texture* tex1, SDL_Texture* tex2, SDL_Texture* jumpTex) {
        x = 50;
        y = SCREEN_HEIGHT - DINO_HEIGHT;
        velocity = 0;
        isJumping = false;
        jumpCount = 0;
        runTexture1 = tex1;
        runTexture2 = tex2;
        jumpTexture = jumpTex;
        lastFrameTime = 0;
        currentRunFrame = false;
    }

    // Disable copy constructor and assignment operator
    Dino(const Dino&) = delete;
    Dino& operator=(const Dino&) = delete;

    // Reset method to reinitialize the Dino object
    void reset(SDL_Texture* tex1, SDL_Texture* tex2, SDL_Texture* jumpTex) {
        x = 50;
        y = SCREEN_HEIGHT - DINO_HEIGHT;
        velocity = 0;
        isJumping = false;
        jumpCount = 0;
        runTexture1 = tex1;
        runTexture2 = tex2;
        jumpTexture = jumpTex;
        lastFrameTime = 0;
        currentRunFrame = false;
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

    SDL_Texture* getCurrentTexture() {
        if (isJumping) {
            return jumpTexture;
        } else {
            Uint32 currentTime = SDL_GetTicks();
            if (currentTime - lastFrameTime >= frameDelay) {
                currentRunFrame = !currentRunFrame;
                lastFrameTime = currentTime;
            }
            return currentRunFrame ? runTexture2 : runTexture1;
        }
    }
};

class Obstacle {
public:
    int x, y;
    float speed;
    bool passed;
    SDL_Texture* texture;

    Obstacle(SDL_Texture* tex) {
        x = SCREEN_WIDTH;
        y = SCREEN_HEIGHT - OBSTACLE_HEIGHT;
        speed = 7.0f;
        passed = false;
        texture = tex;
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

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
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

    // Load game over texture
    SDL_Surface* gameOverSurface = IMG_Load("gameover.jpg");
    if (!gameOverSurface) return 1;
    SDL_Texture* gameOverTexture = SDL_CreateTextureFromSurface(renderer, gameOverSurface);
    SDL_FreeSurface(gameOverSurface);
    if (!gameOverTexture) return 1;

    // Load obstacle texture
    SDL_Surface* obstacleSurface = IMG_Load("13.png");
    if (!obstacleSurface) return 1;
    SDL_Texture* obstacleTexture = SDL_CreateTextureFromSurface(renderer, obstacleSurface);
    SDL_FreeSurface(obstacleSurface);
    if (!obstacleTexture) return 1;

    // Load dino run textures
    SDL_Surface* runSurface1 = IMG_Load("vegitorun1.png");
    if (!runSurface1) return 1;
    SDL_Texture* runTexture1 = SDL_CreateTextureFromSurface(renderer, runSurface1);
    SDL_FreeSurface(runSurface1);
    if (!runTexture1) return 1;

    SDL_Surface* runSurface2 = IMG_Load("vegitorun2.png");
    if (!runSurface2) return 1;
    SDL_Texture* runTexture2 = SDL_CreateTextureFromSurface(renderer, runSurface2);
    SDL_FreeSurface(runSurface2);
    if (!runTexture2) return 1;

    // Load dino jump texture
    SDL_Surface* jumpSurface = IMG_Load("vegitojump.png");
    if (!jumpSurface) return 1;
    SDL_Texture* jumpTexture = SDL_CreateTextureFromSurface(renderer, jumpSurface);
    SDL_FreeSurface(jumpSurface);
    if (!jumpTexture) return 1;

    Dino dino(runTexture1, runTexture2, jumpTexture);
    Obstacle obstacle(obstacleTexture);
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
                    // Reset the dino object instead of reassigning
                    dino.reset(runTexture1, runTexture2, jumpTexture);
                    obstacle = Obstacle(obstacleTexture);
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
                obstacle = Obstacle(obstacleTexture);
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
            SDL_Rect dinoRect = dino.getRect();
            SDL_RenderCopy(renderer, dino.getCurrentTexture(), NULL, &dinoRect);

            SDL_Rect obstacleRect = obstacle.getRect();
            SDL_RenderCopy(renderer, obstacleTexture, NULL, &obstacleRect);

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

    SDL_DestroyTexture(obstacleTexture);
    SDL_DestroyTexture(gameOverTexture);
    SDL_DestroyTexture(runTexture1);
    SDL_DestroyTexture(runTexture2);
    SDL_DestroyTexture(jumpTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();

    return 0;
}
