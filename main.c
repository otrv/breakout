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
  Rectangle rect;
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
  return (a->rect.x < b->rect.x + b->rect.width && a->rect.x + a->rect.width > b->rect.x &&
          a->rect.y < b->rect.y + b->rect.height && a->rect.y + a->rect.height > b->rect.y);
}

int main(void) {
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "breakout");

  SetTargetFPS(60);

  // 2 for player and ball
  Entity entities[ENTITY_COUNT];

  entities[0] = (Entity){
      .rect =
          (Rectangle){
              .width = PLAYER_WIDTH,
              .height = PLAYER_HEIGTH,
              .x = SCREEN_WIDTH / 2 - 60,
              .y = 1450,
          },
      .color = GREEN,
      .alive = true,
      .kind = PLAYER,
  };

  entities[1] = (Entity){
      .color = WHITE,
      .rect =
          (Rectangle){
              .width = BALL_SIZE,
              .height = BALL_SIZE,
              .x = entities[0].rect.x + (PLAYER_WIDTH / 2) - (BALL_SIZE / 2),
              .y = entities[0].rect.y - BALL_SIZE,
          },
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
        .rect =
            (Rectangle){
                .width = ENEMY_SIZE,
                .height = ENEMY_SIZE,
                .x = x,
                .y = y,
            },
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
        if (entity->rect.x <= 0 || entity->rect.x >= SCREEN_WIDTH - BALL_SIZE) {
          entity->vel_x = -entity->vel_x;
        }

        if (entity->rect.y <= 0 || entity->rect.y >= SCREEN_HEIGHT - BALL_SIZE) {
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

      entity->rect.x += entity->vel_x * dt;
      entity->rect.y += entity->vel_y * dt;
      entity->rect.x = MAX(0, entity->rect.x);
      entity->rect.x = MIN(SCREEN_WIDTH - entity->rect.width, entity->rect.x);

      if (entity->alive) {
        DrawRectangleRec(entity->rect, entity->color);
      }
    }

    EndDrawing();
  }

  CloseWindow();

  return 0;
}
