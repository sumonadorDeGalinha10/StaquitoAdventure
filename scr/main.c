#include "raylib.h"
#include "../include/player.h"
#include "../include/powerup.h"

#define MAX_BULLETS 100
#define MAX_ENEMIES 50
#define MAX_POWERUPS 10

int main(void)
{
    const int screenW = 800;
    const int screenH = 600;

    InitWindow(screenW, screenH, "Staquito Adventure");
    SetTargetFPS(60);

    Player player;
    InitPlayer(&player);

    Vector2 bullets[MAX_BULLETS];
    int bulletCount = 0;
    float bulletSpeed = 7.0f;
    PowerUp powerUps[MAX_POWERUPS];
    int powerUpCount = 0;
    float powerUpSpawnTimer = 0;
    const float powerUpSpawnInterval = 5.0f;
    const float powerUpFallSpeed = 120.0f;

    Rectangle enemies[MAX_ENEMIES];
    int enemyCount = 0;
    float enemySpawnTimer = 0;

    while (!WindowShouldClose())
    {

        UpdatePlayer(&player);

        Shoot(&player, bullets, &bulletCount, MAX_BULLETS);

        for (int i = 0; i < bulletCount; i++)
        {
            bullets[i].y -= bulletSpeed;

            if (bullets[i].y < 0)
            {
                for (int j = i; j < bulletCount - 1; j++)
                {
                    bullets[j] = bullets[j + 1];
                }
                bulletCount--;
                i--;
            }
        }

        enemySpawnTimer += GetFrameTime();
        if (enemySpawnTimer >= 2.0f && enemyCount < MAX_ENEMIES)
        {
            enemies[enemyCount] = (Rectangle){
                GetRandomValue(0, 800 - 30),
                -30,
                30,
                30};
            enemyCount++;
            enemySpawnTimer = 0;
        }

        powerUpSpawnTimer += GetFrameTime();
        if (powerUpSpawnTimer >= powerUpSpawnInterval && powerUpCount < MAX_POWERUPS)
        {
            PowerUp tmp;
            Vector2 pos;
            tmp.size = (Vector2){20, 20};
            pos.x = (float)GetRandomValue(0, screenW - (int)tmp.size.x);
            pos.y = -tmp.size.y;
            int type = GetRandomValue(0, 2);
            initPowerUp(&powerUps[powerUpCount], type, pos);
            powerUpCount++;
            powerUpSpawnTimer = 0.0f;
        }

        for (int i = 0; i < enemyCount; i++)
        {
            enemies[i].y += 2.0f;

            if (enemies[i].y > 600)
            {
                for (int j = i; j < enemyCount - 1; j++)
                {
                    enemies[j] = enemies[j + 1];
                }
                enemyCount--;
                i--;
            }
        }

        for (int i = 0; i < bulletCount; i++)
        {
            for (int j = 0; j < enemyCount; j++)
            {
                if (CheckCollisionPointRec(bullets[i], enemies[j]))
                {

                    for (int k = i; k < bulletCount - 1; k++)
                    {
                        bullets[k] = bullets[k + 1];
                    }
                    bulletCount--;
                    i--;

                    for (int k = j; k < enemyCount - 1; k++)
                    {
                        enemies[k] = enemies[k + 1];
                    }
                    enemyCount--;
                    j--;

                    player.score += 10;
                    break;
                }
            }
            if (CheckCollisionPointRec(player.position, enemies[i]))
            {
                player.health -= 10;

                for (int k = i; k < enemyCount - 1; k++)
                {
                    enemies[k] = enemies[k + 1];
                }
                enemyCount--;
                i--;

                if (player.health <= 0)
                {
                    // tenho que adicionar uma tela de game over dps
                }
            }
        }

        for (int i = 0; i < powerUpCount; i++)
        {
            
            if (!powerUps[i].isActive)
                continue;

            
            powerUps[i].position.y += powerUpFallSpeed * GetFrameTime();

            if (powerUps[i].position.y > screenH)
            {
                for (int k = i; k < powerUpCount - 1; k++)
                powerUps[k] = powerUps[k + 1];
                powerUpCount--;
                i--;
                continue;
            }

            Rectangle pRect = {player.position.x, player.position.y, player.size.x, player.size.y};
            Rectangle puRect = {powerUps[i].position.x, powerUps[i].position.y, powerUps[i].size.x, powerUps[i].size.y};

            if (CheckCollisionRecs(pRect, puRect))
            {
    
                switch (powerUps[i].type)
                {
                case 0:
                    player.health += 20;
                    break;
                case 1:
                    player.speed += 2.0f;
                    break;
                case 2:
                    player.score += 50;
                    break;
                }

                for (int k = i; k < powerUpCount - 1; k++)
                powerUps[k] = powerUps[k + 1];
                powerUpCount--;
                i--;
            }
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawPlayer(&player);

        for (int i = 0; i < bulletCount; i++)
        {
            DrawCircleV(bullets[i], 3, RED);
        }

        for (int i = 0; i < enemyCount; i++)
        {
            DrawRectangleRec(enemies[i], DARKGRAY);
        }

        for (int i = 0; i < powerUpCount; i++)
        {
            if (powerUps[i].isActive)
            {
                DrawRectangleV(powerUps[i].position, powerUps[i].size, powerUps[i].color);
            }
        }

        DrawText(TextFormat("Score: %d", player.score), 10, 10, 20, BLACK);
        DrawText(TextFormat("Health: %d", player.health), 10, 40, 20, BLACK);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}