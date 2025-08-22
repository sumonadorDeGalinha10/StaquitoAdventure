#include "raylib.h"
#include "../include/player.h"


#define MAX_BULLETS 100
#define MAX_ENEMIES 50

int main(void) {

    InitWindow(800, 600, "Staquito Adventure");
    SetTargetFPS(60);

 
    Player player;
    InitPlayer(&player);
    
 
    Vector2 bullets[MAX_BULLETS];
    int bulletCount = 0;
    float bulletSpeed = 7.0f;
    

    Rectangle enemies[MAX_ENEMIES];
    int enemyCount = 0;
    float enemySpawnTimer = 0;

    while (!WindowShouldClose()) {
     
        UpdatePlayer(&player);
        
        Shoot(&player, bullets, &bulletCount, MAX_BULLETS);
        
        for (int i = 0; i < bulletCount; i++) {
            bullets[i].y -= bulletSpeed;
            
            if (bullets[i].y < 0) {
                for (int j = i; j < bulletCount - 1; j++) {
                    bullets[j] = bullets[j + 1];
                }
                bulletCount--;
                i--;
            }
        }
        
        enemySpawnTimer += GetFrameTime();
        if (enemySpawnTimer >= 2.0f && enemyCount < MAX_ENEMIES) {
            enemies[enemyCount] = (Rectangle){
                GetRandomValue(0, 800 - 30),
                -30,
                30,
                30
            };
            enemyCount++;
            enemySpawnTimer = 0;
        }
        
   
        for (int i = 0; i < enemyCount; i++) {
            enemies[i].y += 2.0f;
            
    
            if (enemies[i].y > 600) {
                for (int j = i; j < enemyCount - 1; j++) {
                    enemies[j] = enemies[j + 1];
                }
                enemyCount--;
                i--;
            }
        }
        
     
        for (int i = 0; i < bulletCount; i++) {
            for (int j = 0; j < enemyCount; j++) {
                if (CheckCollisionPointRec(bullets[i], enemies[j])) {
                  
                    for (int k = i; k < bulletCount - 1; k++) {
                        bullets[k] = bullets[k + 1];
                    }
                    bulletCount--;
                    i--;
                    
                    for (int k = j; k < enemyCount - 1; k++) {
                        enemies[k] = enemies[k + 1];
                    }
                    enemyCount--;
                    j--;
                    
                    player.score += 10;
                    break;
                }
            }
        }

   
        BeginDrawing();
            ClearBackground(RAYWHITE);
            
            // Desenha o player
            DrawPlayer(&player);
            
            // Desenha projéteis
            for (int i = 0; i < bulletCount; i++) {
                DrawCircleV(bullets[i], 3, RED);
            }
            
            // Desenha inimigos
            for (int i = 0; i < enemyCount; i++) {
                DrawRectangleRec(enemies[i], DARKGRAY);
            }
            
            // Desenha UI
            DrawText(TextFormat("Score: %d", player.score), 10, 10, 20, BLACK);
            DrawText(TextFormat("Health: %d", player.health), 10, 40, 20, BLACK);
            
        EndDrawing();
    }

    CloseWindow();
    return 0;
}