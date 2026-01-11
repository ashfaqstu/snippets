


#include <bits/stdc++.h>
using namespace std;

typedef pair<long long, int> pll;
vector<vector<pair<int, int>>> adj;
vector<long long> kth_shortest_paths(int n, int k, int start, int target) {
    
    vector<int> count(n + 1, 0);
    vector<long long> results;
    
    priority_queue<pll, vector<pll>, greater<pll>> pq;
    
    pq.push({0, start});
    
    while (!pq.empty() && count[target] < k) {
        long long d = pq.top().first;
        int u = pq.top().second;
        pq.pop();
        
        count[u]++;
        
       
        if (u == target) {
            results.push_back(d);
        }
        
        
        if (count[u] > k) continue;
        
        for (auto& edge : adj[u]) {
            int v = edge.first;
            int weight = edge.second;
            pq.push({d + weight, v});
        }
    }
    
    return results; 
}

int main(){
    int n,m,k;
    cin>>n>>m>>k;
    adj.resize(n+1);
    for(int i=0;i<m;i++){
        int u,v,w;
        cin>>u>>v>>w;
        adj[u].push_back({v,w});
    }
    auto ans=kth_shortest_paths(n,k,1,n);
    for(auto a: ans){
        cout<<a<<" ";
    }
}