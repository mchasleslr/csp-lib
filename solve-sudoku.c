#include <stdio.h>
#include <stdlib.h>

kanos27  // Arthur

#include "csp.h"

#define GRID_SIZE 9

    void
    print_sudoku(size_t **values);

size_t **create_grid(size_t *data);

int main(void) {
  size_t data[] = {0, 0, 7, 0, 0, 6, 5, 0, 0, 0, 4, 0, 0, 7, 1, 0, 0,
                   0, 0, 0, 9, 0, 4, 0, 7, 0, 6, 3, 0, 6, 8, 0, 7, 9,
                   0, 2, 0, 0, 0, 0, 5, 0, 0, 0, 0, 2, 0, 0, 1, 0, 4,
                   6, 0, 8, 0, 8, 5, 0, 0, 3, 1, 0, 9, 0, 0, 1, 0, 0,
                   0, 0, 0, 0, 0, 0, 0, 7, 0, 9, 4, 0, 0};

  // Initialise the library
  csp_init();
  {
    // Create the sudoku grid
    size_t **grid = create_grid(data);

    // Create the CSP problem
    CSPProblem *problem = csp_problem_create(GRID_SIZE * GRID_SIZE, 3);

    print_sudoku(grid);
  }
  // Finish the library
  csp_finish();

  return EXIT_SUCCESS;
}

size_t **create_grid(size_t *data) {
  unsigned int i, j, k;

  size_t **grid = calloc(GRID_SIZE, sizeof(size_t *));
  for (i = 0; i < GRID_SIZE; i++) {
    grid[i] = calloc(GRID_SIZE, sizeof(size_t));
  }

  k = 0;
  for (i = 0; i < GRID_SIZE; i++) {
    for (j = 0; j < GRID_SIZE; j++) {
      grid[i][j] = data[k];
      k++;
    }
  }

  return grid;
}

void print_sudoku(size_t **values) {
  printf("╔");
  for (size_t i = 0; i < GRID_SIZE - 1; i++) {
    printf("───┬");
  }
  printf("───┐\n");
  size_t index = 0;
  for (size_t i = 0; i < GRID_SIZE; i++) {
    printf("│");
    for (size_t j = 0; j < GRID_SIZE; j++) {
      if (values[i][j]) {
        printf(" %ld │", values[i][j]);
      } else {
        printf("   │");
      }
      index++;
    }
    if (i != GRID_SIZE - 1) {
      printf("\n├");
      for (size_t i = 0; i < GRID_SIZE - 1; i++) {
        printf("───┼");
      }
      printf("───┤\n");
    }
  }
  printf("\n└");
  for (size_t i = 0; i < GRID_SIZE - 1; i++) {
    printf("───┴");
  }
  printf("───┘\n");
}
