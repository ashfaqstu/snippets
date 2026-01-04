#include<bits/stdc++.h>
using namespace std;

vector<vector<pair<int,int>>> adj;

void djk(int src){
    int V = adj.size();
    priority_queue<pair<int,int>, vector<pair<int,int>>,greater<pair<int,int>>>pq;
    vector<int> dist(V+1,INT16_MAX);
    dist[src]=0;
    pq.emplace(0,src);
    while(!pq.empty()){
        auto [d,u]=pq.top();
        pq.pop();
        

        if(d>dist[u]) continue;
        for(auto &p: adj[u]){
            int v= p.first;
            int d=p.second;

            if( d+dist[u]<dist[v]){
                dist[v]=d+dist[u];
                pq.emplace(v,dist[v]);
            }
        }
    }
    
    for(auto v:dist){
        cout<<v<<" ";
    }

}
int main(){
    int n,m;
    cin>>n>>m;
    adj.resize(n+1);
    for(int i=0;i<m;i++){
        int u,v,w;
        cin>>u>>v>>w;
        adj[u].push_back({v,w});
    }
    djk(1);

}