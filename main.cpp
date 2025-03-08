#include <SDL.h>          // Thư viện SDL
#include <iostream>       // Thư viện nhập/xuất C++

// Định nghĩa hằng số
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 400;
const int DINO_WIDTH = 40;
const int DINO_HEIGHT = 40;
const int OBSTACLE_WIDTH = 20;
const int OBSTACLE_HEIGHT = 40;

// Class Dino
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
        velocity += 1; // Gravity

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

// Class Obstacle
class Obstacle {
public:
    int x, y;
    int speed;

    Obstacle() {
        x = SCREEN_WIDTH;
        y = SCREEN_HEIGHT - OBSTACLE_HEIGHT;
        speed = 7;
    }

    void update() {
        x -= speed;
    }

    SDL_Rect getRect() {
        return {x, y, OBSTACLE_WIDTH, OBSTACLE_HEIGHT};
    }

    bool isOffScreen() {
        return x < -OBSTACLE_WIDTH;
    }
};

// Hàm main
int main(int argc, char* argv[]) {
    // Khởi tạo SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    // Tạo cửa sổ
    SDL_Window* window = SDL_CreateWindow("Dino Game",
                                        SDL_WINDOWPOS_UNDEFINED,
                                        SDL_WINDOWPOS_UNDEFINED,
                                        SCREEN_WIDTH,
                                        SCREEN_HEIGHT,
                                        SDL_WINDOW_SHOWN);
    if (!window) {
        std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    // Tạo renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cout << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    // Game objects
    Dino dino;
    Obstacle obstacle;
    bool quit = false;
    SDL_Event e;
    Uint32 lastObstacleTime = 0;
    const Uint32 obstacleInterval = 2000; // 2 giây

    // Game loop
    while (!quit) {
        // Xử lý sự kiện
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
            if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_SPACE) {
                dino.jump();
            }
        }

        // Cập nhật game
        dino.update();
        obstacle.update();

        // Tạo chướng ngại vật mới
        Uint32 currentTime = SDL_GetTicks();
        if (currentTime - lastObstacleTime > obstacleInterval && obstacle.isOffScreen()) {
            obstacle = Obstacle();
            lastObstacleTime = currentTime;
        }

        // Kiểm tra va chạm
        SDL_Rect dinoRect = dino.getRect();
        SDL_Rect obstacleRect = obstacle.getRect();
        if (SDL_HasIntersection(&dinoRect, &obstacleRect)) {
            std::cout << "Game Over!" << std::endl;
            quit = true;
        }

        // Vẽ
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        // Vẽ khủng long
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderFillRect(renderer, &dinoRect);

        // Vẽ chướng ngại vật
        SDL_RenderFillRect(renderer, &obstacleRect);

        SDL_RenderPresent(renderer);

        // Điều chỉnh FPS
        SDL_Delay(16); // ~60 FPS
    }

    // Giải phóng tài nguyên
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
