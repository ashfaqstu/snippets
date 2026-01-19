#include<bits/stdc++.h>
using namespace std;

#define int long long

void solve() {
    int n, m;
    cin >> n >> m;

    vector<vector<pair<int, int>>> adj(n);
    for (int i = 0; i < m; i++) {
        int u, v, w;
        cin >> u >> v >> w;
        adj[u].push_back({v, w});
        adj[v].push_back({u, w});
    }

    int a, b;              // mandatory edge a-b
    cin >> a >> b;

    vector<int> vis(n, 0);
    vector<pair<int,int>> mst_edges;

    priority_queue<
        tuple<int, int, int>,
        vector<tuple<int, int, int>>,
        greater<tuple<int, int, int>>
    > pq;

    int total_weight = 0;

    // ---------- FORCE mandatory edge ----------
    bool found = false;
    int mandatory_weight = 0;

    for (auto &e : adj[a]) {
        if (e.first == b) {
            mandatory_weight = e.second;
            found = true;
            break;
        }
    }

    if (!found) {
        cout << "mandatory edge not present\n";
        return;
    }

    vis[a] = vis[b] = 1;
    total_weight += mandatory_weight;
    mst_edges.push_back({a, b});

    // push edges from both endpoints
    for (auto &e : adj[a]) {
        if (!vis[e.first]) {
            pq.push({e.second, e.first, a});
        }
    }

    for (auto &e : adj[b]) {
        if (!vis[e.first]) {
            pq.push({e.second, e.first, b});
        }
    }

    // ---------- NORMAL PRIM ----------
    while (!pq.empty()) {
        auto [w, u, p] = pq.top();
        pq.pop();

        if (vis[u]) continue;

        vis[u] = 1;
        total_weight += w;
        mst_edges.push_back({p, u});

        for (auto &edge : adj[u]) {
            int v = edge.first;
            int weight = edge.second;
            if (!vis[v]) {
                pq.push({weight, v, u});
            }
        }
    }

    // ---------- CONNECTIVITY CHECK ----------
    bool connected = true;
    for (int i = 0; i < n; i++) {
        if (!vis[i]) {
            connected = false;
            break;
        }
    }

    if (!connected) {
        cout << "all node not connected\n";
    } else {
        cout << "Total weight " << total_weight << "\n";
        cout << "Mandatory edge " << a << " " << b << "\n";
        for (auto &edge : mst_edges) {
            cout << edge.first << " " << edge.second << "\n";
        }
    }
}

int32_t main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    solve();
    return 0;
}
