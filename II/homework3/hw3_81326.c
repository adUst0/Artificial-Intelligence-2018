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

typedef unsigned char      boolean;
typedef char               sint8;
typedef unsigned char      uint8;
typedef short              sint16;
typedef unsigned short     uint16;
typedef int                sint32;
typedef unsigned int       uint32;
typedef long long          sint64;
typedef unsigned long long uint64;

//------------------------------------------------------------------------------
// GLOBAL CONSTANTS DEFINITIONS
//------------------------------------------------------------------------------

#ifndef FALSE
#define FALSE (0U)
#endif
#ifndef TRUE
#define TRUE  (1U)
#endif

#define BOARD_MAX_SIZE 12000

//------------------------------------------------------------------------------
// PRIVATE HELPER FUNCTIONS
//------------------------------------------------------------------------------

uint16 g_conflicts[BOARD_MAX_SIZE];
uint16 totalConflicts = 0;

static uint16 getConflictsOfCell(uint16 arr[], uint16 size, uint16 row, uint16 col) {
    uint16 conflicts = 0;

    for (uint16 x = 0; x < size; x++) {
        if (x == col) {
            continue;
        }

        uint16 y = arr[x];

        if (y == row || abs(y - row) == abs(x - col)) {
            conflicts++;
        }
    }

    return conflicts;
}

static void printBoard(uint16 arr[], uint16 size) {
    for (uint16 i = 0; i < size; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    for (uint16 i = 0; i < size; i++) {
        for (uint16 j = 0; j < size; j++) {
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

void resetBoard(uint16 arr[], uint16 size) {
    for (uint16 i = 0; i < size; i++) {
        arr[i] = i;
    }

    const uint16 MAX_ITERATIONS = 1;

    for (uint16 k = 0; k < MAX_ITERATIONS; k++) {
        for (uint16 i = 0; i < size; i++) {
            uint16 j = rand() % size;
            uint16 temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
        }
    }
}

void calculateConflicts(uint16 arr[], uint16 size) {
    for (uint16 i = 0; i < size; i++) {
        g_conflicts[i] = getConflictsOfCell(arr, size, arr[i], i);
        totalConflicts += g_conflicts[i];
        // printf("%d ", g_conflicts[i]);
    }
    // printf("\n");
}

uint16 getColWithMaxConflicts(uint16 arr[], uint16 size) {

    uint16 colWithMaxConflicts = 0;
    uint16 maxConflicts = 0;

    uint16 candidates[BOARD_MAX_SIZE];
    uint16 candidatesCount = 0;

    for (uint16 col = 0; col < size; col++) {
        uint16 conflicts = g_conflicts[col];

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

    uint16 idx = rand() % candidatesCount;
    return candidates[idx];
}

// Return: the rowIndex where the queen will make minimum conflicts
uint16 getRowWithMinConflicts(uint16 arr[], uint16 size, uint16 col) {

    uint16 rowWithMinConflicts = arr[col];
    uint16 minConflictsCount = 30000;

    uint16 candidates[BOARD_MAX_SIZE];
    uint16 candidatesCount = 0;

    for (uint16 row = 0; row < size; row++) {
        if (row == arr[col]) {
            continue;
        }

        uint16 conflicts = getConflictsOfCell(arr, size, row, col);

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

    uint16 idx = rand() % candidatesCount;
    return candidates[idx];    
}

boolean hasConflicts(uint16 arr[], uint16 size) {
    return totalConflicts > 0;
}

void updateConflicts(uint16 arr[], uint16 size, uint16 newRow, uint16 oldRow, uint16 col) {

    for (uint16 x = 0; x < size; x++) {
        if (x == col) {
            continue;
        }

        uint16 y = arr[x];

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

boolean findSolution(uint16 arr[], uint16 size) {
    const uint16 MAX_ITERATIONS = 3 * size;

    resetBoard(arr, size);
    calculateConflicts(arr, size);

    for (uint16 i = 0; i < MAX_ITERATIONS; i++) {
        uint16 col = getColWithMaxConflicts(arr, size);

        uint16 oldRow = arr[col];
        uint16 newRow = getRowWithMinConflicts(arr, size, col);

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

void solve(uint16 arr[], uint16 size) {
    const uint16 MAX_RETRIES = 5;

    for (uint16 i = 0; i < MAX_RETRIES; i++) {
        if (findSolution(arr, size)) {
            printf("Success!\n");
            // printBoard(arr, size);
            exit(EXIT_SUCCESS);
        }
    }

    printf("No solution was found!\n");
    exit(EXIT_FAILURE);
}

uint16 main() {
    srand(time(0));

    uint16 boardSize = 1000;
    uint16 rowIndex[BOARD_MAX_SIZE];

    solve(rowIndex, boardSize);

    return EXIT_SUCCESS;
}