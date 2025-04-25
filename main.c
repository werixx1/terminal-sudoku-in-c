#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <tgmath.h>

void print_board(int size, int **board)
{
  printf("\n");
  for (int i = 0; i < size; i++)
    {
    if (i % (int)sqrt(size) == 0 && i != 0)
      printf("-------------------------\n");
    for (int j = 0; j < size; j++) {
      if (j % (int)sqrt(size) == 0 && j != 0)
        printf("| ");
      printf("%2d ", board[i][j]);
    }
    printf("\n");
  }
}
// todo add logic for generating board
// temporary board for testing how it looks printed
void idiot_board(int size, int **board)
{
  for (int i = 0; i < size; i++)
    for (int j = 0; j < size; j++)
      board[i][j] = 1;
}

int **allocate_board(int size)
{
  int **board = malloc(size * sizeof(int *));
  for (int i = 0; i < size; i++)
    board[i] = malloc(size * sizeof(int));
  return board;
}

void free_board(int **board, int size) {
  for (int i = 0; i < size; i++)
    free(board[i]);
  free(board);
}

// GAME LOGIC HERE
void start_new_game()
{
  printf(">NEW GAME\n");
  int size;
  printf("Choose board size (4, 9, 16): \n");
  scanf("%d", &size);
  // todo add input verification
  printf("Selected board size: %dx%d\n", size, size);

  // temporary
  int **board = allocate_board(size);
  idiot_board(size, board);
  print_board(size, board);
  free_board(board, size);
}

int main()
{
  printf("--------------------------------\n");
  printf("\t> TERMINAL SUDOKU <\n");
  int choice = 0;
  // menu
  while (true)
  {
    printf("\nWhat would you like to do? (press number)\n");
    printf("1. New game\n2. Instruction\n3. Quit\n");
    scanf("%d", &choice);

    switch (choice)
    {
      case 1:
        start_new_game();
        break;
      case 2:
        printf("Instructions: press some numbers i guess?? "
               "dont ask me\n");
        break;
      case 3:
        printf("Quitting...");
        exit(0);
      default:
        printf("No such option is available. "
               "Please choose a different option.\n");
        break;
    }
  }
  return 0;
}