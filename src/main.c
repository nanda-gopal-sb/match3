#include "raylib.h"
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>
#define BOARD_SIZE 8
#define TILE_SIZE 50
#define TILE_TYPES 5

const char tile_chars[TILE_TYPES] = {'e', 'x', 'g', 'h', 'i'};
char board[BOARD_SIZE][BOARD_SIZE];

Texture2D background;
Font gameFont;
Font scoreFont;

Color line_colour = (Color){255, 100, 50, 100};
Color text_colour = (Color){255, 100, 50, 100};

char randomTile()
{
    return tile_chars[rand() % TILE_TYPES];
}

void loadFonts(Font *gameFont, Font *scoreFont)
{
    *gameFont = LoadFont("./src/resources/GameFont.TTF");
    *scoreFont = LoadFont("./src/resources/scoreFont.ttf");
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
                gameFont,
                TextFormat("%c", board[x][y]),
                (Vector2){rect.x + 8, rect.y + 10},
                35, 1, GRAY);
        }
    }
}

int main()
{
    fill_array();
    const int screen_height = 800;
    const int screen_width = 800;
    InitWindow(screen_height, screen_width, "Match3");

    background = LoadTexture("./src/resources/plen.png");
    srand(time(NULL));
    loadFonts(&gameFont, &scoreFont);
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLACK);
        DrawTexture(background, 0, 0, WHITE);
        draw_board();
        EndDrawing();
    }
    CloseWindow();
    return 0;
}