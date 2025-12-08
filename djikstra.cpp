#include<bits/stdc++.h>
using namespace std;

vector<vector<pair<int,int>>> adj;

void djk(int src){
    int V = adj.size();
    priority_queue<pair<int,int>, vector<pair<int,int>>,greater<pair<int,int>>>pq;
    vector<int> dist(V,INT16_MAX);
    dist[src]=0;
    pq.emplace(0,src);
    while(!pq.empty()){
        auto top=pq.top();
        pq.pop();
        int d= top.second;
        int u= top.first;

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
    


}
int main(){

}