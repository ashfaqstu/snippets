


#include <bits/stdc++.h>
using namespace std;

typedef pair<long long, int> pll;

vector<long long> kth_shortest_paths(int n, int k, int start, int target, vector<vector<pair<int, int>>>& adj) {
    // count[i] stores how many times node i has been extracted from the PQ
    vector<int> count(n + 1, 0);
    vector<long long> results;
    
    // Min-priority queue: {distance, node}
    priority_queue<pll, vector<pll>, greater<pll>> pq;
    
    pq.push({0, start});
    
    while (!pq.empty() && count[target] < k) {
        long long d = pq.top().first;
        int u = pq.top().second;
        pq.pop();
        
        count[u]++;
        
        // If we popped the target, we found one of the shortest paths
        if (u == target) {
            results.push_back(d);
        }
        
        // If we've already found k paths through this node, don't expand further
        if (count[u] > k) continue;
        
        for (auto& edge : adj[u]) {
            int v = edge.first;
            int weight = edge.second;
            pq.push({d + weight, v});
        }
    }
    
    return results; // Contains distances from 1st to k-th shortest
}

int main(){
    int n,m,k;
    cin>>n>>m>>k;
    
}