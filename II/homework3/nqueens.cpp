/*! N-Queen problem
 *  
 * \brief       Place N chess queens on an N×N chessboard so that no two queens attack each other.
 * \author      Boris Ivanov
 *  
 * The provided solution implements MinConflicts algorithm (LSA, Hill Climbing).
 *
 * Results: 
 *       - n = 1000 -> T = 1 s
 *       - n = 10000 -> T = 550 s
 *
 * We will put all queens in different columns and change only their row. 
 *       => We can store the board with 1-D array, keeping only the row of the current queen.
 *
 * Example: N = 4. rows[] = {1, 3, 0, 2}
 *                               _ _ * _
 *                               * _ _ _
 *                               _ _ _ *
 *                               _ * _ _
 *
 * Pseudocode:
 *       rows[N] = rand[N]
 *       K = {2, 3}
 *       MAX_ITERATIONS = N * K
 *
 *       for (i : 1, MAX_ITERATIONS)
 *           col = getColWithMaxConflicts()
 *           row = getRowWithMinConflicts(col)
 *           
 *           rows[col] = row;
 *
 *           if (!hasConflicts())
 *               print "Solution was found!"
 *               exit
 *
 *       restart // no solution was found
 *
 * Optimizations:
 *       - On rows initialization, put every queen on different row.
 *       - RandomRestart: Generate random board on every restart. (But keep every queen on different row. => Random shuffle)
 *       - getColWithMaxConflicts() : if more than one candidate, choose randomly between the candidates
 *       - getRowWithMinConflicts(col) : if more than one candidate, choose randomly between the candidates
 *       - Keep static look-up table for the conflicts of the queen: conflicts[size];
 *       - Keep total count of conflicts => hasConflicts() - O(1)
 *
**/

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <cmath>
#include <algorithm>

const int BOARD_MAX_SIZE = 12000;

class Board {
public:
    int rows[BOARD_MAX_SIZE];
    int size;
    int conflicts[BOARD_MAX_SIZE];
    int conflictsCount;

private:
    /*! \brief      Calculates the number of conflicts for a given cell.
     * \param       row: row index
     * \param       col: col index
     * \return      Number of Conflicts for the specified cell by row, col
     *
     * Only horizontal, main diagonal and secondary diagonal conflicts are counted
     * because the queens are guaranteed to be in different columns.
     *
     * Since vertical conflicts are not calculated, it is possible to calculate
     * the conflicts for given queen in all possible rows, without acctually moving 
     * the queen to other row. For example: given col and row = rows[col], we can 
     * calculate the conflicts of cell [row + 2] [col] without the need of moving the queen +2 rows.
    **/
    int getConflictsOfCell(int row, int col) {
        int conflicts = 0;

        for (int x = 0; x < size; x++) {
            if (x == col) {
                continue;
            }

            int y = rows[x];

            if (y == row || abs(y - row) == abs(x - col)) {
                conflicts++;
            }
        }

        return conflicts;
    }

public:
    Board(int size) {
        this->size = size;
        this->conflictsCount = 0;
    }

    /// \brief      Print the whole board
    void printBoard() {
        for (int i = 0; i < size; i++) {
            printf("%d ", rows[i]);
        }
        printf("\n");

        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
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

    /*! \brief      Checks if there are conflicts on the board
     * \return      true / false
    **/
    bool hasConflicts() {
        return conflictsCount > 0;
    }

    /*! \brief      Reset the current board (this->rows)
     * 
     * Every queen is put to new line and then random shuffle is performed.
    **/
    void reset() {
        for (int i = 0; i < size; i++) {
            rows[i] = i;
        }

        for (int i = 0; i < size; i++) {
            int j = rand() % size;
            std::swap(rows[i], rows[j]);
        }    
    }

    /*! \brief      Calculates the conflicts of every queen.
     * 
     * This method is used to initially calculate the conflicts for every queen.
    **/
    void calculateConflicts() {
        conflictsCount = 0;
        for (int i = 0; i < size; i++) {
            conflicts[i] = getConflictsOfCell(rows[i], i);
            conflictsCount += conflicts[i];
        }
    }

    /*! \brief      Finds the queen that has most conflicts.
     * \return      Column (queen) with max conflicts
     *
     * If there is more than one queen with the max conflicts, 
     * choose randomly between all the candidates.
    **/
    int getColWithMaxConflicts() {
        int colWithMaxConflicts = 0;
        int maxConflicts = 0;

        int candidates[BOARD_MAX_SIZE];
        int candidatesCount = 0;

        for (int col = 0; col < size; col++) {
            int currConflicts = conflicts[col];

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

        int idx = rand() % candidatesCount;
        return candidates[idx];    
    }

    /*! \brief      Finds the queen that has most conflicts.
     * \return      The rowIndex where the queen will make(have) minimum conflicts
     *
     * If there is more than one row with min conflicts, 
     * choose randomly between all the candidates.
    **/
    int getRowWithMinConflicts(int col) {
        int rowWithMinConflicts = rows[col];
        int minConflictsCount = 30000;

        int candidates[BOARD_MAX_SIZE];
        int candidatesCount = 0;

        for (int row = 0; row < size; row++) {
            if (row == rows[col]) {
                continue;
            }

            int conflicts = getConflictsOfCell(row, col);

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

    /*! \brief      Updates the static lookup table of the conflicts for the affected queens and the total conflictsCount.
    *  \param       newRow: index of the new row of the queen
    *  \param       oldRow: index of the old row of the queen
    *  \param       col: index of the column of the queen
     *
     * For every queen except the current, 
     * check if newRow / oldRow affects the conflicts
     * and update accordingly. Finally, 
     * calculate the conflicts for the current col, because 
     * conflicts[col] keeps the conflicts of the old position.
    **/
    void updateConflicts(int newRow, int oldRow, int col) {
        for (int x = 0; x < size; x++) {
            if (x == col) {
                continue;
            }

            int y = rows[x];

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


/*! \brief      Tries to find a solution for the NQueens problem implementing MinConflicts algorithm
 * \param       board
 * \return      true if solution is found
 *              false if no solution is found for MAX_ITERATIONS, return false
**/
bool findSolution(Board &board) {
    const int MAX_ITERATIONS = 3 * board.size;

    board.reset();
    board.calculateConflicts();

    for (int i = 0; i < MAX_ITERATIONS; i++) {
        int col = board.getColWithMaxConflicts();

        int oldRow = board.rows[col];
        int newRow = board.getRowWithMinConflicts(col);

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

/*! \brief      Start MinConflicts until a solution is found or MAX_RETRIES are elpased. 
 * \param       board
**/
void solve(Board &board, bool canPrintSolution) {
    const int MAX_RETRIES = 1000;

    for (int i = 0; i < MAX_RETRIES; i++) {
        if (findSolution(board)) {
            printf("Success!\n");
            if (canPrintSolution) {
                board.printBoard();
            }
            exit(EXIT_SUCCESS);
        }
    }

    printf("No solution was found!\n");
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {

    srand(time(0));

    int boardSize = 1000;
    int canPrintSolution = false;

    /// Set boardSize from command line argument
    if (argc >= 2) {
        boardSize = atoi(argv[1]);
    }

    /// Set canPrintSolution from command line argument
    if (argc >= 3) {
        if (strcmp(argv[2], "y") == 0) {
            canPrintSolution = true;
        }
        else {
            canPrintSolution = false;
        }
    }

    Board board(boardSize);

    solve(board, canPrintSolution);

    return EXIT_SUCCESS;
}