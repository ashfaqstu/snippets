#include<bits/stdc++.h>
using namespace std;

#define int long long

void solve() {
    int n, m;
    cin>>n>>m;
    vector<vector<pair<int, int>>> adj(n);
    for (int i = 0; i < m; i++) {
        int u, v, w;
        cin >> u >> v >> w;
        adj[u].push_back({v,w});
        adj[v].push_back({u,w});
    }
    int root;
    cin>>root;
    vector<int> vis(n, 0);
    vector<pair<int,int>>mst_edges; 

    priority_queue<tuple<int, int, int>, vector<tuple<int, int, int>>, greater<tuple<int, int, int>>> pq;

    int total_weight=0;

    pq.push({0,root,-1});

    while (!pq.empty()) {
        auto [w, u, p] = pq.top();
        pq.pop();

        if (vis[u]) continue;

        vis[u] = 1;
        total_weight += w;
        if (p!=-1) {
            mst_edges.push_back({p, u});
        }

        for (auto &edge : adj[u]) {
            int v = edge.first;
            int weight = edge.second;
            if (!vis[v]) {
                pq.push({weight, v, u});
            }
        }
    }

    bool connected=true;
    for(int i=0;i<n;i++) {
        if(!vis[i]){
            connected=false;
            break;
        }
    }

    if (!connected) {
        cout << "all node not connected" << endl;
    } else {
        cout << "Total weight " << total_weight << endl;
        cout << "Root node " << root<<endl;
        for (auto &edge : mst_edges) {
            cout << edge.first << " " << edge.second << endl;
        }
    }
}

int32_t main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    solve();
    return 0;
}