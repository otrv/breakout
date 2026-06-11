#include "raylib.h"
#include <math.h>
#include <stdbool.h>
#include <stdint.h>

typedef struct {
  int32_t x;
  int32_t y;
  uint32_t w;
  uint32_t h;
  int32_t vel_x;
  int32_t vel_y;
  Color color;
  bool alive;
} Entity;

#define SCREEN_WIDTH 900
#define SCREEN_HEIGHT 1600
#define PLAYER_VELOCITY 500.0f

#define ENEMY_PADDING 20
#define ENEMY_SIZE 40
#define BALL_SIZE 20
#define PLAYER_WIDTH 120
#define PLAYER_HEIGTH 40

#define ENEMY_PER_ROW 15
#define ENEMY_ROW_COUNT 10
#define ENEMY_COUNT (ENEMY_PER_ROW * ENEMY_ROW_COUNT)

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

bool check_aabb(Entity a, Entity b) {
  return (a.x < b.x + b.w && a.x + a.w > b.x && a.y < b.y + b.h && a.y + a.h > b.y);
}

int main(void) {
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "breakout");

  SetTargetFPS(60);

  Entity player = (Entity){
      .w = PLAYER_WIDTH,
      .h = PLAYER_HEIGTH,
      .x = SCREEN_WIDTH / 2 - 60,
      .y = 1450,
      .color = GREEN,
      .alive = true,
  };

  Entity enemies[ENEMY_COUNT];

  Entity ball = (Entity){
      .color = WHITE,
      .w = BALL_SIZE,
      .h = BALL_SIZE,
      .x = player.x + (PLAYER_WIDTH / 2) - (BALL_SIZE / 2),
      .y = player.y - BALL_SIZE,
      .vel_x = -300.0f,
      .vel_y = -300.0f,
      .alive = true,
  };

  for (int i = 0; i < ENEMY_COUNT; i++) {
    int row = floor((float)i / ENEMY_PER_ROW) + 1;
    int col = i % ENEMY_PER_ROW + 1;

    int x = (col - 1) * (ENEMY_SIZE + ENEMY_PADDING) + (ENEMY_PADDING / 2);
    int y = (row - 1) * (ENEMY_SIZE + ENEMY_PADDING) + (ENEMY_PADDING / 2);

    enemies[i] = (Entity){
        .w = ENEMY_SIZE,
        .h = ENEMY_SIZE,
        .x = x,
        .y = y,
        .color = RED,
        .alive = true,
    };
  }

  while (!WindowShouldClose()) {
    float dt = GetFrameTime();

    if (IsKeyDown(KEY_D)) {
      player.vel_x = PLAYER_VELOCITY;
    } else if (IsKeyDown(KEY_A)) {
      player.vel_x = -PLAYER_VELOCITY;
    } else {
      player.vel_x = 0;
    }

    if (ball.x <= 0 || ball.x >= SCREEN_WIDTH - BALL_SIZE) {
      ball.vel_x = -ball.vel_x;
    }

    if (ball.y <= 0 || ball.y >= SCREEN_HEIGHT - BALL_SIZE) {
      ball.vel_y = -ball.vel_y;
    }

    if (check_aabb(player, ball)) {
      ball.vel_y = -ball.vel_y;
    }

    BeginDrawing();
    ClearBackground(BLACK);

    for (int i = 0; i < ENEMY_COUNT; i++) {
      if (enemies[i].alive) {
        if (check_aabb(ball, enemies[i])) {
          ball.vel_y = -ball.vel_y;
          enemies[i].alive = false;
        }

        DrawRectangle(enemies[i].x, enemies[i].y, enemies[i].w, enemies[i].h, enemies[i].color);
      }
    }

    player.x += player.vel_x * dt;
    ball.x += ball.vel_x * dt;
    ball.y += ball.vel_y * dt;

    player.x = MAX(0, player.x);
    player.x = MIN(SCREEN_WIDTH - PLAYER_WIDTH, player.x);

    DrawRectangle(player.x, player.y, player.w, player.h, player.color);
    DrawRectangle(ball.x, ball.y, ball.w, ball.h, ball.color);
    EndDrawing();
  }

  CloseWindow();

  return 0;
}
