#include<bits/stdc++.h>

using namespace std;
int flag=0;
vector<vector<int>> adj;

void dfs(vector<int>&vis,int u,stack<int> &s){
    if(vis[u]==2) return;
    if(vis[u]==1) {
        flag=1;
        return;
    }
    vis[u]=1;
    for(int i=0;i<adj[u].size();i++){
        dfs(vis,adj[u][i],s);
    }
    s.push(u);
    vis[u]=2;
}

void solve(){
    int n,m;
    cin>>n>>m;
    adj.resize(n+1);
    stack<int>s;
    vector<int> vis(n+1,0);
    for(int i=0;i<m;i++){
        int u,v;
        cin>>u>>v;
        adj[u].push_back(v);
    }
    for(int i=1;i<n+1;i++){
        if(vis[i]==0){
            dfs(vis,i,s);
        }
    }
    if(flag==1){
        cout<<-1;
        return;
    }
    while(!s.empty()){
        int k=s.top();
        s.pop();
        cout<<k<<" ";
    }
    



}

int main(){
    solve();
}
