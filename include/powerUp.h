#ifndef POWERUP_H
#define POWERUP_H

#include "raylib.h"


typedef struct PowerUp {
    Vector2 position;
    Vector2 size;
    Color color;
    int type;
    bool isActive;
} PowerUp;

void initPowerUp(PowerUp *powerUp, int type, Vector2 position);

#endif