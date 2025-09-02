#ifndef ENEMY_H
#define ENEMY_H

#include "raylib.h"
#include <stdbool.h>
#include "player.h"  

typedef enum {
    ENEMY_NORMAL,
    ENEMY_ZIGZAG,
    ENEMY_SHOOTING,
    ENEMY_TANK
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

void InitEnemy(Enemy *e, float x, float y, EnemyType type, float speedMultiplier);
void UpdateEnemy(Enemy *e, float dt, Player *player);
void RenderEnemy(Enemy *e);
void RemoveEnemy(Enemy list[], int *count, int index);
#endif
