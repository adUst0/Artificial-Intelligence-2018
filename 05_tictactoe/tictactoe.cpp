/*! Tic-tac-toe
 *
 * \brief       Homework 5
 * \author      Boris Ivanov
 *
 * Implement AI for Tic-tac-toe using Minimax algorithm with alpa-beta pruning
 *      - Must be able to choose who is the first player
 *      - The algorithm must be optimal (the fastest way to win => min depth )
 *
 * In the current implementation MAX will be the AI and MIN will be the human
 *
 * Minimax tree leaves value:
 *      * 1  -> Max wins
 *      * -1 -> Min wins
 *      * 0  -> Draw
**/

#include <iostream>
#include <algorithm>
#include <cstring>
#include <vector>

using namespace std;

#define s32INF (0xFFFFFFF)

// CONFIGURATION
#define PLAYER_MAX ('x')
#define PLAYER_MIN ('o')
#define PLAYER_MAX_WIN_STRING ("xxx")
#define PLAYER_MIN_WIN_STRING ("ooo")

class Board {
    char raw[3][3];
    static const char POS_EMPTY = ' ';
public:
    Board() {
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                raw[i][j] = POS_EMPTY;
            }
        }
    }

    bool hasNoEmptyCells() const {
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (isPosEmpty(i, j)) {
                    return false;
                }
            }
        }

        return true;
    }

    bool isPosEmpty(int i, int j) const {
        return raw[i][j] == POS_EMPTY;
    }

    char* operator[](int i) {
        return raw[i];
    }

    const char* operator[](int i) const {
        return raw[i];
    }

    string getRow(int row) const {
        string value = string() + raw[row][0] + raw[row][1] + raw[row][2];
        return value;
    }

    string getCol(int col) const {
        string value = string() + raw[0][col] + raw[1][col] + raw[2][col];
        return value;
    }

    string getMainDiagonal() const {
        string value = string() + raw[0][0] + raw[1][1] + raw[2][2];
        return value;
    }

    string getSecondaryDiagonal() const {
        string value = string() + raw[0][2] + raw[1][1] + raw[2][0];
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

//==============================================================================
// IMPLEMENTATION
//==============================================================================

// Alpha-beta algorithm
Board AlphaBetaDecision(Board& board);
int MaxValue(Board& board, int alpha, int beta);
int MinValue(Board& board, int alpha, int beta);

// Helper functions
vector<Board> getChildren(Board &board, char player);
int getTerminalStateValue(const Board& board);
bool isTerminalState(const Board &board);

/*! \brief      Finds best possible move for player MAX
 * \param       board current state in game
 * \return      Best possible move for MAX
**/
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

/*! \brief      Finds maximum value for current state
 * \param       board current state in game
 * \param       alpha the value of the best alternative for max along the path
 *              to state
 * \param       beta the value of the best alternative for min along the path
 *              to state
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
 * \param       board current state in game
 * \param       alpha the value of the best alternative for max along the path
 *              to state
 * \param       beta the value of the best alternative for min along the path
 *              to state
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

/*! \brief      Finds all possible moves from the current state for particular
 *              player
 * \param       board current state in game
 * \param       player the player who is in turn
 * \return      all possible moves for this player
**/
vector<Board> getChildren(Board &board, char player) {
    vector<Board> children;

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (!board.isPosEmpty(i, j)) {
                continue;
            }
            Board nextState = board;
            nextState[i][j] = player;
            children.push_back(nextState);
        }
    }

    return children;
}

/*! \brief      Returns the value in a terminal state
 * \param       board current (terminal) state in game
 * \return      1 or -1 or 0
**/
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

/*! \brief      Checks whether the current state is a terminal state
 * \param       board current state in game
 * \return      true / false
**/
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

class Game {
    Board board;
    enum {HUMAN, AI} firstPlayer;

    pair<int, int> getHumanNextPosition() {
        int i, j;
        cout << "Row Col: ";
        cin >> i >> j;

        while(i < 0 || j < 0 || i > 2 || j > 2 ||
            !board.isPosEmpty(i, j)) {
            cout << "Incorrect position. Try again!" << endl;
            cout << "Row Col: ";
            cin >> i >> j;
        }

        return make_pair(i, j);
    }

    void printResult() {
        if (getTerminalStateValue(board) == 1) {
            cout << "Player MAX has won!" << endl;
        }
        else if (getTerminalStateValue(board) == -1) {
            cout << "Player MIN has won!" << endl;
        }
        else {
            cout << "Draw!" << endl;
        }
    }
public:
    Game() {
        firstPlayer = AI;
        cout << "==================================================" << endl;
        cout << "*                 Tic-tac-toe AI                 *" << endl;
        cout << "*                                                *" << endl;
        cout << "* - Player plays with O                          *" << endl;
        cout << "* - AI plays with X                              *" << endl;
        cout << "==================================================\n\n";
    }

    void choosePlayer() {
        cout << "Be the first player? [y/n]: ";
        char c;
        cin >> c;
        firstPlayer = (c == 'y') ? HUMAN : AI;
    }

    void start() {
        if (firstPlayer == HUMAN) {
            cout << board << endl;
            pair<int, int> pos = getHumanNextPosition();
            board[pos.first][pos.second] = PLAYER_MIN;
            cout << board << endl;
        }

        while (!isTerminalState(board)) {
            board = AlphaBetaDecision(board);
            cout << board << endl;
            if (isTerminalState(board)) {
                break;
            }
            pair<int, int> pos = getHumanNextPosition();
            board[pos.first][pos.second] = PLAYER_MIN;
            cout << board << endl;
        }

        printResult();
    }
};

int main() {

    Game game;
    game.choosePlayer();
    game.start();

    return 0;
}