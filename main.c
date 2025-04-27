#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <tgmath.h>
#include <time.h>
#include <string.h>


// TODO make square var a const ?
// TODO get rid of warning flags somehow
// current game status struct
typedef struct
{
    int **board;
    int **solution;
    int size;
    int mistakes;
    int max_mistakes;
    int empty_cells;
    time_t start_time;
    time_t saved_time; // for saving progress
} GameState;

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
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            new_board[i][j] = og_board[i][j];
}

// function for printing current board
void print_board(GameState *game)
{
    int size = game->size;
    int square = (int)sqrt(size); // sudoku square ?? how is it called
    printf("\nMistakes: %d/%d\n", game->mistakes, game->max_mistakes);

    // printing nums of columns
    printf("    ");
    for (int j = 0; j < size; j++)
        printf("%2d ", j + 1);
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
// TODO: make print_solution and print_board one function
void print_solution(GameState *game)
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

// function for checking if a num is not used in row
bool unused_in_row(int **board, int size, int row, int num)
{
    for (int col = 0; col < size; col++)
        if (board[row][col] == num) // if a num is already in a certain place
            return false;
    return true;
}

// function for checking if a num is not used in col
bool unused_in_col(int **board, int size, int col, int num)
{
    for (int row = 0; row < size; row++)
        if (board[row][col] == num)
            return false;
    return true;
}

// function for checking if a num is not used in a square ?? HOW IS IT CALLED
bool unused_in_square(int **board, int row_start, int col_start, int num, int square)
{
    for (int i = 0; i < square; i++)
        for (int j = 0; j < square; j++)
            if (board[row_start + i][col_start + j] == num)
                return false;
    return true;
}

// function for check if we can place a num in a certain place
// (if its empty and unused in row col and square)
bool is_safe(int **board, int size, int row, int col, int num)
{
    int square = (int)sqrt(size);
    return board[row][col] == 0 &&
           unused_in_row(board, size, row, num) &&
           unused_in_col(board, size, col, num) &&
           unused_in_square(board, row - row % square,
                            col - col % square, num, square);
}

// function to fill a square
void fill_box(int **board, int row_start, int col_start, int size)
{
    int square = (int)sqrt(size);
    int num;
    for (int i = 0; i < square; i++)
    {
        for (int j = 0; j < square; j++)
        {
            do
            {
                num = rand() % size + 1;
            } while (!unused_in_square(board, row_start, col_start, num, square));
            board[row_start + i][col_start + j] = num;
        }
    }
}

// fill diagonal boxes
void fill_diagonal(int **board, int size)
{
    int subgrid = (int)sqrt(size);
    for (int i = 0; i < size; i += subgrid)
        fill_box(board, i, i, size);
}

// fill remaining cells
bool fill_remaining(int **board, int size, int row, int col)
{
    if (row >= size)
        return true;

    if (col >= size)
        return fill_remaining(board, size, row + 1, 0);

    if (board[row][col] != 0)
        return fill_remaining(board, size, row, col + 1);

    for (int num = 1; num <= size; num++)
    {
        if (is_safe(board, size, row, col, num))
        {
            board[row][col] = num;
            if (fill_remaining(board, size, row, col + 1))
                return true;
            board[row][col] = 0;
        }
    }
    return false;
}

// remove k digits randomly
void remove_k_digits(int **board, int size, int k)
{
    int count = 0;
    while (count < k)
    {
        int i = rand() % size;
        int j = rand() % size;
        if (board[i][j] != 0)
        {
            board[i][j] = 0;
            count++;
        }
    }
}

// function for generating sudoku board
int **generate_sudoku(int size, int empty_cells)
{
    int **board = allocate_board(size);
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            board[i][j] = 0;

    fill_diagonal(board, size);
    if (!fill_remaining(board, size, 0, 0))
    {
        free_board(board, size);
        return generate_sudoku(size, empty_cells); // try again if generation fails
    }

    // checking whether there is not too many empty spaces
    // (more than spaces even available)
    if (empty_cells > size * size)
        empty_cells = size * size - 1;

    remove_k_digits(board, size, empty_cells);
    return board;
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
    while (clock() < start_time + milli_seconds)
        ;
}


// function for calculating empty cells based on picked difficulty
int calculate_empty_cells(int size, int difficulty)
{
    switch (difficulty)
    {
        case 1: return size * size / 2;      // 50% filled
        case 2: return size * size * 2 / 3;  // 33% filled
        case 3: return size * size * 3 / 4; // 25% filled return size * size / 2;
        case 4:
            //char answer;
            printf("...\n");
            delay(2);
            printf("Aright then...\n");
            delay(2);
            printf("\n\n> FILL THE WHOLE BOARD WITH 0 MISTAKES < \n");
            return size * size -1; // ony one num is visible
        default:
            return size * size / 2; // easy is the default
    }
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
        printf("Something went wrong?\n");
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


// function for player to make a move
void make_move(GameState *game)
{
    int row, col, num;
    char answer[10];

   // clock_t t;
   // t = clock();

    while (true)
    {
        printf("\nOptions:\n-type 'number_of_row number_of_column"
               " number_you_want_to_place' (ex. 1 2 4) with spaces\n"
               "-type 'd row column' to delete a number\n-type 's' to save the current progress "
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
        if (strcmp(answer, "d") == 0)
        {
            scanf("%d %d", &row, &col);
            row--; col--;

            if (row >= 0 && row < game->size && col >= 0 && col < game->size)
            {
                if (game->board[row][col] == 0) {
                    printf("This cell is already empty\n");
                } else {
                    game->board[row][col] = 0;
                    printf("Number from (%d,%d) has been deleted\n", row+1, col+1);
                }
                return;
            }
            else {printf("There is no potition like this\n");}
        }
        else
        {
            // rrying to parse player input as numbers
            row = atoi(answer);
            scanf("%d %d", &col, &num);
            row--; col--;

            if (row >= 0 && row < game->size && col >= 0 && col < game->size &&
                num > 0 && num <= game->size)
            {
                if (game->board[row][col] != 0)
                {
                    printf("This cell is not empty. You can delete it's value by"
                           " typing d row column\n");
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
                        print_board(game);
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

void play_game(GameState *game)
{
    while (true)
    {
        print_board(game);
        make_move(game);
    }
}

void start_new_game()
{
    printf("> NEW GAME\n");
    int size;
    printf("Choose board size (4, 9, 16): \n");
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

int main()
{
    srand(time(NULL)); // ?
    printf("--------------------------------\n");
    printf("\t> TERMINAL SUDOKU <\n");
    int choice = 0;
    // menu
    while (true)
    {
        printf("\nWhat would you like to do? (press number)\n");
        printf("1. New game\n2. Instruction\n3. Load previous game\n4. Quit\n");
        scanf("%d", &choice);

        switch (choice)
        {
            case 1: start_new_game(); break;
            case 2:
                printf("Instructions: press some numbers i guess?? "
               "dont ask me\n");
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
                "Please choose a different option.\n");
                break;
        }
    }
    return 0;
}
