#include "game.h"
#include "sudoku.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <tgmath.h>


void start_new_game()
{
    printf("> NEW GAME\n");
    int size;
    printf("Choose board size (4, 9, 16):\n");
    scanf("%d", &size);

    if (size != 4 && size != 9 && size != 16)
    {
        printf("Invalid size. Using default 9x9.\n");
        size = 9;
    }

    int difficulty = get_difficulty();

    GameState game;
    init_game(&game, size, difficulty);
    printf("\n-- Starting %dx%d Sudoku --\n", size, size);
    play_game(&game);

    free_board(game.board, size);
    free_board(game.solution, size);
}

// initialize game state
void init_game(GameState *game, int size, int difficulty)
{
    game->size = size;
    game->max_mistakes = (difficulty == 4) ? 0 : 3; // for all games except
    // NIGHTMARE player can make 3 mistakes (for nightmare is 0)
    game->mistakes = 0;
    game->empty_cells = calculate_empty_cells(size, difficulty);
    time(&game->start_time); // starting the counter
    game->saved_time = 0;

    game->solution = generate_sudoku(size, 0); // full board
    game->board = allocate_board(size); // board for player
    // copying solution to player board
    copy_board(game->solution, game->board, size);
    // removing some nums for it to be playable
    remove_k_digits(game->board, size, game->empty_cells);
}


void play_game(GameState *game)
{
    while (true)
    {
        print_current_board(game);
        make_move(game);
    }
}

// function for player to make a move
void make_move(GameState *game)
{
    int col, num;
    // clock_t t;
   // t = clock();

    while (true)
    {
        char answer[10];
        printf("\nOptions:\n-type 'number_of_row number_of_column"
               " number_you_want_to_place' (ex. 1 2 4) with spaces\n"
               "-type 's' to save the current progress "
               "(and quit)"
               "\n-type 'q' to quit: \n");
        scanf("%s", answer);

        if (strcmp(answer, "q") == 0)
        {
            exit(0);
        }
        if (strcmp(answer, "s") == 0)
        {
            save_game(game);
            exit(0);
        }
        else
        {
            // rrying to parse player input as numbers
            int row = atoi(answer);
            scanf("%d %d", &col, &num);
            row--; col--;

            if (row >= 0 && row < game->size && col >= 0 && col < game->size &&
                num > 0 && num <= game->size)
            {
                if (game->board[row][col] != 0)
                {
                    printf("This cell is not empty, you can't put a number here\n");
                    continue;
                }

                if (num == game->solution[row][col])
                {
                    game->board[row][col] = num;
                    printf("Correct! Number %d has been placed"
                           " on (%d,%d)", row+1, col+1, num);
                    game->empty_cells--;

                    if (game->empty_cells == 0)
                    {
                        printf("\nCongratulations you solved the game!\n");
                        print_current_board(game);
                        display_play_time(game->start_time);
                        //t = clock() - t;
                        //double time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds
                        //printf("You took %f seconds playing\n", time_taken);
                        exit(0);
                    }
                }
                else
                {
                    game->mistakes++;
                    printf("Wrong!:( Mistakes: %d/%d\n", game->mistakes, game->max_mistakes);

                    if (game->mistakes >= game->max_mistakes)
                    {
                        printf("\nGame over! Too many mistakes\n");
                        print_solution(game);
                        display_play_time(game->start_time);
                       // t = clock() - t;
                        //double time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds
                        //printf("You took %f seconds playing\n", time_taken);
                        exit(0);
                    }
                }
                return;
            }
            else
            {
                printf("Invalid input, please enter row (1-%d), col (1-%d), number (1-%d)\n",
                      game->size, game->size, game->size);
            }
        }
    }
}
