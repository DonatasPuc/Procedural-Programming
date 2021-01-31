/*  Author: Donatas Pucinskas
 *  Program to solve crosswords given grid and word list.
 *  This implementation uses recursive backtracking.
 *  Program works only partially: there is a logical error in backtracking function, 
 *  so it only sometimes produces correct output.
 */ 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_WORD_COUNT 50
#define MAX_GRID_SIZE 25
#define MAX_WORD_LENGTH 25

#define WORD_FILE_NAME "words3.txt"
#define GRID_FILE_NAME "grid3.txt"
#define REZULT_FILE_NAME "solution.txt"
#define WALL_OUTPUT_SYMBOL '.'
#define WALL_SYMBOL '1'
#define EMPTY_SYMBOL '0'

// Structure for storing information about a word
typedef struct word{
    char str[MAX_WORD_LENGTH];
    int length;
    int used;
} Word;

// Represents an empty slot in a grid
typedef struct slot{
    int row;         // row in which empty slot starts
    int col;         // col in which empty slot starts
    int length;
} Slot;

// Structure for storing information about crossword grid
typedef struct grid{
    char matrix[MAX_GRID_SIZE][MAX_GRID_SIZE];
    int counts[MAX_GRID_SIZE][MAX_GRID_SIZE];
    int row;            // number of rows in a grid
    int col;            // number of collumns in a grid
} Grid;

void printGrid(Grid *grid, FILE *fp);

// Loads word list and grid information from files with given names. Exits program on error
void loadInfo(char grid_file_name[], char word_file_name[], Word words[], int *word_count, Grid *grid) {
    FILE *word_file = fopen(word_file_name, "r");
    FILE *grid_file = fopen(grid_file_name, "r");

    if ((word_file == NULL) || (grid_file == NULL)) {
        printf("File could not be opened. Exiting program");
        exit(0);
    }

    // Read words from file
    while ((*word_count < MAX_WORD_COUNT) && (fscanf(word_file, "%256s", words[*word_count].str) != EOF)) {
        words[*word_count].length = strlen(words[*word_count].str);
        words[*word_count].used = 0;
        ++(*word_count);
    }

    // Read grid from file
    int c = 0;
    grid->row = 0;
    grid->col = 0;
    while ((c = fgetc(grid_file)) != EOF) {
        if (c != '\n') {
            grid->matrix[grid->row][grid->col] = c;
            grid->col += 1;
        } else {
            grid->row += 1;
            grid->col = 0;
        }
    }
    grid->row += 1;

    for (int i = 0; i < grid->row; ++i) {
        for (int j = 0; j < grid->col; ++j) {
            grid->counts[i][j] = 0;
        }
    }

    fclose(word_file);
    fclose(grid_file);
}

// Gets horizontal and vertical empty slots in grid
void getSlots(Grid *g, Slot horizontal[], int *h_count, Slot vertical[], int *v_count) {
    *h_count = 0;
    *v_count = 0;

    // find horizontal slots
    for (int row = 0, size; row < g->row; ++row) {
        for (int col = 0; col < g->col - 1; ++col) {
            // Look for a new slot
            if(g->matrix[row][col] == EMPTY_SYMBOL && g->matrix[row][col+1] == EMPTY_SYMBOL) {
                // Find slot size
                for (size = 2; col + size < g->col && g->matrix[row][col+size] == EMPTY_SYMBOL; ++size)
                    ;
                // Add this slot
                Slot s;
                s.row = row;
                s.col = col;
                s.length = size;
                horizontal[*h_count] = s;
                *h_count += 1;
                // Skip past this slot
                col += size;
            }
        }
    }
    // find vertical slots
    for (int col = 0, size; col < g->col; ++col) {
        for (int row = 0; row < g->row - 1; ++row) {
            // Look for a new slot
            if(g->matrix[row][col] == EMPTY_SYMBOL && g->matrix[row+1][col] == EMPTY_SYMBOL) {
                // Find slot size
                for (size = 2; row + size < g->row && g->matrix[row+size][col] == EMPTY_SYMBOL; ++size)
                    ;
                // Add this slot
                Slot s;
                s.row = row;
                s.col = col;
                s.length = size;
                vertical[*v_count] = s;
                *v_count += 1;
                // Skip past this slot
                row += size;
            }
        }
    }
}

// Fills in word into grid
// Return 0 - word can't be placed, 1 - word was placed
// Dir - direction of word. 0 - horizontal, 1 - vertical
int fill(Grid *g, Slot *s, char word[], int dir) {
    if (dir == 0) {
        // Check if word can be placed
        for (int i = s->col; i < s->col + s->length; ++i) {
            if (g->matrix[s->row][i] != EMPTY_SYMBOL && g->matrix[s->row][i] != word[i - s->col]) {
                return 0;
            }
        }
        // Place word
        for (int i = s->col; i < s->col + s->length; ++i) {
            g->matrix[s->row][i] = word[i - s->col];
            g->counts[s->row][i] += 1;
        }
        return 1;
    } else {
        // Check if word can be placed
        for (int i = s->row; i < s->row + s->length; ++i) {
            if (g->matrix[i][s->col] != EMPTY_SYMBOL && g->matrix[i][s->col] != word[i - s->row]) {
                return 0;
            }
        }
        // Place word
        for (int i = s->row; i < s->row + s->length; ++i) {
            g->matrix[i][s->col] = word[i - s->row];
            g->counts[i][s->col] += 1;
        }
        return 1;
    }
}

// Removes word from grid
// Dir - direction of word. 0 - horizontal, 1 - vertical
void unfill(Grid *g, Slot *s, char word[], int dir) {
    if (dir == 0) {
        // Decrement count
        for (int i = s->col; i < s->col + s->length; ++i) {
            g->counts[s->row][i] -= 1;
        }
        // Remove letters if needed
        for (int i = s->col; i < s->col + s->length; ++i) {
            if (g->counts[s->row][i] == 0) {
                g->matrix[s->row][i] = EMPTY_SYMBOL;
            }
        }
    } else {
        // Check if word can be placed
        for (int i = s->row; i < s->row + s->length; ++i) {
            g->counts[i][s->col] -= 1;
        }
        // Place word
        for (int i = s->row; i < s->row + s->length; ++i) {
            if (g->counts[i][s->col] == 0) {
                g->matrix[i][s->col] = EMPTY_SYMBOL;
            }
        }
    }
}

// Recursive function to generate crossword solution
// Return value: 0 - if solution was not found, 1 - solution was found
int solve(Grid *g, Word words[], int *word_count, Slot horizontal[], int *h_count, Slot vertical[], int *v_count, int slot_index) {
    // All slots are filled
    if (slot_index == (*h_count + *v_count)) {
        return 1;
    }

    // Loop to fill words horizontally
    if (slot_index < *h_count) {
        for (int i = 0; i < *word_count; ++i) {
            if (words[i].used == 0 && words[i].length == horizontal[slot_index].length) {
                if (fill(g, horizontal + slot_index, words[i].str, 0)) {
                    words[i].used = 1;
                    if (solve(g, words, word_count, horizontal, h_count, vertical, v_count, slot_index + 1)) {
                        return 1;
                    }

                    // Backtrack
                    unfill(g, horizontal + slot_index, words[i].str, 0);
                    words[i].used = 0;
                }
            }
        }
        return 0;
    } else {      // Loop to fill words vertically
        int v_slot_index = slot_index - *h_count;     // corrected slot_index
        for (int i = 0; i < *word_count; ++i) {
            if (words[i].used == 0 && words[i].length == vertical[v_slot_index].length) {
                if (fill(g, vertical + v_slot_index, words[i].str, 1)) {
                    words[i].used = 1;
                    if (solve(g, words, word_count, horizontal, h_count, vertical, v_count, slot_index + 1)) {
                        return 1;
                    }

                    // Backtrack
                    unfill(g, vertical + v_slot_index, words[i].str, 1);
                    words[i].used = 0;
                }
            }
        }
        return 0;
    }
    
}

// Prints crossword
void printGrid(Grid *grid, FILE *fp) {
    for (int i = 0; i < grid->row; ++i) {
        for (int j = 0; j < grid->col; ++j) {
            char c = (grid->matrix[i][j] == WALL_SYMBOL) ? WALL_OUTPUT_SYMBOL : grid->matrix[i][j];
            printf("%c", c);
        }
        printf("\n");
    }

    // for (int i = 0; i < grid->row; ++i) {
    //     for (int j = 0; j < grid->col; ++j) {
    //         fputc(grid->matrix[i][j], fp);
    //     }
    //     fputc('\n', fp);
    // }
}

int main(int argc, char *argv[]) {
    int word_count = 0;
    Word words[MAX_WORD_COUNT];
    Grid grid;
    Slot horizontal[MAX_GRID_SIZE * MAX_GRID_SIZE];
    Slot vertical[MAX_GRID_SIZE * MAX_GRID_SIZE];
    int h_count;        // horizontal slot count
    int v_count;        // vertical slot count


    // File names were provided by command line
    if (argc == 3) {
        loadInfo(argv[1], argv[2], words, &word_count, &grid);
    } else { // Use predefined file names
        loadInfo(GRID_FILE_NAME, WORD_FILE_NAME, words, &word_count, &grid);
    }
    getSlots(&grid, horizontal, &h_count, vertical, &v_count);


    if (solve(&grid, words, &word_count, horizontal, &h_count, vertical, &v_count, 0) == 0) {
        printf("No solution");
        return 0;
    }

    FILE *result_file = fopen(REZULT_FILE_NAME, "w");
    if (result_file == 0) {
        printf("File could not be opened. Exiting program");
        return 0;
    }

    printGrid(&grid, result_file);

    // print slot info
    // printf("Horizontal %d:\n", h_count);
    // for (int i = 0; i < h_count; ++i) {
    //     printf("row: %d col: %d length: %d\n", horizontal[i].row, horizontal[i].col, horizontal[i].length);
    // }
    // printf("Vertical %d:\n", v_count);
    // for (int i = 0; i < v_count; ++i) {
    //     printf("row: %d col: %d length: %d\n", vertical[i].row, vertical[i].col, vertical[i].length);
    // }
    // printf("\n");


    fclose(result_file);

    return 0;
}