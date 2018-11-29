/*! Tic-tac-toe
 *
 * \brief       Homework 5
 * \author      Boris Ivanov
 *
 * Implement AI for Tic-tac-toe using Minimax algorithm with alpa-beta pruning
 *      - Must be able to choose who is the first player
 *      - The algorithm must be optimal (the fastest way to win)
 *
 * In the current implementation MAX will be the AI and MIN will be the human
 *
 * Minimax tree leaves value:
 *      * 1  -> Max wins
 *      * -1 -> Min wins
 *      * 0  -> Draw
**/

#define s32INF (0xFFFFFFF)

#define PLAYER_MAX ('x')
#define PLAYER_MIN ('o')
#define PLAYER_MAX_WIN_STRING ("xxx")
#define PLAYER_MIN_WIN_STRING ("ooo")

#define BOARD_POS_FREE (' ')

#include <iostream>
#include <algorithm>
#include <cstring>
#include <vector>

using namespace std;

class Board {
    char board[3][3];
public:
    Board() {
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                board[i][j] = ' ';
            }
        }
    }

    bool hasNoEmptyCells() const {
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (board[i][j] == ' ') {
                    return false;
                }
            }
        }

        return true;
    }

    char* operator[](int i) {
        return board[i];
    }

    const char* operator[](int i) const {
        return board[i];
    }

    string getRow(int row) const {
        string value = string() + board[row][0] + board[row][1] + board[row][2];
        return value;
    }

    string getCol(int col) const {
        string value = string() + board[0][col] + board[1][col] + board[2][col];
        return value;
    }

    string getMainDiagonal() const {
        string value = string() + board[0][0] + board[1][1] + board[2][2];
        return value;
    }

    string getSecondaryDiagonal() const {
        string value = string() + board[0][2] + board[1][1] + board[2][0];
        return value;
    }

    friend ostream& operator<<(ostream& os, const Board& board) {
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                os << board[i][j] << "|";
            }
            cout << endl;
        }

        return os;
    }
};

enum Turn {PLAYER, AI};

int getTerminalStateValue(const Board& board);
bool isTerminalState(const Board &board);
int MaxValue(Board& board, int alpha, int beta);
int MinValue(Board& board, int alpha, int beta);
Board AlphaBetaDecision(Board& board);
vector<Board> getChildren(Board &board, char player);

vector<Board> getChildren(Board &board, char player) {
    vector<Board> children;

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] != BOARD_POS_FREE) {
                continue;
            }
            Board nextState = board;
            nextState[i][j] = player;
            children.push_back(nextState);
        }
    }

    return children;
}

int getTerminalStateValue(const Board& board) {

    string mainDiagonal = board.getMainDiagonal();
    string secondaryDiagonal = board.getSecondaryDiagonal();

    if (mainDiagonal == PLAYER_MAX_WIN_STRING ||
        secondaryDiagonal == PLAYER_MAX_WIN_STRING) {
        return 1;
    }

    if (mainDiagonal == PLAYER_MIN_WIN_STRING ||
        secondaryDiagonal == PLAYER_MIN_WIN_STRING) {
        return -1;
    }

    for (int i = 0; i < 3; i++) {
        string row = board.getRow(i);
        string col = board.getCol(i);
        if (row == PLAYER_MAX_WIN_STRING ||
            col == PLAYER_MAX_WIN_STRING) {
            return 1;
        }
        if (row == PLAYER_MIN_WIN_STRING ||
            col == PLAYER_MIN_WIN_STRING) {
            return -1;
        }
    }

    return 0;
}

bool isTerminalState(const Board &board) {

    if (board.hasNoEmptyCells()) {
        return true;
    }

    string mainDiagonal = board.getMainDiagonal();
    string secondaryDiagonal = board.getSecondaryDiagonal();

    if (mainDiagonal == PLAYER_MAX_WIN_STRING ||
        secondaryDiagonal == PLAYER_MAX_WIN_STRING ||
        mainDiagonal == PLAYER_MIN_WIN_STRING ||
        secondaryDiagonal == PLAYER_MIN_WIN_STRING) {
        return true;
    }

    for (int i = 0; i < 3; i++) {
        string row = board.getRow(i);
        string col = board.getCol(i);
        if (row == PLAYER_MAX_WIN_STRING ||
            col == PLAYER_MAX_WIN_STRING ||
            row == PLAYER_MIN_WIN_STRING ||
            col == PLAYER_MIN_WIN_STRING) {
            return true;
        }
    }

    return false;
}

/*! \brief      Find maximum value for current state
 * \param       board - current state in game
 * \param       alpha - the value of the best alternative for max along the path to state
 * \param       beta - the value of the best alternative for min along the path to state
 * \return      1 or 0 or -1
**/
int MaxValue(Board& board, int alpha, int beta) {
    if (isTerminalState(board)) {
        return getTerminalStateValue(board);
    }

    int best = -s32INF;
    vector<Board> children = getChildren(board, PLAYER_MAX);

    for (auto child : children) {
        best = std::max(best, MinValue(child, alpha, beta));

        if (best >= beta) {
            return best;
        }

        alpha = std::max(alpha, best);
    }

    return best;
}

/*! \brief      Finds minimum value for current state
 * \param       board - current state in game
 * \param       alpha - the value of the best alternative for max along the path to state
 * \param       beta - the value of the best alternative for min along the path to state
 * \return      1 or 0 or -1
**/
int MinValue(Board& board, int alpha, int beta) {
    if (isTerminalState(board)) {
        return getTerminalStateValue(board);
    }

    int best = s32INF;
    vector<Board> children = getChildren(board, PLAYER_MIN);

    for (auto child : children) {
        best = std::min(best, MaxValue(child, alpha, beta));

        if (best <= alpha) {
            return best;
        }

        beta = std::min(beta, best);
    }

    return best;
}

Board AlphaBetaDecision(Board& board) {
    int maxValue = -1;
    Board bestBoard = board;

    vector<Board> children = getChildren(board, PLAYER_MAX);

    for (auto child : children) {
        int value = MinValue(child, -s32INF, s32INF);

        if (value > maxValue) {
            maxValue = value;
            bestBoard = child;
        }
    }

    return bestBoard;
}

void makeMove(Board& board, Turn turn) {
    if (turn == PLAYER) {

    }
    else {
        AlphaBetaDecision(board);
    }
}

int main() {
    cout << "==================================================" << endl;
    cout << "*                 Tic-tac-toe AI                 *" << endl;
    cout << "*                                                *" << endl;
    cout << "* - Player plays with O                          *" << endl;
    cout << "* - AI plays with X                              *" << endl;
    cout << "==================================================\n\n";

    Board board;

    cout << "Be the first player? [y/n] " << endl;
    char c;
    cin >> c;

    if (c == 'y') {
        cout << board << endl;
        int i, j;
        cout << "Row Col: "; cin >> i >> j;
        board[i][j] = PLAYER_MIN;
        cout << board << endl;
    }

    while (!isTerminalState(board)) {
        board = AlphaBetaDecision(board);
        cout << board << endl;
        if (isTerminalState(board)) {
            break;
        }
        int i, j;
        cout << "Row Col: "; cin >> i >> j;
        board[i][j] = PLAYER_MIN;
        cout << board << endl;
    }

    if (getTerminalStateValue(board) == 1) {
        cout << "Player MAX has won!" << endl;
    }
    else if (getTerminalStateValue(board) == -1) {
        cout << "Player MIN has won!" << endl;
    }
    else {
        cout << "Draw!" << endl;
    }

    return 0;
}