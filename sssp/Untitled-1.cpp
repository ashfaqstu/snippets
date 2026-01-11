#include<bits/stdc++.h>
using namespace std;

typedef long long ll;
const ll INF = 1e18; // 1. Use a much larger INF

vector<vector<pair<int,int>>> adj;

void djk(int src, int n){ // 2. Added n to help with printing
    priority_queue<pair<ll,int>, vector<pair<ll,int>>, greater<pair<ll,int>>> pq;
    vector<ll> dist(n + 1, INF); // 3. Use long long and correct size
    
    dist[src] = 0;
    pq.emplace(0, src); // Correct order: {distance, node}

    while(!pq.empty()){
        auto [d, u] = pq.top();
        pq.pop();

        if(d > dist[u]) continue;

        for(auto &p : adj[u]){
            int v = p.first;
            int weight = p.second; // Renamed 'd' to 'weight' to avoid confusion

            if(dist[u] + weight < dist[v]){
                dist[v] = dist[u] + weight;
                pq.emplace(dist[v], v); // 4. Correct order: {distance, node}
            }
        }
    }
    
    for(int i = 1; i <= n; i++){ // 5. Print only valid cities 1 to n
        cout << dist[i] << (i == n ? "" : " ");
    }
}

int main(){
    ios_base::sync_with_stdio(false); // 6. Added for performance
    cin.tie(NULL);

    int n, m;
    if(!(cin >> n >> m)) return 0;

    adj.resize(n + 1);
    for(int i = 0; i < m; i++){
        int u, v, w;
        cin >> u >> v >> w;
        adj[u].push_back({v, w});
    }
    djk(1, n);

    return 0;
}