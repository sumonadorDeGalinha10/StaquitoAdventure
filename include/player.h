#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"


typedef struct Player {
    Vector2 position;
    Vector2 size;
    Color color;
    float speed;
    float dashSpeed;
    float dashCooldown;
    float dashDuration;
    bool isDashing;
    int health;
    int score;
    int weaponType;
} Player;


void InitPlayer(Player *player);
void UpdatePlayer(Player *player);
void DrawPlayer(Player *player);
void Dash(Player *player);
void Shoot(Player *player, Vector2 *bullets, int *bulletCount, int maxBullets);

#endif