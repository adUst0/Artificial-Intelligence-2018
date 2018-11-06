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
typedef unsigned char uint8;
typedef char sint8;
typedef unsigned short uint16;
typedef short sint16;
typedef unsigned int uint32;
typedef int sint32;
typedef unsigned long long uint64;
typedef long long sint64;

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

static int getHorizontalConflictsOfColumn(int arr[], int size,
    int col) {

    int conflicts = 0;

    for (int i = col - 1; i >= 0; i--) {
        if (arr[i] == arr[col]) {
            ++conflicts;
            break;
        }
    }

    for (int i = col + 1; i < size; i++) {
        if (arr[i] == arr[col]) {
            ++conflicts;
            break;
        }
    }

    return conflicts;
}

static int getMainDiagConflictsOfColumn(int arr[], int size, int col) {

    int conflicts = 0;

    for (int x = col - 1; x >= 0; x--) {
        int y = arr[x];

        if (col - arr[col] == x - y) {
            conflicts++;
            break;
        }
    }

    for (int x = col + 1; x < size; x++) {
        int y = arr[x];

        if (col - arr[col] == x - y) {
            conflicts++;
            break;
        }
    }

    return conflicts;
}

static int getSecDiagConflictsOfColumn(int arr[], int size,
    int col) {

    int conflicts = 0;

    for (int x = col - 1; x >= 0; x--) {
        int y = arr[x];

        if (col + arr[col] == x + y) {
            conflicts++;
            break;
        }
    }

    for (int x = col + 1; x < size; x++) {
        int y = arr[x];

        if (col + arr[col] == x + y) {
            conflicts++;
            break;
        }
    }

    return conflicts;
}

static int getConflictsOfColumn(int arr[], int size, int col) {

    int conflicts = 0;

    conflicts += getHorizontalConflictsOfColumn(arr, size, col);
    conflicts += getMainDiagConflictsOfColumn(arr, size, col);
    conflicts += getSecDiagConflictsOfColumn(arr, size, col);

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

    const int MAX_ITERATIONS = size * 3;

    for (int k = 0; k < MAX_ITERATIONS; k++) {
        for (int i = 0; i < size; i++) {
            arr[i] = rand() % size;
        }
    }
}

int getColWithMaxConflicts(int arr[], int size) {

    int colWithMaxConflicts = 0;
    int maxConflictsCount = 0;

    for (int col = 0; col < size; col++) {
        int conflicts = getConflictsOfColumn(arr, size, col);

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

// Put the queen to every possible row and calculate the conflicts. 
// Restore queen's original position before returning
// Return: the rowIndex with minimum conflicts
int getRowWithMinConflicts(int arr[], int size, int col) {

    // Keep the original position of the queen
    const int queenOriginalRow = arr[col];

    int rowWithMinConflicts = queenOriginalRow;
    int minConflictsCount = 30000;

    for (int i = queenOriginalRow - 1; i >= 0; i--) {
        arr[col] = i;
        int conflicts = getConflictsOfColumn(arr, size, col);
        if (conflicts < minConflictsCount) {
            rowWithMinConflicts = i;
            minConflictsCount = conflicts;
        }
        else if (conflicts == minConflictsCount) {
            if (rand() % 2) {
                rowWithMinConflicts = i;
                minConflictsCount = conflicts;
            }
        }
    }
    arr[col] = queenOriginalRow; // reset the initial value

    for (int i = queenOriginalRow + 1; i < size; i++) {
        arr[col] = i;
        int conflicts = getConflictsOfColumn(arr, size, col);
        if (conflicts < minConflictsCount) {
            rowWithMinConflicts = i;
            minConflictsCount = conflicts;
        }
        else if (conflicts == minConflictsCount) {
            if (rand() % 2) {
                rowWithMinConflicts = i;
                minConflictsCount = conflicts;
            }
        }
    }
    arr[col] = queenOriginalRow; // reset the initial value

    return rowWithMinConflicts;
}

boolean hasConflicts(int arr[], int size) {

    for (int i = 0; i < size; i++) {
        if (getConflictsOfColumn(arr, size, i) > 0) {
            return TRUE;
        }
    }

    return FALSE;
}


boolean findSolution(int arr[], int size) {

    const int K = 5;
    const int MAX_ITERATIONS = K * size;

    resetBoard(arr, size);

    // printBoard(arr, size);

    for (int i = 0; i < MAX_ITERATIONS; i++) {
        int col = getColWithMaxConflicts(arr, size);
        arr[col] = getRowWithMinConflicts(arr, size, col);
        // printBoard(arr, size);

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

    int n = 400;

#ifdef DYN_BOARD
    int *rowIndex = malloc(n * sizeof(int));
#else
    int rowIndex[BOARD_MAX_SIZE];
#endif

    solve(rowIndex, n);

    return EXIT_SUCCESS;
}