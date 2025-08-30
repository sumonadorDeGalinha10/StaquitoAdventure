#ifndef ENEMY_H
#define ENEMY_H

#include "raylib.h"
#include <stdbool.h>

typedef enum {
    ENEMY_NORMAL,
    ENEMY_ZIGZAG
} EnemyType;

typedef struct Enemy {
    Rectangle rect;
    float speed;
    int health;
    bool active;
    EnemyType type;

    float baseX;
    float timeAlive;
    float amplitude;
    float frequency;
} Enemy;

void InitEnemy(Enemy *e, float x, float y, EnemyType type);

void UpdateEnemy(Enemy *e, float dt);

void RenderEnemy(Enemy *e);

bool SpawnEnemy(Enemy list[], int *count, int max, float x, float y);


void RemoveEnemy(Enemy list[], int *count, int index);

#endif
