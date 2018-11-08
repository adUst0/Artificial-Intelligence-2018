//------------------------------------------------------------------------------
// INCLUDED FILES
//------------------------------------------------------------------------------

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <cmath>
#include <algorithm>

//------------------------------------------------------------------------------
// TYPE DEFINITIONS
//------------------------------------------------------------------------------

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

const uint16 BOARD_MAX_SIZE = 12000;

//------------------------------------------------------------------------------
// PUBLIC FUNCTIONS
//------------------------------------------------------------------------------

class Board {
public:
    uint16 rows[BOARD_MAX_SIZE];
    uint16 size;
    uint16 conflicts[BOARD_MAX_SIZE];
    uint16 conflictsCount;

private:
    uint16 getConflictsOfCell(uint16 row, uint16 col) {
        uint16 conflicts = 0;

        for (uint16 x = 0; x < size; x++) {
            if (x == col) {
                continue;
            }

            uint16 y = rows[x];

            if (y == row || abs(y - row) == abs(x - col)) {
                conflicts++;
            }
        }

        return conflicts;
    }

public:
    Board(uint16 size) {
        this->size = size;
        this->conflictsCount = 0;
    }

    void printBoard() {
        for (uint16 i = 0; i < size; i++) {
            printf("%d ", rows[i]);
        }
        printf("\n");

        for (uint16 i = 0; i < size; i++) {
            for (uint16 j = 0; j < size; j++) {
                if (rows[j] != i) {
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

    void reset() {
        for (uint16 i = 0; i < size; i++) {
            rows[i] = i;
        }

        for (uint16 i = 0; i < size; i++) {
            uint16 j = rand() % size;
            std::swap(rows[i], rows[j]);
        }    
    }

    void calculateConflicts() {
        conflictsCount = 0;
        for (uint16 i = 0; i < size; i++) {
            conflicts[i] = getConflictsOfCell(rows[i], i);
            conflictsCount += conflicts[i];
        }
    }

    uint16 getColWithMaxConflicts() {
        uint16 colWithMaxConflicts = 0;
        uint16 maxConflicts = 0;

        uint16 candidates[BOARD_MAX_SIZE];
        uint16 candidatesCount = 0;

        for (uint16 col = 0; col < size; col++) {
            uint16 currConflicts = conflicts[col];

            if (currConflicts > maxConflicts) {
                maxConflicts = currConflicts;
                colWithMaxConflicts = col;

                candidatesCount = 0;
                candidates[candidatesCount++] = col;
            }
            else if (currConflicts == maxConflicts) {
                candidates[candidatesCount++] = col;
            }
        }

        uint16 idx = rand() % candidatesCount;
        return candidates[idx];    
    }

    // Return: the rowIndex where the queen will make minimum conflicts
    uint16 getRowWithMinConflicts(uint16 col) {
        uint16 rowWithMinConflicts = rows[col];
        uint16 minConflictsCount = 30000;

        uint16 candidates[BOARD_MAX_SIZE];
        uint16 candidatesCount = 0;

        for (uint16 row = 0; row < size; row++) {
            if (row == rows[col]) {
                continue;
            }

            uint16 conflicts = getConflictsOfCell(row, col);

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


    bool hasConflicts() {
        return conflictsCount > 0;
    }

    void updateConflicts(uint16 newRow, uint16 oldRow, uint16 col) {
        for (uint16 x = 0; x < size; x++) {
            if (x == col) {
                continue;
            }

            uint16 y = rows[x];

            if (y == newRow || abs(y - newRow) == abs(x - col)) {
                conflicts[x]++;
                conflictsCount++;
            }

            if (y == oldRow || abs(y - oldRow) == abs(x - col)) {
                conflicts[x]--;
                conflictsCount--;
            }
        }

        conflictsCount -= conflicts[col];
        conflicts[col] = getConflictsOfCell(rows[col], col);
        conflictsCount += conflicts[col];
    }

};


bool findSolution(Board &board) {
    const uint16 MAX_ITERATIONS = 3 * board.size;

    board.reset();
    board.calculateConflicts();

    for (uint16 i = 0; i < MAX_ITERATIONS; i++) {
        uint16 col = board.getColWithMaxConflicts();

        uint16 oldRow = board.rows[col];
        uint16 newRow = board.getRowWithMinConflicts(col);

        if (oldRow != newRow) {
            board.rows[col] = newRow;
            board.updateConflicts(board.rows[col], oldRow, col);
        }

        if (!board.hasConflicts()) {
            return true;
        }
    }

    return false;
}

void solve(Board &board) {
    const uint16 MAX_RETRIES = 5;

    for (uint16 i = 0; i < MAX_RETRIES; i++) {
        if (findSolution(board)) {
            printf("Success!\n");
            // printBoard();
            exit(EXIT_SUCCESS);
        }
    }

    printf("No solution was found!\n");
    exit(EXIT_FAILURE);
}

int main() {
    srand(time(0));

    uint16 boardSize = 10000;
    Board board(boardSize);

    solve(board);

    return EXIT_SUCCESS;
}