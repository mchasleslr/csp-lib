#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// kanos27  // Arthur

#include "csp.h"

#define GRID_SIZE 9
#define N_DOMAINS 81
#define N_CONSTRAINTS 3

void print_sudoku(const size_t *values);

bool sudoku_diff(CSPConstraint *, const size_t *, unsigned int *);

bool sudoku_fixed(CSPConstraint *, const size_t *, size_t *);

size_t raw[] = {0, 0, 0, 6, 0, 0, 4, 7, 0, 0, 9, 8, 0, 0, 0, 0, 6, 2, 0, 0, 4,
                0, 1, 0, 5, 0, 0, 9, 0, 0, 3, 0, 0, 6, 0, 0, 4, 2, 0, 0, 0, 8,
                9, 0, 3, 6, 0, 0, 1, 9, 7, 0, 5, 0, 0, 0, 6, 0, 7, 3, 8, 0, 0,
                3, 0, 9, 8, 5, 1, 7, 0, 6, 0, 7, 0, 0, 2, 0, 0, 4, 5};

int main(void) {
  // Initialise the library
  csp_init();
  {
    // Setup
    size_t i;
    size_t num_fixed = 0;
    size_t *grid = calloc(N_DOMAINS, sizeof(size_t));
    size_t *initial_grid = calloc(N_DOMAINS, sizeof(size_t));
    bool *is_fixed = calloc(N_DOMAINS, sizeof(bool));

    // Fill the grid with the loaded sudoku
    for (i = 0; i < 81; i++) {
      if (raw[i]) {
        is_fixed[i] = true;
        initial_grid[i] = raw[i] - 1;
        num_fixed++;
      }
    }

    size_t num_constraints = num_fixed + N_CONSTRAINTS * GRID_SIZE *
                                             (GRID_SIZE * (GRID_SIZE - 1) / 2);

    // Create the CSP problem
    CSPProblem *problem = csp_problem_create(N_DOMAINS, num_constraints);

    // Domains initialization
    for (size_t i = 0; i < N_DOMAINS; i++) {
      csp_problem_set_domain(problem, i, 9);
    }

    // Constraints initialization
    size_t constraint_index = 0;
    for (int i = 0; i < 81; i++) {
      if (is_fixed[i]) {
        CSPConstraint *c = csp_constraint_create(1, (CSPChecker *)sudoku_fixed);
        csp_constraint_set_variable(c, 0, i);
        csp_problem_set_constraint(problem, constraint_index++, c);
      }
    }

    for (int row = 0; row < 9; row++) {
      for (int i = 0; i < 8; i++) {
        for (int j = i + 1; j < 9; j++) {
          CSPConstraint *c =
              csp_constraint_create(2, (CSPChecker *)sudoku_diff);
          csp_constraint_set_variable(c, 0, row * 9 + i);
          csp_constraint_set_variable(c, 1, row * 9 + j);
          csp_problem_set_constraint(problem, constraint_index++, c);
        }
      }
    }

    for (int col = 0; col < 9; col++) {
      for (int i = 0; i < 8; i++) {
        for (int j = i + 1; j < 9; j++) {
          CSPConstraint *c =
              csp_constraint_create(2, (CSPChecker *)sudoku_diff);
          csp_constraint_set_variable(c, 0, i * 9 + col);
          csp_constraint_set_variable(c, 1, j * 9 + col);
          csp_problem_set_constraint(problem, constraint_index++, c);
        }
      }
    }

    for (int block = 0; block < 9; block++) {
      int start_row = (block / 3) * 3;
      int start_col = (block % 3) * 3;
      for (int i = 0; i < 8; i++) {
        for (int j = i + 1; j < 9; j++) {
          CSPConstraint *c =
              csp_constraint_create(2, (CSPChecker *)sudoku_diff);
          int row1 = start_row + (i / 3);
          int col1 = start_col + (i % 3);
          int row2 = start_row + (j / 3);
          int col2 = start_col + (j % 3);
          csp_constraint_set_variable(c, 0, row1 * 9 + col1);
          csp_constraint_set_variable(c, 1, row2 * 9 + col2);
          csp_problem_set_constraint(problem, constraint_index++, c);
        }
      }
    }

    // Solve the CSP problem
    bool result = csp_problem_solve(problem, grid, initial_grid);

    // Destroy the CSP problem
    while (constraint_index--) {
      csp_constraint_destroy(
          csp_problem_get_constraint(problem, constraint_index));
    }
    csp_problem_destroy(problem);

    // Print the solution
    if (result) {
      printf("Solution trouvée\n");
      print_sudoku(grid);
    }

    free(is_fixed);
    free(initial_grid);
    free(grid);
  }
  // Finish the library
  csp_finish();

  return EXIT_SUCCESS;
}

void print_sudoku(const size_t *values) {
  printf("┌");
  for (size_t i = 0; i < GRID_SIZE - 1; i++) {
    printf("───┬");
  }
  printf("───┐\n");
  size_t index = 0;
  for (size_t i = 0; i < GRID_SIZE; i++) {
    printf("│");
    for (size_t j = 0; j < GRID_SIZE; j++) {
      printf(" %ld │", values[i * GRID_SIZE + j] + 1);
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

bool sudoku_diff(CSPConstraint *constraint, const size_t *values,
                 unsigned int *data) {
  // Avoid compiler warnings
  (void)data;

  // Get the variables
  size_t x0 = csp_constraint_get_variable(constraint, 0);
  size_t x1 = csp_constraint_get_variable(constraint, 1);

  // Get the values
  size_t y0 = values[x0];
  size_t y1 = values[x1];

  return y0 != y1;
}

bool sudoku_fixed(CSPConstraint *constraint, const size_t *values,
                  size_t *data) {
  size_t pos = csp_constraint_get_variable(constraint, 0);
  return values[pos] == data[pos];
}
