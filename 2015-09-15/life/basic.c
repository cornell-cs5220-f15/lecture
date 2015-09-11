/*
 * basic.c - Basic implementation of the Game of Life
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "life_common.h"


/**
 * For the basic implementation, we keep two boards (previous and current).
 * In each board, we keep a set of "ghost cells" at the boundary to implement
 * wraparound boundary conditions.  Cells are represented by 8-bit chars,
 * one per cell.
 */
typedef struct board_t {
    char* current;  /* Current generation  */
    char* previous; /* Previous generation */
} board_t;


/**
 * Assuming the variable n is set to nboard, this macro accesses the (i,j)
 * entry of either the current or the previous board array.
 */
#define B(which,i,j) which[((i)+1)*(n+2)+((j)+1)]


/**
 * Create the board for the problem
 */
void create_board(problem_t* problem)
{
    int n = problem->nboard;
    board_t* board = (board_t*) malloc(sizeof(board_t));
    board->current = (char*) malloc((n+2) * (n+2));
    board->previous = (char*) malloc((n+2) * (n+2));
    memset(board->current, 0, n*n);
    memset(board->previous, 0, n*n);
    problem->board = board;
}


/**
 * Free the memory associated with a board
 */
void destroy_board(problem_t* problem)
{
    board_t* board = problem->board;
    free(board->current);
    free(board->previous);
    free(board);
    problem->board = NULL;
}


/**
 * Set one cell in the problem
 */
void set_cell(problem_t* problem, int i, int j)
{
    int n = problem->nboard;
    board_t* board = problem->board;
    if (i <= 0 || i >= n)
        return;
    B(board->current,i,j) = 1;
}


/**
 * Get one cell in the problem
 */
char get_cell(problem_t* problem, int i, int j)
{
    int n = problem->nboard;
    board_t* board = problem->board;
    return B(board->current,i,j);
}


/**
 * Advance the board by one generation
 */
void advance_board1(problem_t* problem)
{
    int n = problem->nboard;
    board_t* board = problem->board;
    char* current = board->previous;
    char* previous = board->current;

    for (int j = -1; j <= n; ++j) {
        B(previous, n,j) = B(previous,  0,j);
        B(previous,-1,j) = B(previous,n-1,j);
    }

    for (int i = -1; i <= n; ++i) {
        B(previous,i, n) = B(previous,i,  0);
        B(previous,i,-1) = B(previous,i,n-1);
    }

    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j) {
            int x = 0;
            for (int k = -1; k <= 1; ++k)
                for (int l = -1; l <= 1; ++l)
                    x += B(previous,i+k,j+l);
            x = 2*x-B(previous,i,j);
            B(current,i,j) = (x >= 5 && x <= 7);
        }

    board->current = current;
    board->previous = previous;
}


/**
 * Advance the board by steps generations
 */
void advance_board(problem_t* problem, int steps)
{
    for (int step = 0; step < steps; ++step)
        advance_board1(problem);
}
