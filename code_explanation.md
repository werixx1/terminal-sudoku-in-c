## allocate_board = creating empty board size x size
```c
int **allocate_board(int size)
{
    int **board = malloc(size * sizeof(int *));
    for (int i = 0; i < size; i++)
        board[i] = malloc(size * sizeof(int));
    return board;
}
```
- ``` int **board = malloc(size * sizeof(int *)); ``` = array of pointers and each pointer is representing a row in board 
- ``` for(...) board[i] = malloc(size * sizeof(int)); ``` = for each pointer (row) is created an array of ints (that will be columns)

## print_current_board = printing readable sudoku board

```c
printf("    ");
    for (int j = 0; j < size; j++) {
        if (j % square == 0 && j != 0)
            printf("  "); 
        printf("%2d ", j + 1);
    }
    printf("\n");

    for (int i = 0; i < size; i++)
        {
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
```
- ```c for(...) j % square == 0 && j != 0 ```= for each square (sqrt of size) there's added a space to divide them
- ```c for(...) if (k % square == 0 && k != 0) ``` = if we 'are' at the square '---' is drawn and '+' where lines meet
- ``` printf("%2d  ", i + 1); ``` = printing num of row and whats inside
- ``` if (game->board[i][j] == 0) printf(" . "); else printf("%2d ", game->board[i][j]); ``` = if we dont have a value in a certain place (0 removed previously with
  remove_k_values) we print '.' and if we do we print that number

## display_play_time = calculate player time
```c
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
```

- ```c time_t end_time; time(&end_time); ``` = creating var that stores current time (time() returns current time)
- ```c double seconds = difftime(end_time, start_time); ``` = difftime calculates the difference between current time and start_time 
  and thats the amount of time the player is in the gamme
- int hours, minutes and seconds are just variables that cast the result as these time units

## make_move (the part where program checks players moves)

(...)
```c 
else
        {
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
                        exit(0);
                    }
                }
                else
                {
                    game->mistakes++;
                    printf("Wrong!:( Mistakes: %d/%d\n", game->mistakes, game->max_mistakes);
                    if (game->mistakes >= game->max_mistakes)
// and the rest
```
- ``` row--; col--; ``` = so row and col that player typed have proper indexes (starting from 0)
- ``` if (row >= 0 && row < game->size && col >= 0 && col < game->size && num > 0 && num <= game->size) ``` = checking if all user input is in bounds
- ``` if (game->board[row][col] != 0) ``` = checking if the place that user wants to put a num is not free (and we print a warning if so)
- ``` if (num == game->solution[row][col]) ``` = if user input is the same as on the solution board (same place, same num) we change the number of empty cells
- ``` if (game->empty_cells == 0) ``` = when theres no empty cells left user wins and whole board is displayed + gameplay time
- ``` else  { game->mistakes++; } ``` = else we count mistakes (until theres more than it's allowed)
