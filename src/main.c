#include "raylib.h"
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>
#define BOARD_SIZE 8
#define TILE_SIZE 50
#define TILE_TYPES 5

const char tile_chars[TILE_TYPES] = {'@', '$', '&', '#', '%'};
char board[BOARD_SIZE][BOARD_SIZE];

Vector2 grid_origin;

char randomTile()
{
    return tile_chars[rand() % TILE_TYPES];
}

void fill_array()
{
    for (int y = 0; y < BOARD_SIZE; y++)
    {
        for (int x = 0; x < BOARD_SIZE; x++)
        {
            board[x][y] = randomTile();
        }
    }
}

void draw_board()
{
    for (int y = 0; y < BOARD_SIZE; y++)
    {
        for (int x = 0; x < BOARD_SIZE; x++)
        {
            Rectangle rect = {
                4 * TILE_SIZE + (x * TILE_SIZE),
                4 * TILE_SIZE + (y * TILE_SIZE),
                TILE_SIZE,
                TILE_SIZE};
            DrawRectangleLinesEx(rect, 1, DARKGRAY);
            DrawTextEx(
                GetFontDefault(),
                TextFormat("%c", board[x][y]),
                (Vector2){rect.x + 12, rect.y + 8},
                30, 1, LIGHTGRAY);
        }
    }
}

int main()
{
    fill_array();
    const int screen_height = 800;
    const int screen_width = 800;
    InitWindow(screen_height, screen_width, "Match3");
    // srand(time(NULL));

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLACK);
        draw_board();
        EndDrawing();
    }
    CloseWindow();
    return 0;
}