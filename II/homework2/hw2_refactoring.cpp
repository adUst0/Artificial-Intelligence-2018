#include <iostream>
#include <queue>
#include <vector>
#include <cstring>
#include <algorithm>
using namespace std;

struct NodeInfo
{
    vector<vector<int>> currentState;
    vector<vector<int>> prevState;
    int distance;

    NodeInfo(int size = 3) : 
        distance(0), 
        currentState(vector<vector<int>>(size, vector<int>(size, -1))), 
        prevState(vector<vector<int>>(size, vector<int>(size, -1))) 
    {

    }

    void setCurrentState(const NodeInfo& other) {
        this->currentState = other.currentState;
    }

    void setPrevState(const NodeInfo& other) {
        this->prevState = other.currentState;
    }

    bool operator==(const NodeInfo& other) const {
        return currentState == other.currentState;
    }

    bool operator<(const NodeInfo& other) const {
        return *this > other;
    }

    vector<int>& operator[](int n) {
        return currentState[n];
    }

    int at(int i, int j) const {
        return currentState[i][j];
    }

    int getSize() const {
        return currentState.size();
    }

    bool isFinalState() {
        return getManh() == 0;
    }

    vector<NodeInfo> neighbors() {
        vector<NodeInfo> neigh;
        pair<int, int> emptyPos = findEmptyPos();

        if(emptyPos.second + 1 < getSize()) {
            // left
            neigh.push_back(NodeInfo());
            neigh[neigh.size() - 1].setCurrentState(*this); // ugly code. Maybe refactoring in the future. For now is better performance.
            swap(neigh[neigh.size() - 1][emptyPos.first][emptyPos.second + 1], neigh[neigh.size() - 1][emptyPos.first][emptyPos.second]);
        }
        if(emptyPos.second - 1 >= 0) {
            // right
            neigh.push_back(NodeInfo());
            neigh[neigh.size() - 1].setCurrentState(*this); // ugly code. Maybe refactoring in the future. For now is better performance.
            swap(neigh[neigh.size() - 1][emptyPos.first][emptyPos.second - 1], neigh[neigh.size() - 1][emptyPos.first][emptyPos.second]);
        }
        if(emptyPos.first + 1 < getSize()) {
            // up
            neigh.push_back(NodeInfo());
            neigh[neigh.size() - 1].setCurrentState(*this); // ugly code. Maybe refactoring in the future. For now is better performance.
            swap(neigh[neigh.size() - 1][emptyPos.first + 1][emptyPos.second], neigh[neigh.size() - 1][emptyPos.first][emptyPos.second]);
        }
        if(emptyPos.first - 1 >= 0) {
            // down
            neigh.push_back(NodeInfo());
            neigh[neigh.size() - 1].setCurrentState(*this); // ugly code. Maybe refactoring in the future. For now is better performance.
            swap(neigh[neigh.size() - 1][emptyPos.first - 1][emptyPos.second], neigh[neigh.size() - 1][emptyPos.first][emptyPos.second]);
        }

        return neigh;
    }

private:
    bool operator>(const NodeInfo& other) const {
        int wthis = distance + this->getManh();
        int wother = other.distance + other.getManh();

        return wthis > wother;
    }

    int getManh() const {
        int sum = 0;

        for(int i = 0; i < getSize(); i++) {
            for(int j = 0; j < getSize(); j++) {
                pair<int, int> realPosition = make_pair(i, j);
                pair<int, int> neededPosition;

                if(at(i, j) == 0) {
                    neededPosition = make_pair(getSize() - 1, getSize() - 1);
                }
                else {
                    neededPosition = make_pair((at(i, j) - 1) / getSize(), (at(i, j) - 1) % getSize());
                }

                sum += abs(neededPosition.first - realPosition.first) + abs(neededPosition.second - realPosition.second);
            }
        }

        return sum;
    }

    pair<int, int> findEmptyPos() {
        for(int i = 0; i < getSize(); i++) {
            for(int j = 0; j < getSize(); j++) {
                if(currentState[i][j] == 0) {
                    return make_pair(i, j);
                }
            }
        }
    }

};

NodeInfo start;
NodeInfo final;

void solve() {
    vector<NodeInfo> visited;
    priority_queue<NodeInfo> pq;

    visited.push_back(start);
    pq.push(start);

    while(!pq.empty()) {
        NodeInfo current = pq.top();
        pq.pop();

        if(current.isFinalState()) {
            cout << current.distance << endl;
            return;
        }

        for(NodeInfo &node : current.neighbors()) {
            if(find(visited.begin(), visited.end(), node) == visited.end()) {
                node.distance = current.distance + 1;
                node.setPrevState(current);
                visited.push_back(node);
                pq.push(node);
            }
        }
    }

}

int main() {
    int st[][3] = {
        {6, 2, 3},
        {1, 4, 8},
        {7, 0, 5}
    };
    int end[][3] = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 0}
    };

    for(int i = 0; i < 3; i++)
        for(int j = 0; j < 3; j++) {
            start[i][j] = st[i][j];
            final[i][j] = end[i][j];
        }


    // // 1,1 - 2,1 = 1
    // // 2,1 - 1,1 = 1

    // for(int i = 0; i < N; i++) {
    //     for(int j = 0; j < N; j++) {
    //         start[i][j] = st[i][j];
    //         final[i][j] = end[i][j];
    //     }
    // }

    // // cout << start.getManh() << endl;

    solve();

    return 0;
}
