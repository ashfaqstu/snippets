#include <bits/stdc++.h>
using namespace std;
#define int long long

class DSU {
public:
    vector<int> parent, sz;
    DSU(int n) {
        parent.resize(n);
        sz.assign(n,1);
        for(int i=0;i<n;i++) parent[i]=i;
    }
    int find(int x) {
        if(parent[x]==x) return x;
        return parent[x]=find(parent[x]);
    }
    bool unite(int a, int b) {
        a=find(a); b=find(b);
        if(a==b) return false;
        if(sz[a]<sz[b]) swap(a,b);
        parent[b]=a;
        sz[a]+=sz[b];
        return true;
    }
};

void solve() {
    int n, m;
    cin >> n >> m;
    
    vector<tuple<int,int,int>> edges;
    for(int i=0;i<m;i++){
        int u,v,w;
        cin >> u >> v >> w;
        u--; v--; // 0-based
        edges.push_back({w,u,v});
    }
    
    int must_u, must_v;
    cin >> must_u >> must_v; // 1-based mandatory edge
    must_u--; must_v--;

    DSU dsu(n);
    int total_weight=0;
    vector<pair<int,int>> mst_edges;

    // Step 1: Force include the mandatory edge
    for(int i=0;i<m;i++){
        auto [w,u,v]=edges[i];
        if((u==must_u && v==must_v) || (u==must_v && v==must_u)){
            dsu.unite(u,v);
            total_weight+=w;
            mst_edges.push_back({u,v});
            swap(edges[i], edges[m-1]); // Move it to the end to skip in Kruskal
            edges.pop_back();
            break;
        }
    }

    // Step 2: Kruskal on remaining edges
    sort(edges.begin(), edges.end());
    for(auto &[w,u,v]: edges){
        if(dsu.unite(u,v)){
            total_weight+=w;
            mst_edges.push_back({u,v});
        }
    }

    if(mst_edges.size()!=n-1){
        cout << "MST not possible" << endl;
    } else {
        cout << "Total weight: " << total_weight << endl;
        for(auto &[u,v]: mst_edges){
            cout << u+1 << " " << v+1 << endl;
        }
    }
}

int32_t main(){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    solve();
    return 0;
}
