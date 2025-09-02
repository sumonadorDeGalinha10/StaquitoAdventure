// enemy.c - Implementar inimigos com saúde variável e comportamentos diferentes
#include "../include/enemy.h"
#include <string.h>
#include <math.h>
#include "raylib.h"
#include "../include/player.h"

void InitEnemy(Enemy *e, float x, float y, EnemyType type, float speedMultiplier) {
    e->rect.x = x;
    e->rect.y = y;
    e->rect.width = 30.0f;
    e->rect.height = 30.0f;
    e->type = type;
    e->timeAlive = 0.0f;
    e->baseX = x;
    e->active = true;

    switch (type) {
        case ENEMY_NORMAL:
            e->speed = 120.0f * speedMultiplier;
            e->health = 1;
            break;
        case ENEMY_ZIGZAG:
            e->speed = 100.0f * speedMultiplier;
            e->health = 1;
            e->amplitude = 150.0f;
            e->frequency = 5.0f;
            break;
        case ENEMY_SHOOTING:
            e->speed = 80.0f * speedMultiplier;
            e->health = 2;
            break;
        case ENEMY_TANK:
            e->speed = 60.0f * speedMultiplier;
            e->health = 3;
            break;
    }
}

void UpdateEnemy(Enemy *e, float dt, Player *player) {
    if (!e->active) return;

    e->timeAlive += dt;
    e->rect.y += e->speed * dt;

    if (e->type == ENEMY_ZIGZAG) {
        e->rect.x = e->baseX + sinf(e->timeAlive * e->frequency) * e->amplitude;
    }
}

void RenderEnemy(Enemy *e) {
    if (!e->active) return;

    Color color;
    switch (e->type) {
        case ENEMY_NORMAL: color = DARKGRAY; break;
        case ENEMY_ZIGZAG: color = PURPLE; break;
        case ENEMY_SHOOTING: color = RED; break;
        case ENEMY_TANK: color = MAROON; break;
    }
    
    DrawRectangleRec(e->rect, color);
    
    if (e->health > 1) {
        float healthWidth = (e->rect.width * e->health) / 3.0f;
        DrawRectangle(e->rect.x, e->rect.y - 5, healthWidth, 3, GREEN);
    }
}

void RemoveEnemy(Enemy list[], int *count, int index) {
    if (index < 0 || index >= *count) return;

    for (int i = index; i < *count - 1; i++) {
        list[i] = list[i + 1];
    }
    (*count)--;
}