#include <iostream>
#include <iomanip>
#include <vector>
#include <queue>
using namespace std;

struct Pos {
    int i, j;

    Pos(int i_ = -1, int j_ = -1) : i(i_), j(j_) {}

    bool operator==(const Pos& o) const {
        return i == o.i && j == o.j;
    }
};

class Maze {
private:
    vector<vector<char> > raw;
    int size;

public:
    Maze(int N) : 
        raw(N, vector<char>(N, '1')), 
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

// Abstract class PathFinder
class PathFinder {
private:
    int size;
    vector<vector<Pos>> prevCell; // previous cell in the path (i.e. from which cell we arrived to the current in the traverse)
    vector<vector<bool>> visited;

protected:
    Maze &maze;

public:    
    virtual void findPath(Pos current, Pos target) = 0;

    PathFinder(Maze &maze_) : 
        maze(maze_), 
        size(maze_.getSize()), 
        prevCell(size, vector<Pos>(size)),
        visited(size, vector<bool>(size, false)) {

    }

    bool isVisited(int i, int j) const {
        return visited[i][j];
    }

    void markVisited(int i, int j) {
        visited[i][j] = true;
    }

    vector<Pos>& operator[](const int i) {
        return prevCell[i];
    }

    void printPath(Pos end) {
        Maze temp(this->maze);

        while(temp.isValidPosition(end.i, end.j)) {
            temp[end.i][end.j] = '*';
            end = (*this)[end.i][end.j];
        }


        for(int i = 0; i < temp.getSize(); i++) {
            for(int j = 0; j < temp.getSize(); j++) {
                cout << temp[i][j] << " ";
            }
            cout << endl;
        }
    }

};

class DFSPathFinder : public PathFinder {
public:
    DFSPathFinder(Maze &maze_) : PathFinder(maze_) {}

    virtual void findPath(Pos current, Pos target) {
        this->markVisited(current.i, current.j);

        if(this->isVisited(target.i, target.j)) {
            return; // abort if a path to the target cell is already found
        }

        for(int k = - 1; k <= 1; k += 2) 
        {
            int i = current.i;
            int j = current.j;

            if(this->maze.isValidPosition(i + k, j)) {
                if(!this->maze.isWall(i + k, j) && !this->isVisited(i + k, j)) {
                    (*this)[i + k][j] = current;
                    findPath(Pos(i + k, j), target);
                }
            }

            if(this->maze.isValidPosition(i, j + k)) {
                if(!this->maze.isWall(i, j + k) && !this->isVisited(i, j + k)) {
                    (*this)[i][j + k] = current;
                    findPath(Pos(i, j + k), target);
                }
            }
        }
    }
};

class BFSPathFinder : public PathFinder {
public:
    BFSPathFinder(Maze &maze_) : PathFinder(maze_) {}

    virtual void findPath(Pos start, Pos target) {
        this->markVisited(start.i, start.j);

        queue<Pos> q;
        q.push(start);

        while(!q.empty()) {
            Pos current = q.front();
            q.pop();

            if(this->isVisited(target.i, target.j)) {
                return; // abort if a path to the target cell is already found
            }

            for(int k = - 1; k <= 1; k += 2) 
            {
                int i = current.i;
                int j = current.j;

                if(this->maze.isValidPosition(i + k, j)) {
                    if(!this->maze.isWall(i + k, j) && !this->isVisited(i + k, j)) {
                        (*this)[i + k][j] = current;
                        this->markVisited(i + k, j);
                        q.push(Pos(i + k, j));
                    }
                }

                if(this->maze.isValidPosition(i, j + k)) {
                    if(!this->maze.isWall(i, j + k) && !this->isVisited(i, j + k)) {
                        (*this)[i][j + k] = current;
                        this->markVisited(i, j + k);
                        q.push(Pos(i, j + k));
                    }
                }
            }
        }
    }
};

int main() {
    int N, k;

    cout << "N = "; 
    cin >> N;

    cout << "k = ";
    cin >> k;

    // Aks for custom input
    char yn = '0';
    while(yn != 'y' && yn != 'n') {
        cout << "Custom input(y/n): ";
        cin >> yn;
    }

    Maze maze(N);
    PathFinder *pathFinder1 = new BFSPathFinder(maze);
    PathFinder *pathFinder2 = new DFSPathFinder(maze);

    // Gather custom input or generate random cells 
    if(yn == 'y') {
        for(int s = 0; s < k; s++) {
            int i, j;
            cout << "Wall" << s + 1 << "(i j): ";
            cin >> i >> j;
            maze[i][j] = '0';
        }
    }
    else {
        for(int s = 0; s < k; s++) {
            int i = rand() % N;
            int j = rand() % N;
            while(maze[i][j] == '0') {
                i = rand() % N;
                j = rand() % N;
            }
            maze[i][j] = '0';
        }
    }

    Pos start, end;
    cout << "Start(i j): ";
    cin >> start.i >> start.j;
    cout << "End(i j): ";
    cin >> end.i >> end.j;

    pathFinder1->findPath(start, end);
    cout << "\n\nBFS:\n";
    pathFinder1->printPath(end);

    pathFinder2->findPath(start, end);
    cout << "\n\nDFS:\n";
    pathFinder2->printPath(end);

    return 0;
}

/*
Input from the example in zad1.pdf
7
7
y
0 2
1 1
1 2
3 3
4 3
5 2
5 3
0 0
5 4

*/

