#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
#define int long long

class DSU {
public:
    vector<int> parent, sz;
    DSU(int n) {
        parent.resize(n);
        sz.assign(n, 1);
        for (int i = 0; i < n; i++) parent[i] = i;
    }
    int find(int i) {
        if (parent[i] == i) return i;
        return parent[i] = find(parent[i]);
    }
    bool unite(int i, int j) {
        int ri = find(i);
        int rj = find(j);
        if (ri == rj) return false;
        if (sz[ri] < sz[rj]) swap(ri, rj);
        parent[rj] = ri;
        sz[ri] += sz[rj];
        return true;
    }
};

void solve(){
    int n, m;
    cin >> n >> m;

    vector<tuple<int,int,int>> edges;
    for(int i = 0; i < m; i++){
        int u, v, w;
        cin >> u >> v >> w;
        u--; v--;                     // convert to 0-based
        edges.push_back({w, u, v});
    }

    vector<int> idx(m);
    iota(idx.begin(), idx.end(), 0);

    ll best = -1, second_best = -1;

    // Choose exactly n-1 edges
    vector<int> mask(m, 0);
    for(int i = 0; i < n - 1; i++) mask[i] = 1;

    do {
        DSU dsu(n);
        ll product = 1;
        int cnt = 0;
        bool ok = true;

        for(int i = 0; i < m; i++){
            if(mask[i]){
                auto [w, u, v] = edges[i];
                if(!dsu.unite(u, v)){
                    ok = false;
                    break;
                }
                product *= w;
                cnt++;
            }
        }

        if(!ok || cnt != n - 1) continue;

        // Check connectivity
        int root = dsu.find(0);
        for(int i = 1; i < n; i++){
            if(dsu.find(i) != root){
                ok = false;
                break;
            }
        }
        if(!ok) continue;

        if(best == -1 || product < best){
            second_best = best;
            best = product;
        }
        else if(product > best){
            if(second_best == -1 || product < second_best){
                second_best = product;
            }
        }

    } while(prev_permutation(mask.begin(), mask.end()));

    if(second_best == -1) cout << -1 << "\n";
    else cout << second_best << "\n";
}

int32_t main(){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    solve();
    return 0;
}
