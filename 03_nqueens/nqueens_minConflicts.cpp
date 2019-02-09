/*! N-Queen problem
 *  
 * \brief       Place N chess queens on an N×N chessboard so that no two queens attack each other.
 * \author      Boris Ivanov
 *  
 * The provided solution implements MinConflicts algorithm (LSA, Hill Climbing).
 *
 * Results: 
 *       - n = 1000 -> T = 0.01 s
 *       - n = 10000 -> T = 0.28 s
 *       - n = 20000 -> T = 1.20 s
 *       - n = 30000 -> T = 2.89 s
 *       - n = 50000 -> T = 8.65 s
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
 *       - Keep static look-up table for the conflicts of the queen:
 *           - queensInMainDiag[2 * size - 1] - keeps count of the queens in every "left diagonal". How are they stored:
 *               // Main diagonals index for 3 * 3 board => 5 "left" diagonals in total
 *               // |1|2|3|
 *               // |4|1|2|    
 *               // |5|4|1|
 *
 *          - queensInSecDiag[2 * size - 1] - keeps count of the queens in every "right diagonal". How are they stored:
 *              // Secondary diagonals index
 *              // |1|2|3|
 *              // |2|3|4|    
 *              // |3|4|5|
 *
 *          - queensInRow[size] - keeps count of the queens in every row
**/

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <cmath>
#include <algorithm>
#include <vector>

class Board {
public:
    std::vector<int> rows;
    int size;

private:
    std::vector<int> queensInMainDiag; // keeps count of the queens in every "left diagonal".
    std::vector<int> queensInSecDiag; // keeps count of the queens in every "right diagonal".
    std::vector<int> queensInRow; // keeps count of the queens in every row

    /*! \brief      Finds in which "left" diagonal belongs the specified cell
     * \return      Diagonal in which the specified cell belongs
     *
     * Main diagonals index example for N = 3
     * |1|2|3|
     * |4|1|2|
     * |5|4|1|
    **/
    int getMainDiagonalIndexForCell(int row, int col) {
        if (col - row > 0) {
            return col - row;
        }
        else {
            return abs(col - row) + this->size - 1;
        }
    }

    /*! \brief      Finds in which "right" diagonal belongs the specified cell
     * \return      Diagonal in which the specified cell belongs
     *
     * Secondary diagonals index example for N = 3
     * |1|2|3|
     * |2|3|4|    
     * |3|4|5|
    **/
    int getSecDiagonalIndexForCell(int row, int col) {
        return row + col;
    }

public:
    Board(int size) : rows(std::vector<int>(size)),
        queensInMainDiag(std::vector<int>(size * 2 - 1)), 
        queensInSecDiag(std::vector<int>(size * 2 - 1)), 
        queensInRow(std::vector<int>(size))
     {
        this->size = size;
    }

    /// \brief      Print the whole board
    void print() {
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
        for (int col = 0; col < size; col++) {
            if (getConflictsOfQueen(col) > 0) {
                return true;
            }
        }

        return false;
    }

    int getConflictsOfQueen(int col) {
        int row = rows[col];
        return queensInRow[row] + queensInMainDiag[getMainDiagonalIndexForCell(row, col)] + queensInSecDiag[getSecDiagonalIndexForCell(row, col)] - 3;
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
     * This function is invoked only after reset of the board. 
    **/
    void calculateConflicts() {
        std::fill(queensInRow.begin(), queensInRow.end(), 0);
        std::fill(queensInMainDiag.begin(), queensInMainDiag.end(), 0);
        std::fill(queensInSecDiag.begin(), queensInSecDiag.end(), 0);

        for (int col = 0; col < size; col++) {
            int row = rows[col];

            queensInRow[row]++;
            queensInMainDiag[getMainDiagonalIndexForCell(row, col)]++;
            queensInSecDiag[getSecDiagonalIndexForCell(row, col)]++;
            
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

        std::vector<int> candidates(size);
        int candidatesCount = 0;

        for (int col = 0; col < size; col++) {
            int row = rows[col];
            int currConflicts = queensInRow[row] + queensInMainDiag[getMainDiagonalIndexForCell(row, col)] + queensInSecDiag[getSecDiagonalIndexForCell(row, col)];

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

        std::vector<int> candidates(size);
        int candidatesCount = 0;

        for (int row = 0; row < size; row++) {
            if (row == rows[col]) {
                continue;
            }

            int confl = queensInRow[row] + queensInMainDiag[getMainDiagonalIndexForCell(row, col)] + queensInSecDiag[getSecDiagonalIndexForCell(row, col)];

            if (confl < minConflictsCount) {
                rowWithMinConflicts = row;
                minConflictsCount = confl;

                candidatesCount = 0;
                candidates[candidatesCount++] = row;
            }
            else if (confl == minConflictsCount) {
                candidates[candidatesCount++] = row;
            }
        }

        int idx = rand() % candidatesCount;
        return candidates[idx];
    }

    /*! \brief      Updates the static lookup arrays of the conflicts for the affected queens.
     *  \param       newRow: index of the new row of the queen
     *  \param       oldRow: index of the old row of the queen
     *  \param       col: index of the column of the queen
    **/
    void updateConflicts(int newRow, int oldRow, int col) {
        queensInRow[oldRow]--;
        queensInMainDiag[getMainDiagonalIndexForCell(oldRow, col)]--;
        queensInSecDiag[getSecDiagonalIndexForCell(oldRow, col)]--;

        queensInRow[newRow]++;
        queensInMainDiag[getMainDiagonalIndexForCell(newRow, col)]++;
        queensInSecDiag[getSecDiagonalIndexForCell(newRow, col)]++;
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
                board.print();
            }
            exit(EXIT_SUCCESS);
        }
    }

    printf("No solution was found!\n");
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {

    srand(time(0));

    int boardSize = 10000;
    int canPrintSolution = false;

    // Set boardSize from command line argument
    if (argc >= 2) {
        boardSize = atoi(argv[1]);
    }

    // Set canPrintSolution from command line argument
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