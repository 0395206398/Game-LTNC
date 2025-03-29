#ifndef CONSTANTS_H
#define CONSTANTS_H

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 450;
const int DINO_WIDTH = 100;
const int DINO_HEIGHT = 100;
const int OBSTACLE_WIDTH = 40;
const int OBSTACLE_HEIGHT = 70;
const int FLYING_OBSTACLE_WIDTH = 64;
const int FLYING_OBSTACLE_HEIGHT = 64;
const int ITEM_WIDTH = 40;
const int ITEM_HEIGHT = 40;
const Uint32 SPEED_EFFECT_DURATION = 5000; // 5 seconds for speed effect
const int GROUND_HEIGHT = 0;
enum GameState {
    START,
    PLAYING,
    GAME_OVER
};

#endif // CONSTANTS_H
