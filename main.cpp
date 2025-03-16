#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 450;
const int DINO_WIDTH = 100;
const int DINO_HEIGHT = 100;
const int OBSTACLE_WIDTH = 40;
const int OBSTACLE_HEIGHT = 80;
const int FLYING_OBSTACLE_WIDTH = 80;
const int FLYING_OBSTACLE_HEIGHT = 80;

enum GameState {
    START,
    PLAYING,
    GAME_OVER
};

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

    Dino(const Dino&) = delete;
    Dino& operator=(const Dino&) = delete;

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
        return {x, y, DINO_WIDTH, DINO_HEIGHT}; // 80x80 for rendering
    }

    SDL_Rect getCollisionRect() const {

        return {x , y, DINO_WIDTH, DINO_HEIGHT}; // 60x80 for collision
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

    SDL_Rect getRect() const {
        return {x, y, OBSTACLE_WIDTH, OBSTACLE_HEIGHT};
    }

    bool isOffScreen() const {
        return x < -OBSTACLE_WIDTH;
    }
};

class FlyingObstacle {
public:
    int x, y;
    float speed;
    bool passed;
    SDL_Texture* texture;

    FlyingObstacle(SDL_Texture* tex) {
        x = SCREEN_WIDTH;
        y = SCREEN_HEIGHT / 2;
        speed = 7.0f;
        passed = false;
        texture = tex;
    }

    void update() {
        x -= static_cast<int>(speed);
    }

    SDL_Rect getRect() const {
        return {x, y, FLYING_OBSTACLE_WIDTH, FLYING_OBSTACLE_HEIGHT};
    }

    bool isOffScreen() const {
        return x < -FLYING_OBSTACLE_WIDTH;
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

    SDL_Window* window = SDL_CreateWindow("Vegito in the Prairie",
                                        SDL_WINDOWPOS_UNDEFINED,
                                        SDL_WINDOWPOS_UNDEFINED,
                                        SCREEN_WIDTH,
                                        SCREEN_HEIGHT,
                                        SDL_WINDOW_SHOWN);
    if (!window) return 1;

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) return 1;

    SDL_Surface* backgroundSurface = IMG_Load("background.png");
    if (!backgroundSurface) {
        std::cout << "Failed to load background.png! IMG_Error: " << IMG_GetError() << std::endl;
        return 1;
    }
    SDL_Texture* backgroundTexture = SDL_CreateTextureFromSurface(renderer, backgroundSurface);
    SDL_FreeSurface(backgroundSurface);
    if (!backgroundTexture) return 1;

    SDL_Surface* gameStartSurface = IMG_Load("gamestart.png");
    if (!gameStartSurface) {
        std::cout << "Failed to load gamestart.png! IMG_Error: " << IMG_GetError() << std::endl;
        return 1;
    }
    SDL_Texture* gameStartTexture = SDL_CreateTextureFromSurface(renderer, gameStartSurface);
    SDL_FreeSurface(gameStartSurface);
    if (!gameStartTexture) return 1;

    SDL_Surface* gameOverSurface = IMG_Load("gameover.jpg");
    if (!gameOverSurface) return 1;
    SDL_Texture* gameOverTexture = SDL_CreateTextureFromSurface(renderer, gameOverSurface);
    SDL_FreeSurface(gameOverSurface);
    if (!gameOverTexture) return 1;

    SDL_Surface* obstacleSurface = IMG_Load("13.png");
    if (!obstacleSurface) return 1;
    SDL_Texture* obstacleTexture = SDL_CreateTextureFromSurface(renderer, obstacleSurface);
    SDL_FreeSurface(obstacleSurface);
    if (!obstacleTexture) return 1;

    SDL_Surface* flyingObstacleSurface = IMG_Load("flying_obstacle.png");
    if (!flyingObstacleSurface) {
        std::cout << "Failed to load flying_obstacle.png! IMG_Error: " << IMG_GetError() << std::endl;
        return 1;
    }
    SDL_Texture* flyingObstacleTexture = SDL_CreateTextureFromSurface(renderer, flyingObstacleSurface);
    SDL_FreeSurface(flyingObstacleSurface);
    if (!flyingObstacleTexture) return 1;

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

    SDL_Surface* jumpSurface = IMG_Load("vegitojump.png");
    if (!jumpSurface) return 1;
    SDL_Texture* jumpTexture = SDL_CreateTextureFromSurface(renderer, jumpSurface);
    SDL_FreeSurface(jumpSurface);
    if (!jumpTexture) return 1;

    Dino dino(runTexture1, runTexture2, jumpTexture);
    std::vector<Obstacle> obstacles;
    std::vector<FlyingObstacle> flyingObstacles;
    bool quit = false;
    GameState gameState = START;
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
                if (gameState == START) {
                    gameState = PLAYING;
                    score = 0;
                    speedMultiplier = 1.0f;
                    dino.reset(runTexture1, runTexture2, jumpTexture);
                    obstacles.clear();
                    flyingObstacles.clear();
                    obstacles.push_back(Obstacle(obstacleTexture));
                } else if (gameState == PLAYING) {
                    dino.jump();
                } else if (gameState == GAME_OVER) {
                    gameState = START;
                }
            }
        }

        if (gameState == PLAYING) {
            speedMultiplier += speedIncreaseRate;
            if (speedMultiplier > maxSpeedMultiplier) {
                speedMultiplier = maxSpeedMultiplier;
            }

            dino.update();

            for (auto& obstacle : obstacles) {
                obstacle.speed = baseSpeed * speedMultiplier;
                obstacle.update();
            }
            for (auto& flyingObstacle : flyingObstacles) {
                flyingObstacle.speed = baseSpeed * speedMultiplier;
                flyingObstacle.update();
            }

            for (auto& obstacle : obstacles) {
                if (obstacle.x + OBSTACLE_WIDTH < dino.x && !obstacle.passed) {
                    score += 10;
                    obstacle.passed = true;
                }
            }
            for (auto& flyingObstacle : flyingObstacles) {
                if (flyingObstacle.x + FLYING_OBSTACLE_WIDTH < dino.x && !flyingObstacle.passed) {
                    score += 10;
                    flyingObstacle.passed = true;
                }
            }

            Uint32 currentTime = SDL_GetTicks();
            if (currentTime - lastObstacleTime > obstacleInterval) {
                if (obstacles.empty() || obstacles.back().x < SCREEN_WIDTH - 300) {
                    if (rand() % 2 == 0) {
                        obstacles.push_back(Obstacle(obstacleTexture));
                    } else {
                        flyingObstacles.push_back(FlyingObstacle(flyingObstacleTexture));
                    }
                    lastObstacleTime = currentTime;
                }
            }

            obstacles.erase(
                std::remove_if(obstacles.begin(), obstacles.end(),
                    [](const Obstacle& o) { return o.isOffScreen(); }),
                obstacles.end());
            flyingObstacles.erase(
                std::remove_if(flyingObstacles.begin(), flyingObstacles.end(),
                    [](const FlyingObstacle& o) { return o.isOffScreen(); }),
                flyingObstacles.end());

            SDL_Rect dinoCollisionRect = dino.getCollisionRect();
            for (const auto& obstacle : obstacles) {
                SDL_Rect obstacleRect = obstacle.getRect();
                if (SDL_HasIntersection(&dinoCollisionRect, &obstacleRect)) {
                    gameState = GAME_OVER;
                }
            }
            for (const auto& flyingObstacle : flyingObstacles) {
                SDL_Rect flyingRect = flyingObstacle.getRect();
                if (SDL_HasIntersection(&dinoCollisionRect, &flyingRect)) {
                    gameState = GAME_OVER;
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        SDL_Rect backgroundRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
        SDL_RenderCopy(renderer, backgroundTexture, NULL, &backgroundRect);

        if (gameState == START) {
            SDL_Rect gameStartRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
            SDL_RenderCopy(renderer, gameStartTexture, NULL, &gameStartRect);
        } else if (gameState == PLAYING) {
            SDL_Rect dinoRect = dino.getRect();
            SDL_RenderCopy(renderer, dino.getCurrentTexture(), NULL, &dinoRect);

            for (const auto& obstacle : obstacles) {
                SDL_Rect obstacleRect = obstacle.getRect();
                SDL_RenderCopy(renderer, obstacleTexture, NULL, &obstacleRect);
            }
            for (const auto& flyingObstacle : flyingObstacles) {
                SDL_Rect flyingRect = flyingObstacle.getRect();
                SDL_RenderCopy(renderer, flyingObstacleTexture, NULL, &flyingRect);
            }

            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            drawNumber(renderer, score, 10, 10, 20);
        } else if (gameState == GAME_OVER) {
            SDL_Rect gameOverRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
            SDL_RenderCopy(renderer, gameOverTexture, NULL, &gameOverRect);

            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            drawNumber(renderer, score, SCREEN_WIDTH/2 - 20, SCREEN_HEIGHT/2 - 10, 20);
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    SDL_DestroyTexture(backgroundTexture);
    SDL_DestroyTexture(gameStartTexture);
    SDL_DestroyTexture(obstacleTexture);
    SDL_DestroyTexture(flyingObstacleTexture);
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
