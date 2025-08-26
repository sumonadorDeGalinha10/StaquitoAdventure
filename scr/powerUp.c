#include "../include/powerup.h"

void initPowerUp(PowerUp *powerUp, int type, Vector2 position) {
    powerUp->position = position;
    powerUp->size = (Vector2){20, 20};
    powerUp->type = type;
    powerUp->isActive = true;

    switch (type) {
        case 0: powerUp->color = GREEN; break;
        case 1: powerUp->color = BLUE; break;
        case 2: powerUp->color = GOLD; break;
        default: powerUp->color = GRAY; break;
    }
}
