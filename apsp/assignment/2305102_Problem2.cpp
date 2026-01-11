#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
// #define int long long
// #define INF 10e16
const double EPS = 1e-9;
void floyd(int n, vector<vector<double>> &dist){
    for(int k=1; k<=n;k++)
        for (int i =1; i<=n; i++)
            for (int j =1;j<=n;j++)
                dist[i][j] =max(dist[i][j], dist[i][k]*dist[k][j]);

}

void solve(){
    int n,m;
    cin>>n;
    unordered_map<string,int> name;
    for(int i=0;i<n;i++){
        string ab;
        cin>>ab;
        name[ab] =i+1;
    }

    vector<vector <double>> dist(n+1,vector<double>(n+1,0.0));
    cin>>m;
    for(int i=0; i<n;i++){
        dist[i+1][i+1]=1.0;
    }
    for(int i=0;i<m;i++){
        string u,v;
        double w;
        cin>>u>>w>>v;

        dist[name[u]][name[v]] = max(dist[name[u]][name[v]], w);
    }
    
    floyd(n,dist);
     
    for(int i=1;i<=n;i++){
       
        if(dist[i][i]>1.0 +EPS){
            cout<<"Yes"; 
            return;}
        
    }
    cout<<"No";
    
}
int main(){
 
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    ll t=1;
    // cin>>t
    
    while(t--){
        solve();
    }
    return 0;
}