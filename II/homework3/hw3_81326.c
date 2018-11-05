#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef unsigned char boolean;
typedef unsigned char uint8;
typedef char sint8;
typedef unsigned short uint16;
typedef short sint16;
typedef unsigned int uint32;
typedef int sint32;
typedef unsigned long long uint64;
typedef long long sint64;

#ifndef FALSE
    #define FALSE (0U)
#endif
#ifndef TRUE
    #define TRUE  (1U)
#endif

void resetBoard(uint16* arr, uint16 size) {
    for (uint16 i = 0; i < size; i++) {
        arr[i] = rand() % size;
    }
}

uint16 getColWithMaxConflicts(uint16* arr, uint16 size) {
    return 0U;
}

uint16 getRowWithMinConflicts(uint16* arr, uint16 size) {
    return 0U;
}

boolean hasConflicts(uint16* arr, uint16 size) {
    return FALSE;
}


boolean solve(uint16* arr, uint16 size) {
    const uint8 K = 3;
    const uint32 MAX_ITERATIONS = K * size;

    resetBoard(arr, size);

    for (uint32 i = 0; i < MAX_ITERATIONS; i++) {
        uint16 col = getColWithMaxConflicts(arr, size);
        arr[col] = getRowWithMinConflicts(arr, size);

        if (!hasConflicts(arr, size)) {
            return TRUE;
        }
    }

    return FALSE;
}

int main() {
    srand(time(NULL));

    const uint8 MAX_RETRIES = 100;

    uint16 n = 10U;
    uint16 *rowIndex = malloc(n * sizeof(uint16));

    for (uint8 i = 0; i < MAX_RETRIES; i++) {
        if (solve(rowIndex, n)) {
            printf("Success!\n");
            exit(EXIT_SUCCESS);
        }
    }
    
    printf("No solution was found!\n");
    exit(EXIT_FAILURE);

    return 0;
}