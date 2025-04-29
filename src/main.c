#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <tgmath.h>
#include <time.h>
#include <string.h>
#include "game.h"
#include "sudoku.h"
#include "utils.h"

void show_instructions()
{
    const char* instr = "\t> INSTRUCTIONS: <\n"
                       "Choose new game or load previous one"
                       " (if it was saved).\nAfter starting new game you will choose "
                       "a level\n(amount of visible numbers on the board differ in each)"
                       " and board size.\n"
                       "You can make 3 mistakes (placing numbers in wrong cells)"
                       "\nafter that the game ends.\n"
                       "You can save your game at any moment and come back to it"
                       "\n(but each save rewrites the previous one)\nor quit (without saving)\n"
                       "Good luck!\n";
    printf("%s", instr);
}

int main()
{
    srand(time(NULL)); // ?
    printf("--------------------------------\n");
    printf("\t> TERMINAL SUDOKU <\n");
    int choice = 0;
    char input[256];
    // menu
    while (true)
    {
        printf("\nWhat would you like to do? (press number)\n");
        printf("1. New game\n2. Instruction\n3. Load previous game\n4. Quit\n");

        if (fgets(input, sizeof(input), stdin) == NULL) 
        {
            printf("Error reading input\n");
            continue;
        }
        
        if (sscanf(input, "%d", &choice) != 1) 
        {
            printf("Invalid input. Please enter a number.\n");
            continue;
        }

        switch (choice)
        {
            case 1: start_new_game(); break;
            case 2:
                show_instructions();
                break;
            case 3:
                GameState game;
                if (load_game(&game))
                {
                    printf("\n-- Continuing %dx%d Sudoku --\n", game.size, game.size);
                    play_game(&game);
                    free_board(game.board, game.size);
                    free_board(game.solution, game.size);
                }
                break;
            case 4:
                printf("Quitting...\n");
                exit(0);
            default:
                printf("No such option is available. "
                "Please choose 1-4.\n");
                break;
        }
    }
}
