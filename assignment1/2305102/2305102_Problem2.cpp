#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
#define int long long

vector<tuple<int,int,ll>> adj;



void solve(){
    int n,m;
    cin>>n>>m;
    
    for(int i=0;i<m;i++){
        int u,v;
        long long w;
        cin>>u>>v>>w;
        adj.push_back(make_tuple(u, v, w));
        
    }
    vector<int> dist(n+1, 0);    
    vector<int> p(n+1, 0);
    dist[1]=0;
    int x=-1;
    for(int i=0;i<n;i++){        
        for(auto edge: adj){
            auto[u,v,w]= edge;
            

            if( dist[u]+w<dist[v]){
                if(i==n-1) x=v;
                dist[v]=dist[u]+w;
                p[v]=u;
            }
        }
        
    }
    
    
    if(x==-1) cout<<-1;
    else{
        for(int i=0;i<n;i++){
            x=p[x];
        }
        int temp=x;
        cout<<temp<<" ";
        temp=p[x];
        while(temp!=x){
            
            cout<<temp<<" ";
            temp=p[temp];
            
        }
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