#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
#define int long long
#define inf 10e9
const double EPS = 1e-9;

int bfs(int s,int t, vector<int> &parent,vector<vector<int>>& edge, vector<vector<int>> &cap){
    fill(parent.begin(),parent.end(),-1);
    parent[s]=-2;
    
    queue<pair<int,int >> q;
    q.push({s,inf});
    while(!q.empty()){

        int cur= q.front().first;
        int flow= q.front().second;
        q.pop();
        
        for(auto ed: edge[cur]){
            if(parent[ed]==-1 && cap[cur][ed]!=0){
                parent[ed]= cur;
                int new_flow= min(flow,cap[cur][ed]);
                if(ed==t) return new_flow;
                q.push({ed,new_flow});
                
            }
        }
    }
   
    return 0;




}

void solve(){
    int m,h;
    float r;
    cin>>m>>h>>r;
    int n=m+h+2;
    int s= n-1;
    int t=n;
    
    vector<vector<int>> edge(n+1);
    vector<vector<int>> capacity(n+1, vector<int>(n+1,0));
    vector<tuple<int,int,int>> edge_order;
    vector<pair<float,float>> mouse;
    for(int i=0;i<m;i++){
        float u,v;
        cin>>u>>v;
        mouse.push_back({u,v});
        edge[i].push_back(s);
        edge[s].push_back(i);
        capacity[s][i]=1;
    }
    vector<pair<float,float>>hole;
    for(int i=0;i<h;i++){
        float u,v;
        int cap;
        cin>>u>>v>>cap;
        hole.push_back({u,v});
        edge[i+m].push_back(t);
        edge[t].push_back(i+m);
        capacity[i+m][t]=cap;
    }
    
    for(int i=0;i<m;i++){
        for(int j=0;j<h;j++){
            if(abs(pow((mouse[i].first-hole[j].first),2)+pow((mouse[i].second-hole[j].second),2))<=r*r){
                edge[i].push_back(j+m);
                edge[j+m].push_back(i);
                capacity[i][j+m]=1;
            }
        }
        
    }
    
    
    
    int flow=0;
    int new_flow;
    vector<int> parent(n+1);
    while(new_flow= bfs(s,t, parent,edge,capacity)){
        flow+=new_flow;
        int cur=t;
        while(cur!=s){
            capacity[cur][parent[cur]]+=new_flow;
            capacity[parent[cur]][cur]-=new_flow;
            cur=parent[cur];
            
        }
    }

    cout<<flow<<endl;
    // for(auto tup: edge_order){
    //     auto[u,v,w]=tup;
    //     cout<<u<<" "<<v<<" "<<w-capacity[u][v]<<"/"<<w<<endl;
    // }

    
}
int32_t main(){
 
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    ll t;
    cin>>t;
    while(t--){
        solve();
    }
    return 0;
}