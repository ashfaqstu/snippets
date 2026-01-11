#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
const ll INF = 1e18;

// We need two adjacency lists
vector<vector<pair<int, int>>> adj;
vector<vector<pair<int, int>>> rev_adj;

// Modified Dijkstra to return the distance vector
vector<ll> djk(int src, int n, vector<vector<pair<int, int>>>& graph) {
    // priority_queue stores {distance, node}
    priority_queue<pair<ll, int>, vector<pair<ll, int>>, greater<pair<ll, int>>> pq;
    vector<ll> dist(n + 1, INF);
    
    dist[src] = 0;
    pq.push({0, src});

    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();

        if (d > dist[u]) continue;

        for (auto& edge : graph[u]) {
            int v = edge.first;
            int weight = edge.second;

            if (dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                pq.push({dist[v], v});
            }
        }
    }
    return dist;
}

int main() {
    

    int n, m;
    cin >> n >> m;

    adj.resize(n + 1);
    rev_adj.resize(n + 1);
    
    // To iterate over edges easily later, we can store them in a list
    struct Edge {
        int u, v, w;
    };
    vector<Edge> edges;

    for (int i = 0; i < m; i++) {
        int u, v, w;
        cin >> u >> v >> w;
        adj[u].push_back({v, w});
        rev_adj[v].push_back({u, w}); // Reverse graph
        edges.push_back({u, v, w});
    }

    // 1. Distance from Syrjälä (Node 1) in the original graph
    vector<ll> dist_from_start = djk(1, n, adj);

    // 2. Distance to Metsälä (Node n) in the reversed graph
    vector<ll> dist_to_end = djk(n, n, rev_adj);

    ll min_total_price = INF;

    // 3. Iterate through every edge and try applying the coupon to it
    for (auto& e : edges) {
        // We only consider the edge if the endpoints were reachable
        if (dist_from_start[e.u] != INF && dist_to_end[e.v] != INF) {
            ll current_price = dist_from_start[e.u] + (e.w / 2) + dist_to_end[e.v];
            min_total_price = min(min_total_price, current_price);
        }
    }

    cout << min_total_price << endl;

    return 0;
}