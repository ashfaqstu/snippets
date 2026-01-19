#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
#define int long long

class DSU {
public:
    vector<int> parent;
    vector<int> sz;
    DSU(int n) {
        parent.resize(n + 1);
        sz.assign(n + 1, 1);
        for (int i = 0; i <= n; i++) parent[i] = i;
    }
    int find(int i) {
        if (parent[i] == i) return i;
        return parent[i] = find(parent[i]);
    }
    bool unite(int i, int j) {
        int root_i = find(i);
        int root_j = find(j);
        if (root_i != root_j) {
            if (sz[root_i] < sz[root_j]) swap(root_i, root_j);
            parent[root_j] = root_i;
            sz[root_i] += sz[root_j];
            return true;
        }
        return false;
    }
};

struct Edge {
    int u, v, w, id;
    bool in_mst = false;
};

// Global max_edge matrix for V^2 approach
int max_e[2005][2005]; 
vector<pair<int, int>> adj[2005];

void dfs(int start, int u, int p, int current_max) {
    max_e[start][u] = current_max;
    for (auto &edge : adj[u]) {
        int v = edge.first;
        int w = edge.second;
        if (v != p) {
            dfs(start, v, u, max(current_max, w));
        }
    }
}

void solve() {
    int n, m;
    if (!(cin >> n >> m)) return;

    vector<Edge> edges(m);
    for (int i = 0; i < m; i++) {
        cin >> edges[i].u >> edges[i].v >> edges[i].w;
        edges[i].id = i;
    }

    // 1. Sort edges for Kruskal's
    sort(edges.begin(), edges.end(), [](Edge a, Edge b) {
        return a.w < b.w;
    });

    DSU dsu(n);
    int mst_weight = 0;
    int edges_count = 0;

    for (int i = 0; i < m; i++) {
        if (dsu.unite(edges[i].u, edges[i].v)) {
            mst_weight += edges[i].w;
            edges[i].in_mst = true;
            adj[edges[i].u].push_back({edges[i].v, edges[i].w});
            adj[edges[i].v].push_back({edges[i].u, edges[i].w});
            edges_count++;
        }
    }

    if (edges_count != n - 1) {
        cout << "Disconnected graph, no MST possible." << endl;
        return;
    }

    // 2. Precompute the maximum edge on the path between every pair (u, v) in the MST
    for (int i = 1; i <= n; i++) {
        dfs(i, i, -1, 0);
    }

    // 3. Find the minimum increase by adding a non-MST edge
    int second_best_weight = LLONG_MAX;
    bool found = false;

    for (int i = 0; i < m; i++) {
        if (!edges[i].in_mst) {
            int u = edges[i].u;
            int v = edges[i].v;
            int w = edges[i].w;

            // Adding this edge creates a cycle; remove the max edge in that cycle
            // Current weight = MST - (Max edge on MST path u-v) + (Current non-MST edge)
            int current_res = mst_weight - max_e[u][v] + w;
            
            // If you need a "strictly" second best, ensure current_res > mst_weight
            if (current_res >= mst_weight) {
                second_best_weight = min(second_best_weight, current_res);
                found = true;
            }
        }
    }

    cout << "MST Weight: " << mst_weight << endl;
    if (!found || second_best_weight == LLONG_MAX) {
        cout << "Second Best MST: Not Possible" << endl;
    } else {
        cout << "Second Best MST Weight: " << second_best_weight << endl;
    }
}

int32_t main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    solve();
    return 0;
}