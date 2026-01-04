#include<bits/stdc++.h>

using namespace std;

vector<vector<int>> adj;
void vis_bfs(vector<vector<int>> &vis,int i,int j){
    if(adj[i+1][j]==0 && vis[i+1][j]==0){
        vis[i+1][j]=1;
        vis_bfs(vis,i+1,j);

    }
    if(adj[i-1][j]==0 && vis[i-1][j]==0){
        vis[i-1][j]=1;
        vis_bfs(vis,i-1,j);

    }
    if(adj[i][j+1]==0 && vis[i][j+1]==0){
        vis[i][j+1]=1;
        vis_bfs(vis,i,j+1);

    }
    if(adj[i][j-1]==0 && vis[i][j-1]==0){
        vis[i][j-1]=1;
        vis_bfs(vis,i,j-1);

    }
}
void solve(){
    int n,m;
    cin>>n>>m;
    adj.resize(n);
    vector<vector<int>> vis(n,vector<int>(m));
    
    for(int i=0;i<n;i++){
        for(int j=0;j<m;j++){
            int t;
            cin>>t;
            
            adj[i].push_back(t);
            vis[i][j]=0;
        }
    }
    // for(int i=0;i<n;i++){
    //     for(int j=0;j<m;j++){
            
    //         cout<<adj[i][j];
    //     }
    // }
    int ans=0;
    for(int i=1;i<n-1;i++){
        for(int j=1;j<m-1;j++){
            if(adj[i][j]==0 && vis[i][j]==0){
                ans++;
                vis[i][j]==1;
                vis_bfs(vis,i,j);
            }
            
        }
    }
    cout<<ans;
    



}

int main(){
    solve();
}
