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
    int a,m;
    cin>>a>>m;
    int n=a+m+3;
    int s=n-1;
    int t=n;
    vector<vector<int>> edge(n+1);
    vector<vector<int>> capacity(n+1, vector<int>(n+1,0));
    vector<tuple<int,int,int>> edge_order;

    for(int i=0;i<m;i++){
        int b;
        cin>>b;
        edge[b+a].push_back(t);
        edge[t].push_back(b+a);
        capacity[b+a][t]=1;
        while(cin.peek() != '\n' && cin.peek() != EOF){
            int id;
            cin>>id;
            edge[id].push_back(b+a);
            edge[b+a].push_back(id);
            capacity[id][b+a]=1;
        }
        
    }
    for(int i=1;i<=a;i++){
        edge[i].push_back(s);
        edge[s].push_back(i);
        capacity[s][i]=1;
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
    for(auto tup: edge_order){
        auto[u,v,w]=tup;
        cout<<u<<" "<<v<<" "<<w-capacity[u][v]<<"/"<<w<<endl;
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