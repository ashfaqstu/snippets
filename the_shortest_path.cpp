#include<bits/stdc++.h>
using namespace std;

const long long INF = 1e16; // 1e18 can sometimes cause overflow when adding weights

struct Edge {
    int u, v;
    long long weight;
};

void solve() {
    int n, m, k, b, e_count;
    if (!(cin >> n >> m >> k >> b >> e_count)) return;

    vector<int> capitals(k);
    for (int i = 0; i < k; i++) cin >> capitals[i];

    vector<int> blk(n + 1, 0);
    for (int i = 0; i < b; i++) {
        int tmp;
        cin >> tmp;
        blk[tmp] = 1;
    }

    vector<int> emergency_posts(e_count);
    for (int i = 0; i < e_count; i++) cin >> emergency_posts[i];

    vector<Edge> edges;
    for (int i = 0; i < m; i++) {
        int u, v;
        long long w;
        cin >> u >> v >> w;
        // Only consider edges where neither city is blocked
        if (!blk[u] && !blk[v]) {
            edges.push_back({u, v, w});
        }
    }

    // Single distance vector for all capitals
    vector<long long> dist(n + 1, INF);
    
    // MULTI-SOURCE INITIALIZATION
    for (int cap : capitals) {
        if (!blk[cap]) dist[cap] = 0;
    }

    // Bellman-Ford: Relax edges N-1 times
    for (int i = 1; i < n; i++) {
        bool changed = false;
        for (const auto& edge : edges) {
            if (dist[edge.u] != INF && dist[edge.u] + edge.weight < dist[edge.v]) {
                dist[edge.v] = dist[edge.u] + edge.weight;
                changed = true;
            }
        }
        if (!changed) break; 
    }

    // Negative cycle detection
    for (const auto& edge : edges) {
        if (dist[edge.u] != INF && dist[edge.u] + edge.weight < dist[edge.v]) {
            cout << "Abyss Detected" << endl;
            return;
        }
    }

    // Output formatted correctly
    for (int post : emergency_posts) {
        if (blk[post] || dist[post] >= (INF / 2)) {
            cout << "INF" << endl;
        } else {
            cout << dist[post] << endl;
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    solve();
    return 0;
}