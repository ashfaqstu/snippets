#include<bits/stdc++.h>
using namespace std;

class Graph {
    int V;
    vector<vector<int>> adj;

  

public:
    Graph(int vertices) {
        this->V = vertices;
        adj.resize(vertices);
    }

    void add_edge(int u, int v) {
        if (u >= 0 && v >= 0 && u < V && v < V) {
            adj[u].push_back(v);
            adj[v].push_back(u); 
        }
    }

    void remove_edge(int u, int v) {
        if (!(u >= 0 && v >= 0 && u < V && v < V)) return;
        auto it_u = find(adj[u].begin(), adj[u].end(), v);
        if (it_u != adj[u].end()) adj[u].erase(it_u);
        auto it_v = find(adj[v].begin(), adj[v].end(), u);
        if (it_v != adj[v].end()) adj[v].erase(it_v);
    }

    // --- NEW BFS METHOD ---
    void BFS(int startNode) {
        vector<bool> visited(V, false);
        queue<int> q;

        visited[startNode] = true;
        q.push(startNode);

        cout << "BFS Order: ";
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            cout << u << " ";

            for (int v : adj[u]) {
                if (!visited[v]) {
                    visited[v] = true;
                    q.push(v);
                }
            }
        }
        cout << endl;
    }

   
    void print() {
        for (int i = 0; i < V; i++) {
            cout << "Vertex " << i << ": ";
            for (auto n : adj[i]) cout << n << " ";
            cout << endl;
        }
    }
    void dfsRec(vector<vector<int>> &adj, vector<bool> &visited, int s, vector<int> &res) {
        
        visited[s] = true;

        res.push_back(s);

        // Recursively visit all adjacent 
        // vertices that are not visited yet
        for (int i : adj[s])
            if (visited[i] == false)
                dfsRec(adj, visited, i, res);
    }

    vector<int> dfs(vector<vector<int>> &adj) {
        vector<bool> visited(adj.size(), false);
        vector<int> res;
        // Loop through all vertices 
        // to handle disconnected graph
        for (int i = 0; i < adj.size(); i++) {
            if (visited[i] == false) {
                dfsRec(adj, visited, i, res);
            }
        }

        return res;
    }
    // Helper for Kosaraju's: Fills stack by finish time
    void fillOrder(int u, vector<bool>& visited, stack<int>& s) {
        visited[u] = true;
        for (int v : adj[u]) {
            if (!visited[v]) {
                fillOrder(v, visited, s);
            }
        }
        // Push to stack only after processing all children
        s.push(u); 
    }

    // Helper for Kosaraju's: Standard DFS on the Transposed Graph
    void DFS_on_Transpose(int u, vector<bool>& visited, const vector<vector<int>>& adjRev) {
        visited[u] = true;
        cout << u << " ";
        for (int v : adjRev[u]) {
            if (!visited[v]) {
                DFS_on_Transpose(v, visited, adjRev);
            }
        }
    }
    // Kosaraju's Algorithm to print SCCs
    void printSCCs() {
        stack<int> s;
        vector<bool> visited(V, false);

        // Step 1: Fill vertices in stack according to their finishing times
        for (int i = 0; i < V; i++) {
            if (!visited[i]) {
                fillOrder(i, visited, s);
            }
        }

        // Step 2: Create a reversed (transposed) graph
        vector<vector<int>> adjRev(V);
        for (int v = 0; v < V; v++) {
            for (int neighbor : adj[v]) {
                // Original: v -> neighbor. Reverse: neighbor -> v
                adjRev[neighbor].push_back(v); 
            }
        }

        // Step 3: Process all vertices in order defined by Stack
        // Reset visited array for the second pass
        fill(visited.begin(), visited.end(), false);
        
        cout << "Strongly Connected Components:\n";
        while (!s.empty()) {
            int v = s.top();
            s.pop();

            // If node is not visited in the second pass, it's the start of a new SCC
            if (!visited[v]) {
                cout << "{ ";
                DFS_on_Transpose(v, visited, adjRev);
                cout << "}" << endl;
            }
        }
    }
    bool check_bipartite() {
        vector<int> color(V, -1);
        for (int i = 0; i < V; i++) {
            if (color[i] != -1) continue;
            queue<int> q;
            color[i] = 0; // Fixed the bug here
            q.push(i);
            while (!q.empty()) {
                int u = q.front();
                q.pop();
                for (auto v : adj[u]) {
                    if (color[v] == -1) {
                        color[v] = 1 - color[u];
                        q.push(v);
                    } else if (color[v] == color[u]) {
                        return false;
                    }
                }
            }
        }
        return true;
    }
};

int main() {
    // 0 -- 1
    // |    |
    // 3 -- 2    (connected to 4)
    Graph g(5);
    g.add_edge(0, 1);
    g.add_edge(1, 2);
    g.add_edge(2, 3);
    g.add_edge(3, 0);
    g.add_edge(2, 4); 

    cout << "Graph Structure:" << endl;
    g.print();
    cout << endl;

    // Test B

    return 0;
}