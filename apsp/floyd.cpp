#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
#define int long long
#define INF 10e16
void floyd(int n, vector<vector<int>> &dist){
   for(int i=1;i<n+1;i++){
    for(int j=1;j<n+1;j++){
        for(int k=1;k<n+1;k++){
            if(dist[i][k] != INF && dist[j][i]!= INF)
            dist[j][k]=min(dist[j][k],dist[j][i]+dist[i][k]);
        }
    }
   }

}

void solve(){
    int n,m;
    cin>>n>>m;
    vector<vector <int>> dist(n+1,vector<int>(n+1,INF));
    for(int i=0;i<m;i++){
        int u,v,w;
        cin>>u>>v>>w;

        dist[u][v]=w;
    }
    for(int i=0; i<n;i++){
        dist[i+1][i+1]=0;
    }
    floyd(n,dist);
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            cout<< dist[i+1][j+1]<<" ";
        }
        cout<<endl;
    }
}
int32_t main(){
 
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    ll t=1;
    // cin>>t;
    while(t--){
        solve();
    }
    return 0;
}