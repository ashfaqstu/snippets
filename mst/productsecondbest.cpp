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

// Kruskal's MST product function, optionally skipping one edge
int64_t kruskal(int n, vector<tuple<int,int,int>> &edges, int skip_idx, vector<int> &used_edges) {
    DSU dsu(n);
    int count=0;
    int64_t prod=1;
    for (int i=0;i<edges.size();i++){
        if (i==skip_idx) continue;
        auto [w,u,v]=edges[i];
        if(dsu.unite(u,v)){
            prod *= w;
            used_edges.push_back(i);
            count++;
        }
        if(count==n-1) break;
    }
    if(count < n-1) return -1; // not connected
    return prod;
}

void solve(){
    int n, m;
    if (!(cin >> n >> m)) return;

    vector<tuple<int,int,int>> edges;
    for (int i=0;i<m; i++) {
        int u,v,w;
        cin >> u >> v >> w;
        u--; v--; // 0-indexed
        edges.push_back({w, u, v});
    }

    // Sort edges by weight (for Kruskal)
    sort(edges.begin(), edges.end());

    // Step 1: Primary MST
    vector<int> primary_used;
    int64_t primary_prod = kruskal(n, edges, -1, primary_used);

    if(primary_prod == -1){
        cout << -1 << "\n";
        return;
    }

    // Step 2: Try skipping each edge in primary MST to find second-best MST
    int64_t second_best = LLONG_MAX;
    for(auto skip_idx: primary_used){
        vector<int> temp_used;
        int64_t prod = kruskal(n, edges, skip_idx, temp_used);
        if(prod > primary_prod && prod < second_best){
            second_best = prod;
        }
    }

    if(second_best == LLONG_MAX) cout << -1 << "\n";
    else cout << second_best << "\n";
}

int32_t main(){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    solve();
    return 0;
}
