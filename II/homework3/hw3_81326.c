//------------------------------------------------------------------------------
// INCLUDED FILES
//------------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

//------------------------------------------------------------------------------
// TYPE DEFINITIONS
//------------------------------------------------------------------------------

typedef unsigned char boolean;
typedef char          sint8;

//------------------------------------------------------------------------------
// GLOBAL CONSTANTS DEFINITIONS
//------------------------------------------------------------------------------

#ifndef FALSE
#define FALSE (0U)
#endif
#ifndef TRUE
#define TRUE  (1U)
#endif

#define BOARD_MAX_SIZE 32000
// #define DYN_BOARD

//------------------------------------------------------------------------------
// PRIVATE HELPER FUNCTIONS
//------------------------------------------------------------------------------

int g_conflicts[BOARD_MAX_SIZE];

static int getConflictsOfCell(int arr[], int size, int row, int col) {

    int conflicts = 0;
    for (int x = 0; x < size; x++) {
        if (x == col) {
            continue;
        }

        int y = arr[x];

        // Horizontal conflicts
        if (y == row) {
            conflicts++;
        }

        // Main diagonal conflicts
        if (col + row == x + y) {
            conflicts++;
        }

        // Secondary diagonal conflicts
        if (col - row == x - y) {
            conflicts++;
        }
    }

    return conflicts;
}

static void printBoard(int arr[], int size) {
    for (int i = 0; i < size; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (arr[j] != i) {
                printf("_ ");
            }
            else {
                printf("* ");
            }
        }
        printf("\n");
    }

    printf("\n");
}

//------------------------------------------------------------------------------
// PUBLIC FUNCTIONS
//------------------------------------------------------------------------------

void resetBoard(int arr[], int size) {
    for (int i = 0; i < size; i++) {
        arr[i] = i;
    }

    const int MAX_ITERATIONS = 1;

    for (int k = 0; k < MAX_ITERATIONS; k++) {
        for (int i = 0; i < size; i++) {
            int j = rand() % size;
            int temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
        }
    }
}

int getColWithMaxConflicts(int arr[], int size) {

    int colWithMaxConflicts = 0;
    int maxConflictsCount = 0;

    for (int col = 0; col < size; col++) {
        int conflicts = g_conflicts[col];

        if (conflicts > maxConflictsCount) {
            maxConflictsCount = conflicts;
            colWithMaxConflicts = col;
        }
        else if (conflicts == maxConflictsCount) {
            if (rand() % 2) {
                maxConflictsCount = conflicts;
                colWithMaxConflicts = col;
            }
        }
    }

    return colWithMaxConflicts;
}

// Return: the rowIndex where the queen will make minimum conflicts
int getRowWithMinConflicts(int arr[], int size, int col) {

    int rowWithMinConflicts = arr[col];
    int minConflictsCount = 30000;

    for (int row = 0; row < size; row++) {
        if (row == arr[col]) {
            continue;
        }

        int conflicts = getConflictsOfCell(arr, size, row, col);
        if (conflicts < minConflictsCount) {
            rowWithMinConflicts = row;
            minConflictsCount = conflicts;
        }
        else if (conflicts == minConflictsCount) {
            if (rand() % 2) {
                rowWithMinConflicts = row;
                minConflictsCount = conflicts;
            }
        }
    }

    return rowWithMinConflicts;
}

boolean hasConflicts(int arr[], int size) {

    for (int col = 0; col < size; col++) {
        if (g_conflicts[col] > 0) {
            return TRUE;
        }
    }

    return FALSE;
}

void updateConflicts(int arr[], int size, int newRow, int oldRow, int col) {

    for (int i = 0; i < size; i++) {
        if (i == col) {
            continue;
        }

        int x = i;
        int y = arr[i];

        if (y == newRow || 
            x + y == col + newRow ||
            x - y == col - newRow)
        {
            g_conflicts[i]++;
        }

        if (y == oldRow || 
            x + y == col + oldRow ||
            x - y == col - oldRow)
        {
            g_conflicts[i]--;
        }
    }

    g_conflicts[col] = getConflictsOfCell(arr, size, arr[col], col);
}

boolean findSolution(int arr[], int size) {

    const int K = 2;
    const int MAX_ITERATIONS = K * size;

    resetBoard(arr, size);

    // printBoard(arr, size);

    for (int i = 0; i < MAX_ITERATIONS; i++) {
        int col = getColWithMaxConflicts(arr, size);
        int oldRow = arr[col];
        arr[col] = getRowWithMinConflicts(arr, size, col);
        updateConflicts(arr, size, arr[col], oldRow, col);

        if (!hasConflicts(arr, size)) {
            return TRUE;
        }
    }

    return FALSE;
}

void solve(int arr[], int size) {

    const int MAX_RETRIES = 5;

    for (int i = 0; i < MAX_RETRIES; i++) {
        if (findSolution(arr, size)) {
            printf("Success!\n");
            // printBoard(arr, size);
            exit(EXIT_SUCCESS);
        }
    }

    printf("No solution was found!\n");
    exit(EXIT_FAILURE);
}

int main() {

    srand(time(0));

    int n = 10000;

#ifdef DYN_BOARD
    int *rowIndex = malloc(n * sizeof(int));
#else
    int rowIndex[BOARD_MAX_SIZE];
#endif

    solve(rowIndex, n);

    return EXIT_SUCCESS;
}