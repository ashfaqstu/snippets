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
    int m,n;
    cin>>n>>m;
    vector<vector<int>> edge(n+1);
    vector<vector<int>> capacity(n+1, vector<int>(n+1,0));
    vector<tuple<int,int,int>> edge_order;

    for(int i=0;i<m;i++){
        int a,b,c=1;
        cin>>a>>b;
        edge_order.push_back(make_tuple(a,b,c));
        edge[a].push_back(b);
        edge[b].push_back(a);
        capacity[a][b]=c;
        
    }
    int s,t;
    s=1;
    t=n;
  
    int flow=0;
    int new_flow;
    vector<int> parent(n+1);
    vector<stack<int>> path;
    while(new_flow= bfs(s,t, parent,edge,capacity)){
        flow+=new_flow;
        int cur=t;
        stack<int> temp;
        while(cur!=s){
            capacity[cur][parent[cur]]+=new_flow;
            capacity[parent[cur]][cur]-=new_flow;
            temp.push(cur);
            cur=parent[cur];
            
        }
        temp.push(cur);
        path.push_back(temp);
    }

    cout<<flow<<endl;
    // for(auto tup: edge_order){
    //     auto[u,v,w]=tup;
    //     cout<<u<<" "<<v<<" "<<w-capacity[u][v]<<"/"<<w<<endl;
    // }
    for(auto paths: path){
        while(!paths.empty()){
            int temp= paths.top();
            cout<< temp<<" ";
            paths.pop();
        }
        cout<<endl;
    }
    
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