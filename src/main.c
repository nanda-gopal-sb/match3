#include "raylib.h"
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define BOARD_SIZE 8
#define TILE_SIZE 50
#define TILE_TYPES 5

typedef enum { STATE_IDLE, STATE_ANIMATING } TileState;

TileState tile_state;

const char tile_chars[TILE_TYPES] = { 'a', 'b', 'c', 'd', 'e' };
char board[BOARD_SIZE][BOARD_SIZE];
bool matches[BOARD_SIZE][BOARD_SIZE]      = { false };
float fall_offset[BOARD_SIZE][BOARD_SIZE] = { 0 };
int score                                 = 200;
Vector2 selected                          = { -1, -1 };
Texture2D background;
float fall_speed = 8.0f;
Font gameFont;
Font scoreFont;

char randomTile () {
    return tile_chars[(rand () % TILE_TYPES)];
}
void swap_tiles (int x1, int y1, int x2, int y2) {

    char temp     = board[x1][y1];
    board[x1][y1] = board[x2][y2];
    board[x2][y2] = temp;
}
bool are_tiles_adjacent (Vector2 a, Vector2 b) {
    return (abs ((int)a.x - (int)b.x) + abs ((int)a.y - (int)b.y)) == 1;
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


void resolve_matches () {
    for (int x = 0; x < BOARD_SIZE; x++) {
        int write_y = BOARD_SIZE - 1;
        for (int y = BOARD_SIZE - 1; y >= 0; y--) {
            if (!matches[x][y]) {
                if (y != write_y) {
                    board[write_y][x]       = board[y][x];
                    fall_offset[write_y][x] = (write_y - y) * TILE_SIZE;
                    board[y][x]             = ' ';
                }
                write_y--;
            }
        }
        while (write_y >= 0) {
            board[write_y][x]       = randomTile ();
            fall_offset[write_y][x] = (write_y + 1) * TILE_SIZE;
            write_y--;
        }
    }
    tile_state = STATE_ANIMATING;
}

void draw_board () {
    for (int y = 0; y < BOARD_SIZE; y++) {
        for (int x = 0; x < BOARD_SIZE; x++) {
            Rectangle rect = { 4 * TILE_SIZE + (x * TILE_SIZE),
                4 * TILE_SIZE + (y * TILE_SIZE), TILE_SIZE, TILE_SIZE };
            DrawRectangleLinesEx (rect, 1, BLACK);
            if (board[y][x] != ' ') {
                DrawTextEx (gameFont, TextFormat ("%c", board[x][y]),
                (Vector2){ rect.x + 12, rect.y + 8 - fall_offset[y][x] }, 35, 1, WHITE);
            }
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

    if (find_matches ()) {
        resolve_matches ();
        score = 0;
    }

    const int screen_height = 800;
    const int screen_width  = 800;
    InitWindow (screen_height, screen_width, "Match3");
    SetTargetFPS (30);

    background = LoadTexture ("./src/resources/background.png");
    loadFonts (&gameFont, &scoreFont);
    int i         = 0;
    Vector2 mouse = { 0, 0 };

    while (!WindowShouldClose ()) {
        mouse = GetMousePosition ();
        if (IsMouseButtonPressed (MOUSE_LEFT_BUTTON)) {
            printf ("");
            int x = (mouse.x - (4 * TILE_SIZE)) / TILE_SIZE;
            int y = (mouse.y - (4 * TILE_SIZE)) / TILE_SIZE;
            if (x >= 0 && x < BOARD_SIZE && y >= 0 && y < BOARD_SIZE) {
                Vector2 current_tile = (Vector2){ x, y };
                if (selected.x < 0) {
                    selected = current_tile;
                } else {
                    if (are_tiles_adjacent (selected, current_tile)) {
                        swap_tiles (
                        selected.x, selected.y, current_tile.x, current_tile.y);
                        if (find_matches ()) {
                            resolve_matches ();
                        } else {
                            swap_tiles (selected.x, selected.y, current_tile.x,
                            current_tile.y);
                        }
                    }
                    selected = (Vector2){ -1, -1 };
                }
            }
        }
        for (int y = 0; y < BOARD_SIZE; y++) {
            for (int x = 0; x < BOARD_SIZE; x++) {
                if (fall_offset[y][x] > 0) {
                    fall_offset[y][x] -= fall_speed;
                    if (fall_offset[y][x] < 0) {
                        fall_offset[y][x] = 0;
                    }
                }
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
