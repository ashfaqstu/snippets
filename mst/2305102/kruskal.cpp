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
    int n, m;
    if (!(cin >> n >> m)) return;

    vector<tuple<int, int, int>> edges;
    for (int i = 0;i<m; i++) {
        int u,v,w;
        cin >>u>>v>>w;
        edges.push_back({w, u, v});
    }
    sort(edges.begin(), edges.end());
    DSU dsu(n);
    int total_weight=0;
    vector<pair<int,int>> mst_edges;
    for(auto &ed:edges){
        auto [w,u,v ]= ed;
        if(dsu.unite(u,v)) {
            total_weight+= w;
            mst_edges.push_back({u, v});
        }
    }
    if (mst_edges.size() != n - 1) {
        cout<<"not connecteed"<< endl;
    } else {
        cout << "Total weight "<<total_weight << endl;
        for (auto&edge:mst_edges) {
            cout<<edge.first<<" "<< edge.second << endl;
        }
    }

}
int32_t main(){
 
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    solve();
    return 0;
}