#include <iostream>
#include <queue>
#include <set>
#include <cstring>
using namespace std;

const int N = 3;
struct Node {
    int raw[N][N];
    int* operator[](int n) {
        return raw[n];
    }
};

struct NodeInfo
{
    Node node;
    Node prev;
    int distance;

    NodeInfo() : distance(0) {
        memset(&node, -1, sizeof(node));
        memset(&prev, -1, sizeof(prev));
    }

    void copyNode(const NodeInfo& other) {
        memcpy(&node, &other.node, sizeof(node));
    }

    void setPrev(const NodeInfo& other) {
        memcpy(&prev, &other.node, sizeof(prev));
    }

    bool operator==(const NodeInfo& other) const {
        return memcmp(&node, &other.node, sizeof(node)) == 0;
    }

    bool operator<(const NodeInfo& other) const {
        return distance >= other.distance;
    }

    // break some encapsulation
    int* operator[](int n) {
        return node[n];
    }
    
};

NodeInfo start;
NodeInfo final;

bool isFinal(NodeInfo node) {
    return memcmp(node.node.raw, final.node.raw, sizeof(node.node.raw)) == 0;
}

pair<int, int> findEmptyPos(NodeInfo node) {
    for(int i = 0; i < N; i++) {
        for(int j = 0; j < N; j++) {
            if(node[i][j] == 0) {
                return make_pair(i, j);
            }
        }
    }
}

vector<NodeInfo> neighbors(NodeInfo node) {
    vector<NodeInfo> neigh;
    pair<int, int> emptyPos = findEmptyPos(node);

    if(emptyPos.second + 1 < N) {
        // left
        neigh.push_back(NodeInfo());
        neigh[neigh.size() - 1].copyNode(node); // ugly code. Maybe refactoring in the future. For now is better performance. 
        swap(neigh[neigh.size() - 1][emptyPos.first][emptyPos.second + 1], neigh[neigh.size() - 1][emptyPos.first][emptyPos.second]);
    }
    if(emptyPos.second - 1 >= 0) {
        // right
        neigh.push_back(NodeInfo());
        neigh[neigh.size() - 1].copyNode(node); // ugly code. Maybe refactoring in the future. For now is better performance. 
        swap(neigh[neigh.size() - 1][emptyPos.first][emptyPos.second - 1], neigh[neigh.size() - 1][emptyPos.first][emptyPos.second]);
    }
    if(emptyPos.first + 1 < N) {
        // up
        neigh.push_back(NodeInfo());
        neigh[neigh.size() - 1].copyNode(node); // ugly code. Maybe refactoring in the future. For now is better performance. 
        swap(neigh[neigh.size() - 1][emptyPos.first + 1][emptyPos.second], neigh[neigh.size() - 1][emptyPos.first][emptyPos.second]);
    }
    if(emptyPos.first - 1 >= 0) {
        // down
        neigh.push_back(NodeInfo());
        neigh[neigh.size() - 1].copyNode(node); // ugly code. Maybe refactoring in the future. For now is better performance. 
        swap(neigh[neigh.size() - 1][emptyPos.first - 1][emptyPos.second], neigh[neigh.size() - 1][emptyPos.first][emptyPos.second]);
    }
 
    return neigh;   
}

void solve() {
    set<NodeInfo> visited;
    priority_queue<NodeInfo> pq;

    visited.insert(start);
    pq.push(start);

    while(!pq.empty()) {
        NodeInfo current = pq.top();
        pq.pop();

        if(isFinal(current)) {
            cout << current.distance << endl;
            return;
        }

        for(NodeInfo &node : neighbors(current)) {
            if(visited.count(node) == 0) {
                node.distance = current.distance + 1;
                node.setPrev(current);
                visited.insert(node);
                pq.push(node);
            }
        }
    }

}

int main() {

    int st[N][N] = {
        {2, 3, 6},
        {1, 0, 7},
        {4, 8, 5}
    };
    int end[N][N] = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 0}
    };

    for(int i = 0; i < N; i++) {
        for(int j = 0; j < N; j++) {
            start[i][j] = st[i][j];
            final[i][j] = end[i][j];
        }
    }

    solve();

    return 0;
}