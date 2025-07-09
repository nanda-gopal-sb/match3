#include "raylib.h"
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define BOARD_SIZE 8
#define TILE_SIZE 50
#define TILE_TYPES 5

const char tile_chars[TILE_TYPES] = { 'a', 'b', 'c', 'd', 'e' };
char board[BOARD_SIZE][BOARD_SIZE];
bool matches[BOARD_SIZE][BOARD_SIZE] = { false };
int score                            = 200;
Vector2 selected                     = { -1, -1 };
Texture2D background;

Font gameFont;
Font scoreFont;

char randomTile () {
    return tile_chars[(rand () % TILE_TYPES)];
}

void loadFonts (Font* gameFont, Font* scoreFont) {
    *gameFont  = LoadFont ("./src/resources/gameFont.otf");
    *scoreFont = LoadFont ("./src/resources/scoreFont.ttf");
}

void fill_array () {
    for (int y = 0; y < BOARD_SIZE; y++) {
        for (int x = 0; x < BOARD_SIZE; x++) {
            board[x][y] = randomTile ();
        }
    }
}

bool find_matches () {
    bool found = false;
    for (int r = 0; r < BOARD_SIZE; r++) {
        for (int c = 0; c < BOARD_SIZE; c++) {
            matches[r][c] = false;
        }
    }
    for (int r = 0; r < BOARD_SIZE; r++) {
        for (int c = 0; c < BOARD_SIZE;) {
            char currentChar       = board[r][c];
            int currentMatchLength = 0;
            for (int k = c; k < BOARD_SIZE; k++) {
                if (board[r][k] == currentChar) {
                    currentMatchLength++;
                } else {
                    break;
                }
            }
            if (currentMatchLength >= 3) {
                found = true;
                if (currentMatchLength == 4) {
                    for (int k = 0; k < 3; k++) {
                        matches[r][c + k] = true;
                    }
                    score += 20;
                } else {
                    for (int k = 0; k < currentMatchLength; k++) {
                        matches[r][c + k] = true;
                    }
                    score += (currentMatchLength / 3) * 20;
                }
            }
            c += (currentMatchLength > 0) ? currentMatchLength : 1;
        }
    }
    for (int c = 0; c < BOARD_SIZE; c++) {
        for (int r = 0; r < BOARD_SIZE;) {
            char currentChar       = board[r][c];
            int currentMatchLength = 0;
            for (int k = r; k < BOARD_SIZE; k++) {
                if (board[k][c] == currentChar) {
                    currentMatchLength++;
                } else {
                    break;
                }
            }
            if (currentMatchLength >= 3) {
                found = true;
                if (currentMatchLength == 4) {
                    for (int k = 0; k < 3; k++) {
                        matches[r + k][c] = true;
                    }
                    score += 20;
                } else {
                    for (int k = 0; k < currentMatchLength; k++) {
                        matches[r + k][c] = true;
                    }
                    score += (currentMatchLength / 3) * 20;
                }
            }
            r += (currentMatchLength > 0) ? currentMatchLength : 1;
        }
    }
    return found;
}

void checkMatches (char charMatrix[BOARD_SIZE][BOARD_SIZE],
bool matches[BOARD_SIZE][BOARD_SIZE]) {
    for (int i = 0; i < BOARD_SIZE; ++i) {
        int count = 1;
        for (int j = 1; j < BOARD_SIZE; ++j) {
            if (charMatrix[i][j] == charMatrix[i][j - 1]) {
                count++;
            } else {
                if (count >= 3) {
                    for (int k = 0; k < count; ++k) {
                        matches[i][j - 1 - k] = false;
                    }
                }
                count = 1;
            }
        }
        if (count >= 3) {
            for (int k = 0; k < count; ++k) {
                matches[i][BOARD_SIZE - 1 - k] = false;
            }
        }
    }
    for (int j = 0; j < BOARD_SIZE; ++j) {
        int count = 1;
        for (int i = 1; i < BOARD_SIZE; ++i) {
            if (charMatrix[i][j] == charMatrix[i - 1][j]) {
                count++;
            } else {
                if (count >= 3) {
                    for (int k = 0; k < count; ++k) {
                        matches[i - 1 - k][j] = false;
                    }
                }
                count = 1;
            }
        }
        if (count >= 3) {
            for (int k = 0; k < count; ++k) {
                matches[BOARD_SIZE - 1 - k][j] = false;
            }
        }
    }
}
void draw_board () {
    for (int y = 0; y < BOARD_SIZE; y++) {
        for (int x = 0; x < BOARD_SIZE; x++) {
            Rectangle rect = { 4 * TILE_SIZE + (x * TILE_SIZE),
                4 * TILE_SIZE + (y * TILE_SIZE), TILE_SIZE, TILE_SIZE };
            DrawRectangleLinesEx (rect, 1, BLACK);
            DrawTextEx (gameFont, TextFormat ("%c", board[x][y]),
            (Vector2){ rect.x + 12, rect.y + 8 }, 35, 1, matches[x][y] ? GREEN : WHITE);
        }
    }
    if (selected.x >= 0) {
        DrawRectangleLinesEx (
        (Rectangle){ (4 * TILE_SIZE) + (selected.x * TILE_SIZE),
        (4 * TILE_SIZE) + (selected.y * TILE_SIZE), TILE_SIZE, TILE_SIZE },
        2, YELLOW);
    }
    DrawTextEx (scoreFont, TextFormat ("Score : %d", score),
    (Vector2){ 20, 20 }, 35, 1, YELLOW);
}

int main () {
    srand (time (NULL));
    fill_array ();
    find_matches ();
    const int screen_height = 800;
    const int screen_width  = 800;
    InitWindow (screen_height, screen_width, "Match3");

    background = LoadTexture ("./src/resources/background.png");
    loadFonts (&gameFont, &scoreFont);
    int i         = 0;
    Vector2 mouse = { 0, 0 };

    while (!WindowShouldClose ()) {
        mouse = GetMousePosition ();

        if (IsMouseButtonPressed (MOUSE_LEFT_BUTTON)) {

            // printf("%f\t%f", mouse.x, mouse.y);
            int x = (mouse.x - (4 * TILE_SIZE)) / TILE_SIZE;
            int y = (mouse.y - (4 * TILE_SIZE)) / TILE_SIZE;
            // if (x >= 0 && x < BOARD_SIZE && y >= 0 && y < BOARD_SIZE)
            {
                // printf("%d\n%d", selected.x, selected.y);
                selected = (Vector2){ x, y };
            }
        }
        BeginDrawing ();
        ClearBackground (BLACK);
        DrawTexture (background, 0, 0, WHITE);
        draw_board ();
        EndDrawing ();
    }
    CloseWindow ();
    return 0;
}