#include "../include/player.h"
#include "raylib.h"


void InitPlayer(Player *player)
{
    player->position = (Vector2){400, 500};
    player->size = (Vector2){50, 50};
    player->color = BLUE;
    player->speed = 8.0f;
    player->dashSpeed = 15.0f;
    player->dashCooldown = 0.0f;
    player->dashDuration = 0.0f;
    player->isDashing = false;
    player->health = 100;
    player->score = 0;
    player->weaponType = 0;
}

void UpdatePlayer(Player *player) {
    if (player->dashCooldown > 0) {
        player->dashCooldown -= GetFrameTime();
    }
    
    float currentSpeed=player->speed;

    if (player->isDashing) {
        player->dashDuration -= GetFrameTime();
        if (player->dashDuration <= 0) {
            player->isDashing = false;
        }
      currentSpeed = player->dashSpeed;
    }
    
    if (IsKeyDown(KEY_RIGHT)) player->position.x += currentSpeed;
    if (IsKeyDown(KEY_LEFT)) player->position.x -= currentSpeed;
    

    if (player->position.x < 0) player->position.x = 0;
    if (player->position.x > 800 - player->size.x) player->position.x = 800 - player->size.x;
    
    if (IsKeyPressed(KEY_LEFT_SHIFT) && player->dashCooldown <= 0) {
        Dash(player);
    }
}


void Dash(Player *player) {
    player->isDashing = true;
    player->dashDuration = 0.1f;
    player->dashCooldown = 1.5f;
    

    float dashDirection = 0;
    if (IsKeyDown(KEY_RIGHT)) dashDirection = 1;
    if (IsKeyDown(KEY_LEFT)) dashDirection = -1;
    
    if (dashDirection == 0) dashDirection = 1;
    
    player->position.x += dashDirection * player->dashSpeed * 5;
}

void Shoot(Player *player, Vector2 *bullets, int *bulletCount, int maxBullets) {
    if (IsKeyPressed(KEY_SPACE)) {
        switch (player->weaponType) {
            case 0:
                if (*bulletCount < maxBullets - 1) {
                    bullets[*bulletCount] = (Vector2){player->position.x + player->size.x/2, player->position.y};
                    (*bulletCount)++;
                }
                break;
                
            case 1:
                if (*bulletCount < maxBullets - 2) {
                    bullets[*bulletCount] = (Vector2){player->position.x + player->size.x/4, player->position.y};
                    bullets[*bulletCount + 1] = (Vector2){player->position.x + 3*player->size.x/4, player->position.y};
                    *bulletCount += 2;
                }
                break;
                
        }
    }
}
void DrawPlayer(Player *player) {
 
    Color drawColor = player->color;
    if (player->isDashing) {
        drawColor = (Color){drawColor.r, drawColor.g, drawColor.b, 180};
    }
    
    DrawRectangleV(player->position, player->size, drawColor);
    
    if (player->dashCooldown > 0) {
        float cooldownPercent = 1.0f - (player->dashCooldown / 1.0f);
        DrawRectangle((int)player->position.x, (int)player->position.y - 10, 
                     (int)(player->size.x * cooldownPercent), 5, GREEN);
    }
}