#include <iostream>
#include <iomanip>
#include <vector>
using namespace std;

struct pos {
    int i, j;

    pos() : i(-1), j(-1) {}

    pos(int i_, int j_) : i(i_), j(j_) {}

    bool operator==(const pos& o) const {
        return i == o.i && j == o.j;
    }
};

class Maze {
private:
    vector<vector<char> > raw;
    int size;

public:
    Maze(int N) : 
        raw(N, vector<char>(N)), 
        size(N) {}

    Maze(const Maze& other) : raw(other.raw), size(other.size) {}

    bool isValidPosition(int i, int j) const {
        return i >= 0 && j >= 0 && i < size && j < size;
    }
    bool isWall(int i, int j) const {
        return raw[i][j] == '0';
    }
    int getSize() const {
        return size;
    }
    vector<char>& operator[](const int i) {
        return raw[i];
    }
};

class PathFinder {
private:
    int size;
    vector<vector<pos>> prevCell; // previous cell in the path
    vector<vector<bool>> visited;

public:    
    Maze &maze;

    PathFinder(Maze &maze_) : 
        maze(maze_), 
        size(maze_.getSize()), 
        prevCell(size, vector<pos>(size)),
        visited(size, vector<bool>(size, false)) {

    }
    bool isVisited(int i, int j) const {
        return visited[i][j] == true;
    }
    bool isVisited(pos cur) const {
        return visited[cur.i][cur.j] == true;;
    }
    void markVisited(int i, int j) {
        visited[i][j] = true;
    }
    void markVisited(pos cur) {
        visited[cur.i][cur.j] = true;
    }
    vector<pos>& operator[](const int i) {
        return prevCell[i];
    }

};

void dfs(PathFinder &pathFinder, pos current, pos target) {
    pathFinder.markVisited(current);

    if(pathFinder.isVisited(target)) {
        return; // abort if a path to the target cell is already found
    }

    for(int k = - 1; k <= 1; k += 2) 
    {
        int i = current.i;
        int j = current.j;

        if(pathFinder.maze.isValidPosition(i + k, j)) {
            if(!pathFinder.maze.isWall(i + k, j) && !pathFinder.isVisited(i + k, j)) {
                pathFinder[i + k][j] = current;
                dfs(pathFinder, pos(i + k, j), target);
            }
        }

        if(pathFinder.maze.isValidPosition(i, j + k)) {
            if(!pathFinder.maze.isWall(i, j + k) && !pathFinder.isVisited(i, j + k)) {
                pathFinder[i][j + k] = current;
                dfs(pathFinder, pos(i, j + k), target);
            }
        }

    }

}

void showPath(PathFinder &pathFinder, pos end) {
    Maze temp = pathFinder.maze;

    while(temp.isValidPosition(end.i, end.j)) {
        temp[end.i][end.j] = '*';
        end = pathFinder[end.i][end.j];
    }


    for(int i = 0; i < temp.getSize(); i++) {
        for(int j = 0; j < temp.getSize(); j++) {
            cout << temp[i][j] << " ";
        }
        cout << endl;
    }
}

int main() {
    int N, k;
    // cin >> N >> k;
    cin >> N;

    Maze maze(N);
    PathFinder pathFinder(maze);

    for(int i = 0; i < N; i++) {
        for(int j = 0; j < N; j++) {
            cin >> maze[i][j];
        }
    }

    pos start, end;
    cin >> start.i >> start.j;
    cin >> end.i >> end.j;

    dfs(pathFinder, start, end);
    showPath(pathFinder, end);

    return 0;
}