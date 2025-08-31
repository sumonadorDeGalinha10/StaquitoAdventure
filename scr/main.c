#include <stdio.h>
#include <string.h>
#include "raylib.h"
#include "../include/player.h"
#include "../include/powerup.h"
#include "../include/enemy.h"

#define MAX_BULLETS 100
#define MAX_ENEMIES 50
#define MAX_POWERUPS 10

typedef enum
{
    STATE_MENU,
    STATE_PLAYING,
    STATE_GAMEOVER
} GameState;

void ResetGame(Player *player,
               Vector2 bullets[], int *bulletCount,
               PowerUp powerUps[], int *powerUpCount, float *powerUpSpawnTimer,
               Enemy enemies[], int *enemyCount, float *enemySpawnTimer)
{
    InitPlayer(player);
    *bulletCount = 0;
    *powerUpCount = 0;
    *powerUpSpawnTimer = 0.0f;
    *enemyCount = 0;
    *enemySpawnTimer = 0.0f;
    // Opcional: zerar arrays (não obrigatório)
    // memset(bullets, 0, sizeof(Vector2)*MAX_BULLETS);
    // memset(powerUps, 0, sizeof(PowerUp)*MAX_POWERUPS);
    // memset(enemies, 0, sizeof(Enemy)*MAX_ENEMIES);
}

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
    float powerUpSpawnTimer = 0.0f;
    const float powerUpSpawnInterval = 5.0f;
    const float powerUpFallSpeed = 120.0f;
    Enemy enemies[MAX_ENEMIES];
    int enemyCount = 0;
    float enemySpawnTimer = 0.0f;
    const float enemySpawnInterval = 2.0f;

    GameState state = STATE_MENU;

    while (!WindowShouldClose())
    {
        float dt = GetFrameTime();

        if (state == STATE_MENU)
        {
            if (IsKeyPressed(KEY_ENTER))
            {
                ResetGame(&player, bullets, &bulletCount, powerUps, &powerUpCount, &powerUpSpawnTimer,
                          enemies, &enemyCount, &enemySpawnTimer);
                state = STATE_PLAYING;
            }
        }

        else if (state == STATE_PLAYING)
        {
            UpdatePlayer(&player);

            Shoot(&player, bullets, &bulletCount, MAX_BULLETS);

            for (int i = 0; i < bulletCount; i++)
            {
                bullets[i].y -= bulletSpeed;
                if (bullets[i].y < 0)
                {
                    for (int j = i; j < bulletCount - 1; j++)
                        bullets[j] = bullets[j + 1];
                    bulletCount--;
                    i--;
                }
            }

            powerUpSpawnTimer += dt;
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

            enemySpawnTimer += dt;
            if (enemySpawnTimer >= enemySpawnInterval && enemyCount < MAX_ENEMIES)
            {
                float x = (float)GetRandomValue(0, screenW - 30);
                EnemyType type = (GetRandomValue(0, 100) < 70) ? ENEMY_NORMAL : ENEMY_ZIGZAG;

                InitEnemy(&enemies[enemyCount], x, -30.0f, type);
                enemyCount++;
                enemySpawnTimer = 0.0f;
            }

            for (int i = 0; i < enemyCount; i++)
            {
                UpdateEnemy(&enemies[i], dt);
                if (enemies[i].rect.y > screenH)
                {
                    RemoveEnemy(enemies, &enemyCount, i);
                    i--;
                    continue;
                }
            }

            for (int b = 0; b < bulletCount; b++)
            {
                bool bulletRemoved = false;
                for (int e = 0; e < enemyCount; e++)
                {
                    if (CheckCollisionPointRec(bullets[b], enemies[e].rect))
                    {
                        for (int k = b; k < bulletCount - 1; k++)
                            bullets[k] = bullets[k + 1];
                        bulletCount--;
                        b--;
                        bulletRemoved = true;

                        RemoveEnemy(enemies, &enemyCount, e);
                        e--;
                        player.score += 10;
                        break;
                    }
                }
                if (bulletRemoved)
                    continue;
            }

            Rectangle pRect = {player.position.x, player.position.y, player.size.x, player.size.y};
            for (int i = 0; i < enemyCount; i++)
            {
                if (CheckCollisionRecs(pRect, enemies[i].rect))
                {
                    player.health -= 10;
                    RemoveEnemy(enemies, &enemyCount, i);
                    i--;
                    if (player.health <= 0)
                    {
                        state = STATE_GAMEOVER;
                        break;
                    }
                }
            }

            for (int i = 0; i < powerUpCount; i++)
            {
                if (!powerUps[i].isActive)
                    continue;
                powerUps[i].position.y += powerUpFallSpeed * dt;

                if (powerUps[i].position.y > screenH)
                {
                    for (int k = i; k < powerUpCount - 1; k++)
                        powerUps[k] = powerUps[k + 1];
                    powerUpCount--;
                    i--;
                    continue;
                }

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
        }
        else if (state == STATE_GAMEOVER)
        {

            if (IsKeyPressed(KEY_R))
            {
                ResetGame(&player, bullets, &bulletCount, powerUps, &powerUpCount, &powerUpSpawnTimer,
                          enemies, &enemyCount, &enemySpawnTimer);
                state = STATE_PLAYING;
            }
            else if (IsKeyPressed(KEY_Q) || IsKeyPressed(KEY_ESCAPE))
            {
                break;
            }
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        if (state == STATE_MENU)
        {
            DrawText("STAQUITO ADVENTURE", screenW / 2 - MeasureText("STAQUITO ADVENTURE", 40) / 2, screenH / 2 - 60, 40, BLUE);
            DrawText("Press ENTER to Start", screenW / 2 - MeasureText("Press ENTER to Start", 20) / 2, screenH / 2, 20, DARKGRAY);
            DrawText("Press ESC to Quit", screenW / 2 - MeasureText("Press ESC to Quit", 20) / 2, screenH / 2 + 30, 20, DARKGRAY);
        }

        else if (state == STATE_PLAYING)
        {

            DrawPlayer(&player);

            for (int i = 0; i < bulletCount; i++)
                DrawCircleV(bullets[i], 3, RED);

            for (int i = 0; i < enemyCount; i++)
                RenderEnemy(&enemies[i]);

            for (int i = 0; i < powerUpCount; i++)
            {
                if (powerUps[i].isActive)
                    DrawRectangleV(powerUps[i].position, powerUps[i].size, powerUps[i].color);
            }

            DrawText(TextFormat("Score: %d", player.score), 10, 10, 20, BLACK);
            DrawText(TextFormat("Health: %d", player.health), 10, 40, 20, BLACK);
            DrawText(TextFormat("Enemies: %d", enemyCount), 10, 70, 20, BLACK);
        }
        else if (state == STATE_GAMEOVER)
        {
            DrawText("GAME OVER", screenW / 2 - MeasureText("GAME OVER", 40) / 2, screenH / 2 - 40, 40, RED);
            DrawText("Press R to Restart", screenW / 2 - MeasureText("Press R to Restart", 20) / 2, screenH / 2 + 10, 20, BLACK);
            DrawText("Press Q or ESC to Quit", screenW / 2 - MeasureText("Press Q or ESC to Quit", 20) / 2, screenH / 2 + 40, 20, DARKGRAY);
            DrawText(TextFormat("Final Score: %d", player.score), screenW / 2 - MeasureText(TextFormat("Final Score: %d", player.score), 20) / 2, screenH / 2 + 80, 20, BLACK);
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
