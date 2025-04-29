#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h>
#include <time.h>

typedef struct {
    int **board;
    int **solution;
    int size;
    int mistakes;
    int max_mistakes;
    int empty_cells;
    time_t start_time;
    time_t saved_time;
} GameState;

int **allocate_board(int size);
void free_board(int **board, int size);
void copy_board(int **og_board, int **new_board, int size);
void print_current_board(const GameState *game);
void print_solution(const GameState *game);
void display_play_time(time_t start_time);
int get_difficulty();
int calculate_empty_cells(int size, int difficulty);
void delay(int number_of_seconds);
void save_game(GameState *game);
bool load_game(GameState *game);

#endif // UTILS_H

