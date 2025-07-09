#include "raylib.h"
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>

int main()
{

    const int screen_height = 800;
    const int screen_width = 450;
    InitWindow(screen_height, screen_width, "LMAO");
    // srand(time(NULL));

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLUE);
        EndDrawing();
    }
    CloseWindow();
    return 0;
}