#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
#define int long long

class DSU {
public:
    vector<int> parent;
    vector<int> sz;
    DSU(int n) {
        parent.resize(n);
        sz.assign(n, 1);
        for (int i =0;i<n;i++) parent[i] = i;
    }
    int find(int i) {
        if (parent[i]==i)
            return i;
        return parent[i]=find(parent[i]);
    }
    bool unite(int i, int j) {
        int root_i=find(i);
        int root_j=find(j);
        if (root_i!= root_j) {
            if (sz[root_i]<sz[root_j]) swap(root_i, root_j);
            parent[root_j]=root_i;
            sz[root_i]+=sz[root_j];
            return true;
        }
        return false; 
    }
};

void solve(){
    int n, m, P;
    cin >> n >> m >> P;

    int K;
    cin >> K;
    vector<int> risky(n, 0);
    for(int i = 0; i < K; i++){
        int x; cin >> x;
        risky[x] = 1;
    }

    vector<tuple<int,int,int>> edges;
    for(int i = 0; i < m; i++){
        int u, v, w;
        cin >> u >> v >> w;
        edges.push_back({w, u, v});
    }

    DSU dsu(n);
    vector<pair<int,int>> used_edges;
    ll total_cost = 0;

    /* ---------- PHASE 1: SAFE-ONLY EDGES ---------- */
    vector<tuple<int,int,int>> safe_edges;
    for(auto &e : edges){
        auto [w, u, v] = e;
        if(!risky[u] && !risky[v]){
            safe_edges.push_back(e);
        }
    }

    sort(safe_edges.begin(), safe_edges.end());

    for(auto &e : safe_edges){
        auto [w, u, v] = e;
        if(dsu.unite(u, v)){
            total_cost += w;
            used_edges.push_back({u, v});
        }
    }

    /* Check if all safe cities are connected */
    int safe_root = -1;
    bool connected = true;
    for(int i = 0; i < n; i++){
        if(!risky[i]){
            if(safe_root == -1) safe_root = dsu.find(i);
            else if(dsu.find(i) != safe_root){
                connected = false;
                break;
            }
        }
    }

    /* ---------- PHASE 2: ADD RISKY EDGES IF NEEDED ---------- */
    if(!connected){
        vector<tuple<int,int,int>> risky_edges;
        for(auto &e : edges){
            auto [w, u, v] = e;
            int cnt = risky[u] + risky[v];
            int eff = w + cnt * P;
            risky_edges.push_back({eff, u, v});
        }

        sort(risky_edges.begin(), risky_edges.end());

        for(auto &e : risky_edges){
            auto [eff, u, v] = e;
            if(dsu.unite(u, v)){
                total_cost += eff;
                used_edges.push_back({u, v});
            }
        }
    }

    /* Final check */
    safe_root = -1;
    for(int i = 0; i < n; i++){
        if(!risky[i]){
            if(safe_root == -1) safe_root = dsu.find(i);
            else if(dsu.find(i) != safe_root){
                cout << -1 << "\n";
                return;
            }
        }
    }

    /* Output */
    cout << used_edges.size() << "\n";
    for(auto &e : used_edges){
        cout << e.first << " " << e.second << "\n";
    }
    cout << total_cost << "\n";
}

int32_t main(){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    solve();
    return 0;
}
