#include "../include/enemy.h"
#include <string.h>
#include <math.h>


void InitEnemy(Enemy *e, float x, float y, EnemyType type) {
    e->rect.x = x;
    e->rect.y = y;
    e->rect.width = 30.0f;
    e->rect.height = 30.0f;
    e->speed = 120.0f;
    e->health = 1;
    e->active = true;
    e->type = type;
    e->timeAlive = 0.0f;
    e->baseX = x;

    if (type == ENEMY_ZIGZAG) {
        e->amplitude = 150.0f;
        e->frequency = 5.0f;
    }
}


void UpdateEnemy(Enemy *e, float dt) {
    if (!e->active) return;

    e->timeAlive += dt;

    e->rect.y += e->speed * dt;

    if (e->type == ENEMY_ZIGZAG) {
        e->rect.x = e->baseX + sinf(e->timeAlive * e->frequency) * e->amplitude;
    }
}

void RenderEnemy(Enemy *e) {
    if (!e->active) return;

    Color c = (e->type == ENEMY_NORMAL) ? DARKGRAY : PURPLE;
    DrawRectangleRec(e->rect, c);
}

bool SpawnEnemy(Enemy list[], int *count, int max, float x, float y) {
    if (*count >= max) return false;
    InitEnemy(&list[*count], x, y, (GetRandomValue(0, 1) == 0) ? ENEMY_NORMAL : ENEMY_ZIGZAG);
    (*count)++;
    return true;
}

void RemoveEnemy(Enemy list[], int *count, int index) {
    if (index < 0 || index >= *count) return;


    if (index < *count - 1) {
        memmove(&list[index], &list[index + 1],
                ((*count - index - 1) * sizeof(Enemy)));
    }

    (*count)--;
}
