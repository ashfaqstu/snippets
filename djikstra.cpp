#include <bits/stdc++.h>
using namespace std;

// Define Infinity as a large number
const int INF = 1e9;

class WeightedGraph {
    int V;
    // pair<int, int> represents {neighbor, weight}
    vector<vector<pair<int, int>>> adj;

public:
    WeightedGraph(int vertices) {
        this->V = vertices;
        adj.resize(vertices);
    }

    // Modified add_edge to accept weight
    void add_edge(int u, int v, int weight) {
        if (u >= 0 && v >= 0 && u < V && v < V) {
            adj[u].push_back({v, weight});
            adj[v].push_back({u, weight}); // Undirected
        }
    }

    // --- DIJKSTRA'S ALGORITHM ---
    void dijkstra(int startNode) {
        // Min-Priority Queue: Stores {current_distance, node}
        // "greater" ensures the smallest distance is always at the top
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;

        // Vector for distances, initialized to Infinity
        vector<int> dist(V, INF);

        // 1. Initialize start node
        dist[startNode] = 0;
        pq.push({0, startNode}); // Push {distance, node}

        cout << "Running Dijkstra from node " << startNode << "...\n";

        while (!pq.empty()) {
            // 2. Get the node with the smallest distance
            int currentDist = pq.top().first;
            int u = pq.top().second;
            pq.pop();

            // Optimization: If we found a shorter path to u before processing this, skip
            if (currentDist > dist[u]) continue;

            // 3. Explore neighbors
            for (auto edge : adj[u]) {
                int v = edge.first;       // Neighbor node
                int weight = edge.second; // Edge weight

                // Relaxation Step:
                // If path through u is shorter than current known path to v
                if (dist[u] + weight < dist[v]) {
                    dist[v] = dist[u] + weight;
                    pq.push({dist[v], v});
                }
            }
        }

        // 4. Print Results
        cout << "Vertex\tDistance from Source\n";
        for (int i = 0; i < V; ++i) {
            cout << i << "\t";
            if (dist[i] == INF) 
                cout << "INF\n";
            else 
                cout << dist[i] << "\n";
        }
    }
};

int main() {
    // Creating the graph shown below
    //      (4)
    //  0 ------- 1
    //  |        /|
    //  |      /  |
    // (1)   (2) (5)
    //  |  /      |
    //  |/        |
    //  2 ------- 3
    //      (8)

    WeightedGraph g(4);
    
    // add_edge(u, v, weight)
    g.add_edge(0, 1, 4);
    g.add_edge(0, 2, 1);
    g.add_edge(1, 2, 2);
    g.add_edge(1, 3, 5);
    g.add_edge(2, 3, 8);

    g.dijkstra(0);

    return 0;
}