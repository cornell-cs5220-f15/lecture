/*
 * life_common.h - Common header file for Game of Life implementation
 */
#ifndef LIFE_COMMON_H
#define LIFE_COMMON_H

#include <stdint.h>


/**
 * The `board_t` type is a general interface to whatever the
 * implementation chooses to use to represent the board state.
 */
struct board_t;


/**
 * The `problem_t` type contains the problem parameters (specified
 * mostly by command line options).
 */
typedef struct problem_t {
    int verbose;            /* Should we print boards? */
    int nboard;             /* Size of the board */
    int g;                  /* Number of generations */
    const char* init;       /* File with live cell coordinates */
    struct board_t* board;  /* Board data structure */
} problem_t;


/**
 * These interfaces for creating, destroying, advancing, 
 * and accessing the board must be specified by the particular
 * implementation
 */
void create_board(problem_t* problem);
void destroy_board(problem_t* problem);
void advance_board(problem_t* problem, int steps);
void set_cell(problem_t* problem, int i, int j);
char get_cell(problem_t* problem, int i, int j);

#endif /* LIFE_COMMON_H */
