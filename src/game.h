#ifndef GAME_H
#define GAME_H

#include "sudoku.h"
#include "utils.h"

void start_new_game();
void init_game(GameState *game, int size, int difficulty);
void play_game(GameState *game);
void make_move(GameState *game);

#endif // GAME_H
