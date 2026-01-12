#include<bits/stdc++.h>
using namespace std;
#define int long long
const int INF = 1e15;

void solve() {
    int n, m;
    cin >> n >> m;
    
    // adj stores the original weights for the "last two edges" of the cycle
    // dist stores the shortest paths found so far
    vector<vector<int>> adj(n + 1, vector<int>(n + 1, INF));
    vector<vector<int>> dist(n + 1, vector<int>(n + 1, INF));

    for(int i = 1; i <= n; i++) dist[i][i] = 0;

    for(int i = 0; i < m; i++) {
        int u, v, w;
        cin >> u >> v >> w;
        if (w < adj[u][v]) {
            adj[u][v] = adj[v][u] = w;
            dist[u][v] = dist[v][u] = w;
        }
    }

    int min_cycle = INF;

    // The k loop MUST be outermost
    for(int k = 1; k <= n; k++) {
        
        // Before updating dist[i][j] with vertex k, 
        // find the shortest cycle where k is the largest index
        for(int i = 1; i < k; i++) {
            for(int j = i + 1; j < k; j++) {
                if(dist[i][j] < INF && adj[i][k] < INF && adj[k][j] < INF) {
                    min_cycle = min(min_cycle, dist[i][j] + adj[i][k] + adj[k][j]);
                }
            }
        }

        // Standard Floyd-Warshall update
        for(int i = 1; i <= n; i++) {
            for(int j = 1; j <= n; j++) {
                dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
            }
        }
    }

    if(min_cycle >= INF) cout << -1 << endl;
    else cout << min_cycle << endl;
}

int32_t main() {
    solve();
    return 0;
}