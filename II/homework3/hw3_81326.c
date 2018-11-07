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

//------------------------------------------------------------------------------
// PRIVATE HELPER FUNCTIONS
//------------------------------------------------------------------------------

int g_conflicts[BOARD_MAX_SIZE];
int totalConflicts = 0;

static int getConflictsOfCell(int arr[], int size, int row, int col) {
    int conflicts = 0;

    for (int x = 0; x < size; x++) {
        if (x == col) {
            continue;
        }

        int y = arr[x];

        if (y == row || abs(y - row) == abs(x - col)) {
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

void calculateConflicts(int arr[], int size) {
    for (int i = 0; i < size; i++) {
        g_conflicts[i] = getConflictsOfCell(arr, size, arr[i], i);
        totalConflicts += g_conflicts[i];
        // printf("%d ", g_conflicts[i]);
    }
    // printf("\n");
}

int getColWithMaxConflicts(int arr[], int size) {

    int colWithMaxConflicts = 0;
    int maxConflicts = 0;

    int candidates[BOARD_MAX_SIZE];
    int candidatesCount = 0;

    for (int col = 0; col < size; col++) {
        int conflicts = g_conflicts[col];

        if (conflicts > maxConflicts) {
            maxConflicts = conflicts;
            colWithMaxConflicts = col;

            candidatesCount = 0;
            candidates[candidatesCount++] = col;
        }
        else if (conflicts == maxConflicts) {
            candidates[candidatesCount++] = col;
        }
    }

    int idx = rand() % candidatesCount;
    return candidates[idx];
}

// Return: the rowIndex where the queen will make minimum conflicts
int getRowWithMinConflicts(int arr[], int size, int col) {

    int rowWithMinConflicts = arr[col];
    int minConflictsCount = 30000;

    int candidates[BOARD_MAX_SIZE];
    int candidatesCount = 0;

    for (int row = 0; row < size; row++) {
        if (row == arr[col]) {
            continue;
        }

        int conflicts = getConflictsOfCell(arr, size, row, col);

        if (conflicts < minConflictsCount) {
            rowWithMinConflicts = row;
            minConflictsCount = conflicts;

            candidatesCount = 0;
            candidates[candidatesCount++] = row;
        }
        else if (conflicts == minConflictsCount) {
            candidates[candidatesCount++] = row;
        }
    }

    int idx = rand() % candidatesCount;
    return candidates[idx];    
}

boolean hasConflicts(int arr[], int size) {
    return totalConflicts > 0;
}

void updateConflicts(int arr[], int size, int newRow, int oldRow, int col) {

    for (int x = 0; x < size; x++) {
        if (x == col) {
            continue;
        }

        int y = arr[x];

        if (y == newRow || abs(y - newRow) == abs(x - col)) {
            g_conflicts[x]++;
            totalConflicts++;
        }

        if (y == oldRow || abs(y - oldRow) == abs(x - col)) {
            g_conflicts[x]--;
            totalConflicts--;
        }
    }

    totalConflicts -= g_conflicts[col];
    g_conflicts[col] = getConflictsOfCell(arr, size, arr[col], col);
    totalConflicts += g_conflicts[col];
}

boolean findSolution(int arr[], int size) {
    const int MAX_ITERATIONS = 3 * size;

    resetBoard(arr, size);
    calculateConflicts(arr, size);

    for (int i = 0; i < MAX_ITERATIONS; i++) {
        int col = getColWithMaxConflicts(arr, size);

        int oldRow = arr[col];
        int newRow = getRowWithMinConflicts(arr, size, col);

        if (oldRow != newRow) {
            arr[col] = newRow;
            updateConflicts(arr, size, arr[col], oldRow, col);
        }

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

    int boardSize = 1000;
    int rowIndex[BOARD_MAX_SIZE];

    solve(rowIndex, boardSize);

    return EXIT_SUCCESS;
}