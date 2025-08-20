#include "raylib.h"

int main(void) {
    // Inicializa a janela
    InitWindow(800, 600, "Meu primeiro jogo em C");
    SetTargetFPS(60);

    // Player básico
    Rectangle player = { 400, 500, 50, 50 }; // x, y, width, height
    int speed = 5;

    while (!WindowShouldClose()) {
        // Input
        if (IsKeyDown(KEY_RIGHT)) player.x += speed;
        if (IsKeyDown(KEY_LEFT))  player.x -= speed;

        // Render
        BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawRectangleRec(player, BLUE);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
