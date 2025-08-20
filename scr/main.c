#include "raylib.h"

int main(void) {

    InitWindow(800, 600, "Meu primeiro jogo em C");
    SetTargetFPS(60);

    Rectangle player = { 400, 500, 50, 50 };
    int speed = 5;

    while (!WindowShouldClose()) {

        if (IsKeyDown(KEY_RIGHT)) player.x += speed;
        if (IsKeyDown(KEY_LEFT))  player.x -= speed;

            BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawRectangleRec(player, BLUE);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
