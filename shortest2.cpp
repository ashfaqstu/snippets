#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
const ll INF = 1e18;

vector<vector<pair<int, int>>> adj;
vector<vector<ll>> all; // Matrix to store all-pairs distances

void djk(int src, int n) {
    priority_queue<pair<ll, int>, vector<pair<ll, int>>, greater<pair<ll, int>>> pq;
    vector<ll> dist(n + 1, INF);
    
    dist[src] = 0;
    pq.emplace(0, src);

    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();

        if (d > dist[u]) continue;

        for (auto &p : adj[u]) {
            int v = p.first;
            int weight = p.second;

            if (dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                pq.emplace(dist[v], v);
            }
        }
    }
    
    // Correct way: Save the entire result of this Dijkstra run
    all[src] = dist; 
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, m, q;
    if (!(cin >> n >> m >> q)) return 0;

    // Warning: Only works if N is relatively small (e.g., N <= 5000)
    all.resize(n + 1);
    adj.resize(n + 1);

    for (int i = 0; i < m; i++) {
        int u, v, w;
        cin >> u >> v >> w;
        adj[u].push_back({v, w});
        adj[v].push_back({u, w});
        // If the graph is undirected, add: adj[v].push_back({u, w});
    }

    // Run Dijkstra from every single node
    for (int i = 1; i <= n; i++) {
        djk(i, n);
    }

    // Process queries
    for (int i = 0; i < q; i++) {
        int x, y;
        cin >> x >> y;
        ll result = all[x][y];
        if (result == INF) cout << -1 << "\n";
        else cout << result << "\n";
    }

    return 0;
}