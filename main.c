#include "raylib.h"
#include <math.h>
#include <stdbool.h>
#include <stdint.h>

enum EntityKind {
  ENEMY,
  PLAYER,
  BALL,
};

typedef struct {
  int32_t x;
  int32_t y;
  uint32_t w;
  uint32_t h;
  int32_t vel_x;
  int32_t vel_y;
  Color color;
  bool alive;
  enum EntityKind kind;
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
#define ENTITY_COUNT (ENEMY_COUNT + 2) // 2 for player and ball

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

bool check_aabb(Entity *a, Entity *b) {
  return (a->x < b->x + b->w && a->x + a->w > b->x && a->y < b->y + b->h && a->y + a->h > b->y);
}

int main(void) {
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "breakout");

  SetTargetFPS(60);

  // 2 for player and ball
  Entity entities[ENTITY_COUNT];

  entities[0] = (Entity){
      .w = PLAYER_WIDTH,
      .h = PLAYER_HEIGTH,
      .x = SCREEN_WIDTH / 2 - 60,
      .y = 1450,
      .color = GREEN,
      .alive = true,
      .kind = PLAYER,
  };

  entities[1] = (Entity){
      .color = WHITE,
      .w = BALL_SIZE,
      .h = BALL_SIZE,
      .x = entities[0].x + (PLAYER_WIDTH / 2) - (BALL_SIZE / 2),
      .y = entities[0].y - BALL_SIZE,
      .vel_x = -300.0f,
      .vel_y = -300.0f,
      .alive = true,
      .kind = BALL,
  };

  Entity *player = &entities[0];
  Entity *ball = &entities[1];

  for (int i = 0; i < ENEMY_COUNT; i++) {
    int row = floor((float)i / ENEMY_PER_ROW) + 1;
    int col = i % ENEMY_PER_ROW + 1;

    int x = (col - 1) * (ENEMY_SIZE + ENEMY_PADDING) + (ENEMY_PADDING / 2);
    int y = (row - 1) * (ENEMY_SIZE + ENEMY_PADDING) + (ENEMY_PADDING / 2);

    entities[i + 2] = (Entity){
        .w = ENEMY_SIZE,
        .h = ENEMY_SIZE,
        .x = x,
        .y = y,
        .color = RED,
        .alive = true,
        .kind = ENEMY,
    };
  }

  while (!WindowShouldClose()) {
    float dt = GetFrameTime();

    BeginDrawing();
    ClearBackground(BLACK);

    for (int i = 0; i < ENTITY_COUNT; i++) {
      Entity *entity = &entities[i];

      switch (entity->kind) {
      case PLAYER:
        if (IsKeyDown(KEY_D)) {
          player->vel_x = PLAYER_VELOCITY;
        } else if (IsKeyDown(KEY_A)) {
          player->vel_x = -PLAYER_VELOCITY;
        } else {
          player->vel_x = 0;
        }
        break;
      case BALL:
        if (entity->x <= 0 || entity->x >= SCREEN_WIDTH - BALL_SIZE) {
          entity->vel_x = -entity->vel_x;
        }

        if (entity->y <= 0 || entity->y >= SCREEN_HEIGHT - BALL_SIZE) {
          entity->vel_y = -entity->vel_y;
        }

        if (check_aabb(player, entity)) {
          entity->vel_y = -entity->vel_y;
        }
        break;
      case ENEMY:
        if (entity->alive) {
          if (check_aabb(ball, entity)) {
            ball->vel_y = -ball->vel_y;
            entity->alive = false;
          }
        }
        break;
      }

      entity->x += entity->vel_x * dt;
      entity->y += entity->vel_y * dt;
      entity->x = MAX(0, entity->x);
      entity->x = MIN(SCREEN_WIDTH - entity->w, entity->x);

      if (entity->alive) {
        DrawRectangle(entity->x, entity->y, entity->w, entity->h, entity->color);
      }
    }

    EndDrawing();
  }

  CloseWindow();

  return 0;
}
