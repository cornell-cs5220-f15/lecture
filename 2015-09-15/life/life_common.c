#include <stdio.h>
#include <stdlib.h>

#include "crc32.h"
#include "life_common.h"


/**
 * Print a text representation of the current state of the board
 */
void print_board(problem_t* problem)
{
    int n = problem->nboard;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j)
            printf(" %c", get_cell(problem,i,j) ? '*' : '_');
        printf("\n");
    }
}


/**
 * Compute a CRC32 checksum of the board state
 */
uint32_t board_checksum(problem_t* problem)
{
    int n = problem->nboard;
    uint32_t result = 0;
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j) {
            char c = get_cell(problem,i,j);
            result = crc32(result, &c, 1);
        }
    return result;
}


/**
 * Read a file of (i,j) pairs specifying an initial set of live cells
 */
void read_board(problem_t* problem)
{
    FILE* fp = fopen(problem->init, "r");
    int i, j;
    if (fp == NULL) {
        fprintf(stderr, "Could not open board file: %s\n", problem->init);
        exit(-2);
    }
    create_board(problem);
    while (fscanf(fp, "%d %d", &i, &j) == 2)
        set_cell(problem, i, j);
    fclose(fp);
}


/**
 * Print a usage message and quit
 */
void print_usage_quit(const char* name)
{
    fprintf(stderr,
            "Usage: %s [-v] [-f init] [-n boardsize] [-g generations]\n",
            name);
    exit(-1);
}


/**
 * Process command-line options:
 *   -v = verbose output
 *   -f init = use init as file to describe start state
 *   -n size = set the board size (always square with wraparound)
 *   -g step = set the number of generations
 */
void read_options(int argc, char** argv, problem_t* problem)
{
    /* Default options */
    problem->verbose = 0;
    problem->nboard = 100;
    problem->g = 100;
    problem->init = NULL;

    for (int i = 1; i < argc; ++i) {
        if (*argv[i] == '-' && i+1 < argc) {
            switch (argv[i][1]) {
            case 'n':
                problem->nboard = atoi(argv[++i]);
                break;
            case 'g':
                problem->g = atoi(argv[++i]);
                break;
            case 'f':
                problem->init = argv[++i];
                break;
            case 'v':
                problem->verbose = 1;
                break;
            default:
                fprintf(stderr, "Unknown flag: %s", argv[i]);
                print_usage_quit(argv[0]);
            }
        } else {
            print_usage_quit(argv[0]);
        }
    }
    if (problem->init == NULL) {
        fprintf(stderr, "Initialization file not specified\n");
        print_usage_quit(argv[0]);
    }
    read_board(problem);
}


/**
 * Main routine
 */
int main(int argc, char** argv)
{
    problem_t problem;
    read_options(argc, argv, &problem);
    if (problem.verbose) {
        for (int i = 0; i < problem.g; ++i) {
            printf("\nGeneration %d\n", i);
            print_board(&problem);
            advance_board(&problem, 1);
        }
    } else
        advance_board(&problem, problem.g);
    printf("Final checksum: %08X\n", board_checksum(&problem));
    destroy_board(&problem);
    return 0;
}
