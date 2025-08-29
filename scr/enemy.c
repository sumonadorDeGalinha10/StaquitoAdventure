#include "../include/enemy.h"
#include <string.h>

void InitEnemy(Enemy *e, float x, float y) {
    e->rect.x = x;
    e->rect.y = y;
    e->rect.width = 30.0f;
    e->rect.height = 30.0f;
    e->speed = 120.0f;
    e->health = 1;
    e->active = true;
}

void UpdateEnemy(Enemy *e, float dt) {
    if (!e->active) return;
 
    e->rect.y += e->speed * dt;

}

void RenderEnemy(Enemy *e) {
    if (!e->active) return;
    DrawRectangleRec(e->rect, DARKGRAY);
}

bool SpawnEnemy(Enemy list[], int *count, int max, float x, float y) {
    if (*count >= max) return false;
    InitEnemy(&list[*count], x, y);
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
