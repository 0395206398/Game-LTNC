#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include "Constants.h"
#include "Dino.h"
#include "Obstacle.h"
#include "FlyingObstacle.h"
#include "SpeedUpItem.h"
#include "SlowDownItem.h"
#include "Utils.h"
#include "ResourceManager.h"
#include "Button.h"

int main(int argc, char* argv[]) {
    // Khởi tạo SDL với video và audio
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    // Khởi tạo SDL Mixer
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cout << "SDL_mixer could not initialize! Mix_Error: " << Mix_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    // Khởi tạo SDL_image
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        std::cout << "SDL_image could not initialize! IMG_Error: " << IMG_GetError() << std::endl;
        Mix_CloseAudio();
        SDL_Quit();
        return 1;
    }

    // Tạo window
    SDL_Window* window = SDL_CreateWindow("Vegito in the Prairie",
                                        SDL_WINDOWPOS_UNDEFINED,
                                        SDL_WINDOWPOS_UNDEFINED,
                                        SCREEN_WIDTH,
                                        SCREEN_HEIGHT,
                                        SDL_WINDOW_SHOWN);
    if (!window) {
        std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        IMG_Quit();
        Mix_CloseAudio();
        SDL_Quit();
        return 1;
    }

    // Tạo renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cout << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        IMG_Quit();
        Mix_CloseAudio();
        SDL_Quit();
        return 1;
    }

    // Khởi tạo ResourceManager
    ResourceManager resources(renderer);

    // Khởi tạo các đối tượng game
    Dino dino(resources.runTexture1, resources.runTexture2, resources.jumpTexture);
    std::vector<Obstacle> obstacles;
    std::vector<FlyingObstacle> flyingObstacles;
    std::vector<SpeedUpItem> speedUpItems;
    std::vector<SlowDownItem> slowDownItems;

    // Khởi tạo các nút
    Button restartButton(resources.restartButtonTexture, SCREEN_WIDTH / 2 - 300, SCREEN_HEIGHT / 2 - 80, 300, 400);
    Button quitButton(resources.quitButtonTexture, SCREEN_WIDTH / 2 , SCREEN_HEIGHT / 2 - 80 , 300, 400);
    Button startButton(resources.startButtonTexture, SCREEN_WIDTH / 2 - 150, SCREEN_HEIGHT / 2-100, 300, 400);  // Thêm nút Start

    // Các biến trạng thái game
    bool quit = false;
    GameState gameState = START;
    SDL_Event e;
    Uint32 lastObstacleTime = 0;
    Uint32 lastItemTime = 0;
    const Uint32 obstacleInterval = 1500;
    const Uint32 itemInterval = 5000;
    int score = 0;
    bool gameOverSoundPlayed = false;

    // Các biến tốc độ
    float baseSpeed = 7.0f;
    float speedMultiplier = 1.0f;
    const float speedIncreaseRate = 0.001f;
    const float maxSpeedMultiplier = 2.0f;

    // Game loop
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
                    dino.reset(resources.runTexture1, resources.runTexture2, resources.jumpTexture);
                    obstacles.clear();
                    flyingObstacles.clear();
                    speedUpItems.clear();
                    slowDownItems.clear();
                    obstacles.push_back(Obstacle(resources.obstacleTexture));
                    gameOverSoundPlayed = false;
                } else if (gameState == PLAYING) {
                    dino.jump();
                    Mix_PlayChannel(-1, resources.jumpSound, 0);  // Phát âm thanh nhảy
                } else if (gameState == GAME_OVER) {
                    gameState = START;
                    gameOverSoundPlayed = false;
                }
            }
            // Xử lý sự kiện nhấn chuột
            if (e.type == SDL_MOUSEBUTTONDOWN) {
                int mouseX = e.button.x;
                int mouseY = e.button.y;
                if (gameState == START && startButton.isClicked(mouseX, mouseY)) {
                    gameState = PLAYING;
                    score = 0;
                    speedMultiplier = 1.0f;
                    dino.reset(resources.runTexture1, resources.runTexture2, resources.jumpTexture);
                    obstacles.clear();
                    flyingObstacles.clear();
                    speedUpItems.clear();
                    slowDownItems.clear();
                    obstacles.push_back(Obstacle(resources.obstacleTexture));
                    gameOverSoundPlayed = false;
                }
                else if (gameState == GAME_OVER) {
                    if (restartButton.isClicked(mouseX, mouseY)) {
                        gameState = START;
                        gameOverSoundPlayed = false;
                    }
                    if (quitButton.isClicked(mouseX, mouseY)) {
                        quit = true;
                    }
                }
            }
        }

        if (gameState == PLAYING) {
            // Tăng tốc độ dần theo thời gian
            speedMultiplier += speedIncreaseRate;
            if (speedMultiplier > maxSpeedMultiplier) {
                speedMultiplier = maxSpeedMultiplier;
            }

            // Cập nhật trạng thái nhân vật
            dino.update();

            // Tính tốc độ tổng dựa trên speedMultiplier và hiệu ứng item
            float totalSpeedMultiplier = speedMultiplier * dino.speedMultiplier;

            // Cập nhật các đối tượng
            for (auto& obstacle : obstacles) {
                obstacle.speed = baseSpeed * totalSpeedMultiplier;
                obstacle.update();
            }
            for (auto& flyingObstacle : flyingObstacles) {
                flyingObstacle.speed = baseSpeed * totalSpeedMultiplier;
                flyingObstacle.update();
            }
            for (auto& item : speedUpItems) {
                item.speed = baseSpeed * totalSpeedMultiplier;
                item.update();
            }
            for (auto& item : slowDownItems) {
                item.speed = baseSpeed * totalSpeedMultiplier;
                item.update();
            }

            // Cộng điểm khi vượt qua chướng ngại vật
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

            // Sinh chướng ngại vật
            Uint32 currentTime = SDL_GetTicks();
            if (currentTime - lastObstacleTime > obstacleInterval) {
                if (obstacles.empty() || obstacles.back().x < SCREEN_WIDTH - 300) {
                    if (rand() % 2 == 0) {
                        obstacles.push_back(Obstacle(resources.obstacleTexture));
                    } else {
                        flyingObstacles.push_back(FlyingObstacle(resources.flyingObstacleTexture));
                    }
                    lastObstacleTime = currentTime;
                }
            }

            // Sinh item
            if (currentTime - lastItemTime > itemInterval) {
                if (score >= 600 && score < 1000) {
                    speedUpItems.push_back(SpeedUpItem(resources.speedUpItemTexture));
                } else if (score >= 1000) {
                    if (rand() % 2 == 0) {
                        speedUpItems.push_back(SpeedUpItem(resources.speedUpItemTexture));
                    } else {
                        slowDownItems.push_back(SlowDownItem(resources.slowDownItemTexture));
                    }
                }
                lastItemTime = currentTime;
            }

            // Xóa các đối tượng ra khỏi màn hình
            obstacles.erase(
                std::remove_if(obstacles.begin(), obstacles.end(),
                    [](const Obstacle& o) { return o.isOffScreen(); }),
                obstacles.end());
            flyingObstacles.erase(
                std::remove_if(flyingObstacles.begin(), flyingObstacles.end(),
                    [](const FlyingObstacle& o) { return o.isOffScreen(); }),
                flyingObstacles.end());
            speedUpItems.erase(
                std::remove_if(speedUpItems.begin(), speedUpItems.end(),
                    [](const SpeedUpItem& i) { return i.isOffScreen(); }),
                speedUpItems.end());
            slowDownItems.erase(
                std::remove_if(slowDownItems.begin(), slowDownItems.end(),
                    [](const SlowDownItem& i) { return i.isOffScreen(); }),
                slowDownItems.end());

            // Kiểm tra va chạm
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

            // Xử lý va chạm với item tăng tốc
            for (auto it = speedUpItems.begin(); it != speedUpItems.end();) {
                SDL_Rect itemRect = it->getRect();
                if (SDL_HasIntersection(&dinoCollisionRect, &itemRect)) {
                    dino.applySpeedEffect(1.5f);
                    it = speedUpItems.erase(it);
                } else {
                    ++it;
                }
            }

            // Xử lý va chạm với item giảm tốc
            for (auto it = slowDownItems.begin(); it != slowDownItems.end();) {
                SDL_Rect itemRect = it->getRect();
                if (SDL_HasIntersection(&dinoCollisionRect, &itemRect)) {
                    dino.applySpeedEffect(0.5f);
                    it = slowDownItems.erase(it);
                } else {
                    ++it;
                }
            }
        }

        // Vẽ màn hình
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        SDL_Rect backgroundRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
        SDL_RenderCopy(renderer, resources.backgroundTexture, NULL, &backgroundRect);

        if (gameState == START) {
            SDL_Rect gameStartRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
            SDL_RenderCopy(renderer, resources.gameStartTexture, NULL, &gameStartRect);
            startButton.render(renderer);  // Vẽ nút Start
        } else if (gameState == PLAYING) {
            SDL_Rect dinoRect = dino.getRect();
            SDL_RenderCopy(renderer, dino.getCurrentTexture(), NULL, &dinoRect);

            for (const auto& obstacle : obstacles) {
                SDL_Rect obstacleRect = obstacle.getRect();
                SDL_RenderCopy(renderer, resources.obstacleTexture, NULL, &obstacleRect);
            }
            for (const auto& flyingObstacle : flyingObstacles) {
                SDL_Rect flyingRect = flyingObstacle.getRect();
                SDL_RenderCopy(renderer, resources.flyingObstacleTexture, NULL, &flyingRect);
            }
            for (const auto& item : speedUpItems) {
                SDL_Rect itemRect = item.getRect();
                SDL_RenderCopy(renderer, resources.speedUpItemTexture, NULL, &itemRect);
            }
            for (const auto& item : slowDownItems) {
                SDL_Rect itemRect = item.getRect();
                SDL_RenderCopy(renderer, resources.slowDownItemTexture, NULL, &itemRect);
            }

            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            drawNumber(renderer, score, 10, 10, 20);
        } else if (gameState == GAME_OVER) {
            SDL_Rect gameOverRect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
            SDL_RenderCopy(renderer, resources.gameOverTexture, NULL, &gameOverRect);

            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            drawNumber(renderer, score, SCREEN_WIDTH / 2 - 20, SCREEN_HEIGHT / 2 - 10, 20);

            restartButton.render(renderer);
            quitButton.render(renderer);

            if (!gameOverSoundPlayed) {
                Mix_PlayChannel(-1, resources.gameOverSound, 0);
                gameOverSoundPlayed = true;
            }
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(16); // Khoảng 60 FPS
    }

    // Giải phóng tài nguyên
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    Mix_CloseAudio();
    IMG_Quit();
    SDL_Quit();

    return 0;
}
