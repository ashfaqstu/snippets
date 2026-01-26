#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
#define int long long
#define inf 10e9

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
    int m,n,k;
    cin>>n>>k>>m;
    vector<vector<int>> edge(n+2);
    vector<vector<int>> capacity(n+2, vector<int>(n+2,0));
    vector<pair<int,int>> edge_order;

    for(int i=0;i<m;i++){
        int a,b;
        cin>>a>>b;
        edge_order.push_back({a,b});
        edge[a].push_back(b);
        edge[b].push_back(a);
        capacity[a][b]=1;  
    }

    for(int i=0;i<n;i++){
        if(i>=k)
        {edge[i].push_back(n+1);
        edge[n+1].push_back(i);
         capacity[i][n+1]=1;}
        else{
            edge[n].push_back(i);
            edge[i].push_back(n);
            capacity[n][i]=1;
        }
    }
    int s,t;
    s=n;
    t=n+1;
  
    int flow=0;
    int new_flow;
    vector<int> parent(n+2);
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
    for(auto tup: edge_order){
        auto[u,v]=tup;
        if(capacity[v][u])cout<<u<<" "<<v<<" "<<endl;
    }

    
}
int32_t main(){
 
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    ll t;
    t=1;
    while(t--){
        solve();
    }
    return 0;
}