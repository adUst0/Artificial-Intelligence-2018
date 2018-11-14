#include <iostream>
#include <queue>
#include <vector>
#include <cstring>
#include <algorithm>
#include <stack>
using namespace std;

class NodeInfo
{
    vector<vector<int>> currentState;
    vector<vector<int>> prevState;
    int distance;
    int size;

    bool operator>(const NodeInfo& other) const {
        int wthis = distance + this->getManh();
        int wother = other.distance + other.getManh();

        return wthis > wother;
    }

    int getManh() const {
        int sum = 0;

        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                pair<int, int> realPosition = make_pair(i, j);
                pair<int, int> neededPosition;
                int value = currentState[i][j];

                if (value == 0) {
                    neededPosition = make_pair(size - 1, size - 1);
                }
                else {
                    neededPosition = make_pair((value - 1) / size, (value - 1) % size);
                }

                sum += abs(neededPosition.first - realPosition.first) + abs(neededPosition.second - realPosition.second);
            }
        }

        return sum;
    }

public:
    NodeInfo(int size) : 
        distance(0),
        size(size),
        currentState(vector<vector<int>>(size, vector<int>(size, -1))), 
        prevState(vector<vector<int>>(size, vector<int>(size, -1))) 
    {

    }

    void setCurrentState(const vector<vector<int>>& state) {
        if (state.size() != size) {
            cerr << "Error: Trying to setCurrentState with different dimensions" << endl;
            exit(-1);
        }
        this->currentState = state;
    }

    const vector<vector<int>>& getCurrentState() const {
        return currentState;
    }

    vector<vector<int>>& getCurrentState() {
        return currentState;
    }

    void setPrevState(const vector<vector<int>>& state) {
        if (state.size() != size) {
            cerr << "Error: Trying to setPrevState with different dimensions" << endl;
            exit(-1);
        }
        this->prevState = state;
    }

    const vector<vector<int>>& getPrevState() const {
        return prevState;
    }

    int getDistance() const {
        return distance;
    }

    void setDistance(int distance) {
        this->distance = distance;
    }

    int getSize() const {
        return size;
    }

    bool operator==(const NodeInfo& other) const {
        return currentState == other.currentState;
    }

    bool operator!=(const NodeInfo& other) const {
        return !operator==(other);
    }

    bool operator<(const NodeInfo& other) const {
        return *this > other;
    }

    bool isFinalState() const {
        return getManh() == 0;
    }

    pair<int, int> findEmptyBlockPos() const {
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                if (currentState[i][j] == 0) {
                    return make_pair(i, j);
                }
            }
        }
    }

    vector<NodeInfo> neighbors() const {
        vector<NodeInfo> Neighbors;
        pair<int, int> emptyBlockPos = findEmptyBlockPos();

        int left = emptyBlockPos.second + 1;
        int right = emptyBlockPos.second - 1;
        int up = emptyBlockPos.first + 1;
        int down = emptyBlockPos.first - 1;

        NodeInfo node(size);

        if (left < size) {
            node.setCurrentState(this->currentState);
            swap(node.currentState[emptyBlockPos.first][left],
                 node.currentState[emptyBlockPos.first][emptyBlockPos.second]);
            Neighbors.push_back(node);
        }
        if (right >= 0) {
            node.setCurrentState(this->currentState);
            swap(node.currentState[emptyBlockPos.first][right],
                 node.currentState[emptyBlockPos.first][emptyBlockPos.second]);
            Neighbors.push_back(node);
        }
        if (up < size) {
            node.setCurrentState(this->currentState);
            swap(node.currentState[up][emptyBlockPos.second],
                 node.currentState[emptyBlockPos.first][emptyBlockPos.second]);
            Neighbors.push_back(node);
        }
        if (down >= 0) {
            node.setCurrentState(this->currentState);
            swap(node.currentState[down][emptyBlockPos.second],
                 node.currentState[emptyBlockPos.first][emptyBlockPos.second]);
            Neighbors.push_back(node);
        }

        return Neighbors;
    }
};

const NodeInfo& findPrevNode(const vector<NodeInfo>& visited, const NodeInfo& current) {
    for (const NodeInfo& node : visited) {
        if (node.getCurrentState() == current.getPrevState()) {
            return node;
        }
    }
}

void printPath(NodeInfo& start, NodeInfo& current, vector<NodeInfo> visited) {
    int size = current.getSize();
    stack<string> s;

    while (current != start) {
        pair<int, int> emptyBlockPos = current.findEmptyBlockPos();

        int left = emptyBlockPos.second - 1;
        int right = emptyBlockPos.second + 1;
        int up = emptyBlockPos.first - 1;
        int down = emptyBlockPos.first + 1;

        if (left >= 0 && current.getPrevState()[emptyBlockPos.first][left] == 0) {
            s.push("left");
        }
        else if (right < size && current.getPrevState()[emptyBlockPos.first][right] == 0) {
            s.push("right");
        }
        else if (up >= 0 && current.getPrevState()[up][emptyBlockPos.second] == 0) {
            s.push("up");
        }
        else if (down < size && current.getPrevState()[down][emptyBlockPos.second] == 0) {
            s.push("down");
        }

        current.findEmptyBlockPos();
        current = findPrevNode(visited, current);
    }

    while (!s.empty()) {
        cout << s.top() << endl;
        s.pop();
    }
}

void solve(NodeInfo& start) {
    vector<NodeInfo> visited;
    priority_queue<NodeInfo> pq;

    visited.push_back(start);
    pq.push(start);

    while (!pq.empty()) {
        NodeInfo current = pq.top();
        pq.pop();

        if (current.isFinalState()) {
            cout << current.getDistance() << endl;
            printPath(start, current, visited);
            return;
        }

        for (NodeInfo &node : current.neighbors()) {
            if (find(visited.begin(), visited.end(), node) == visited.end()) {
                node.setDistance(current.getDistance() + 1);
                node.setPrevState(current.getCurrentState());
                visited.push_back(node);
                pq.push(node);
            }
        }
    }
}

int main() {
    int n = 0;

    cout << "N = ";
    cin >> n;

    NodeInfo start(n);

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cin >> start.getCurrentState()[i][j];
        }
    }

    solve(start);

    return 0;
}

/*
3
6 2 3
1 4 8
7 0 5

*/

/*
3
1 2 3
4 5 6
0 7 8

*/