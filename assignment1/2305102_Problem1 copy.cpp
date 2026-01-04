#include<bits/stdc++.h>
using namespace std;


vector<vector<pair<int,long long>>> adj;

void djk(int src,int end){
    int V = adj.size();
    priority_queue<tuple<long long,int,int>, vector<tuple<long long,int,int>>,greater<tuple<long long,int,int>>>pq;
    vector<long long> dist(V+1,10e17);
    vector<long long> dist1(V+1,10e17);
    dist[src]=0;
    dist1[src]=0;
    pq.emplace(0,src,0);
    while(!pq.empty()){
       
        auto[di,u,s]=pq.top();
        pq.pop();
        
        
        if (s== 0){
            if (di>dist[u]) continue;
        } 
        else{
            if (di>dist1[u]) continue;
        }
   
        for(pair<int,long long> &p: adj[u]){
            
            int v= p.first;
            long long d=p.second;
            
            if(s==0){
                if( d+dist[u]<dist[v]){
                    dist[v]=d+dist[u];
                    pq.emplace(dist[v],v,0);
                    
                }
                if(d/2+dist[u]<dist1[v]){
                    dist1[v]=d/2+dist[u];
                    pq.emplace(dist1[v],v,1);
                }
            }
            else{
                if( d+dist1[u]<dist1[v]){
                    dist1[v]=d+dist1[u];
                    pq.emplace(dist1[v],v,s);
                }
            }
        }
        
        
        
    }
    cout<<dist1[end];
    


}
int main(){
    int n,m;
    cin>>n>>m;
    adj.resize(n+1);
    for(int i=0;i<m;i++){
        int u,v;
        long long w;
        cin>>u>>v>>w;
        adj[u].push_back({v,w});
        //cout<<adj[u][adj[u].size()-1].first<<endl;;
    }
    djk(1,n);

}