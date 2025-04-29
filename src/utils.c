#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <tgmath.h>
#include <time.h>
#include <string.h>
#include "utils.h"

// allocating memory for board
int **allocate_board(int size)
{
    int **board = malloc(size * sizeof(int *));
    for (int i = 0; i < size; i++)
        board[i] = malloc(size * sizeof(int));
    return board;
}

// freeing board memory
void free_board(int **board, int size)
{
    for (int i = 0; i < size; i++)
        free(board[i]);
    free(board);
}

// function for saving solution before removing numbers from board
void copy_board(int **og_board, int **new_board, int size)
{
    if (og_board == NULL || new_board == NULL || size <= 0) return;
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            new_board[i][j] = og_board[i][j];
}

// function for printing current board
void print_current_board(const GameState *game)
{
    int size = game->size;
    const int square = (int)sqrt(size); // sudoku square ?? how is it called
    printf("\nMistakes: %d/%d\n", game->mistakes, game->max_mistakes);

    // printing nums of columns
    printf("    ");
    for (int j = 0; j < size; j++) {
        if (j % square == 0 && j != 0)
            printf("  "); // cleaner look (spaces between squares)
        printf("%2d ", j + 1);
    }
    printf("\n");

    for (int i = 0; i < size; i++)
        {
        // printing lines between nums for better visibility
        if (i % square == 0 && i != 0)
            {
            printf("    ");
            for (int k = 0; k < size; k++)
                {
                if (k % square == 0 && k != 0)
                    printf("+"); // where lines cross
                printf("---");
            }
            printf("\n");
        }

        // printing row nums and content
        printf("%2d  ", i + 1);
        for (int j = 0; j < size; j++)
            {
            if (j % square == 0 && j != 0)
                printf("| ");
            if (game->board[i][j] == 0)
                printf(" . ");
            else
                printf("%2d ", game->board[i][j]);
            }
        printf("\n");
    }
}

// function for printing solution
void print_solution(const GameState *game)
{
    int size = game->size;
    int square = (int)sqrt(size);
    printf("\nCorrect solution:\n");

    printf("    ");
    for (int j = 0; j < size; j++)
        printf("%2d ", j + 1);
    printf("\n");

    for (int i = 0; i < size; i++)
        {
        if (i % square == 0 && i != 0)
        {
            printf("    ");
            for (int k = 0; k < size; k++)
            {
                if (k % square == 0 && k != 0)
                    printf("+");
                printf("---");
            }
            printf("\n");
        }

        printf("%2d  ", i + 1);
        for (int j = 0; j < size; j++)
        {
            if (j % square == 0 && j != 0)
                printf("| ");
            printf("%2d ", game->solution[i][j]);
        }
        printf("\n");
    }
}

// function for showing how much time it took to finish the game
void display_play_time(time_t start_time)
{
    time_t end_time;
    time(&end_time);
    double seconds = difftime(end_time, start_time);

    int hours = (int)(seconds / 3600);
    int minutes = ((int)seconds % 3600) / 60;
    int secs = (int)seconds % 60;

    printf("\nIt took you ");
    if (hours > 0) // i hope not
        printf("%d hours to play the game", hours);
    if (minutes > 0 && hours < 1)
        printf("%d minutes to play the game", minutes);
    else
        printf("%d seconds to play the game\n", secs);
}

// function for saving game progress
void save_game(GameState *game)
{
    FILE *file = fopen("sudoku_saved_progress.dat", "wb");
    if (file == NULL)
    {
        printf("Something went wrong(?)\n");
        return;
    }

    // saving time played to still track how long it took for a player to
    // finish the game
    time(&game->saved_time);

    // saving current game info
    fwrite(&game->size, sizeof(int), 1, file);
    fwrite(&game->mistakes, sizeof(int), 1, file);
    fwrite(&game->max_mistakes, sizeof(int), 1, file);
    fwrite(&game->empty_cells, sizeof(int), 1, file);
    fwrite(&game->start_time, sizeof(time_t), 1, file);
    fwrite(&game->saved_time, sizeof(time_t), 1, file);

    // saving board and solution
    for (int i = 0; i < game->size; i++)
    {
        fwrite(game->board[i], sizeof(int), game->size, file);
        fwrite(game->solution[i], sizeof(int), game->size, file);
    }

    fclose(file);
    printf("Progress has been saved\n");
}

// function for loading game progress
// (made as a bool so it doesnt return anything weird if the file doesnt exist)
bool load_game(GameState *game)
{
    FILE *file = fopen("sudoku_saved_progress.dat", "rb");
    if (file == NULL)
    {
        printf("No previous saved game found\n");
        return false;
    }

    // read previous progress info
    fread(&game->size, sizeof(int), 1, file);
    fread(&game->mistakes, sizeof(int), 1, file);
    fread(&game->max_mistakes, sizeof(int), 1, file);
    fread(&game->empty_cells, sizeof(int), 1, file);
    fread(&game->start_time, sizeof(time_t), 1, file);
    fread(&game->saved_time, sizeof(time_t), 1, file);

    // allocate memory
    game->board = allocate_board(game->size);
    game->solution = allocate_board(game->size);

    // read board and solution from previous progress
    for (int i = 0; i < game->size; i++)
    {
        fread(game->board[i], sizeof(int), game->size, file);
        fread(game->solution[i], sizeof(int), game->size, file);
    }

    // adjusting time
    time_t current_time;
    time(&current_time);
    double elapsed = difftime(current_time, game->saved_time);
    game->start_time += (time_t)elapsed;

    fclose(file);
    printf("Previous game loaded\n");
    return true;
}


// function for setting diffucuilty level
int get_difficulty()
{
    int level;
    printf("\nChoose difficulty level:\n");
    printf("1. Easy\n");
    printf("2. Medium\n");
    printf("3. Hard\n");
    printf("4. NIGHTMARE (not recommended)\n");
    scanf("%d", &level);

    while (level < 1 || level > 4)
        {
        printf("Such level doesn't exist. Please enter 1-4: ");
        scanf("%d", &level);
    }
    return level;
}

void delay(int number_of_seconds)
{
    int milli_seconds = 1000 * number_of_seconds;
    clock_t start_time = clock();
    while (clock() < start_time + milli_seconds);
}

