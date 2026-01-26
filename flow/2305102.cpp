#include<bits/stdc++.h>
using namespace std;
class edge{
    public:
    int u;
    int v;
    int w;
    edge(int u,int v,int w){
        this->u=u;
        this->v=v;
        this->w=w;
    }
};
void bfs_for_ford_Fulkerson(vector<vector<int>> &residual_matrix,int s,int t,vector<int> &parent){
    vector<int> is_visited(residual_matrix.size(), 0);
    queue<int> q;
    q.push(s);
    is_visited[s]=1;
    while(!q.empty()){
        int u=q.front();
        q.pop();
        for(int v=0;v<residual_matrix.size();v++){
            if(is_visited[v]==0&&residual_matrix[u][v]>0){
                is_visited[v]=1;
                parent[v]=u;
                q.push(v);
                if(v==t){
                    return;
                }
            }
        }
    }

}
void augment_path(vector<vector<int>> &residual_graph,vector<int> &parent,int s,int t,int &max_flow,vector<vector<int>> &flow){
    int v=t;
    int min1=INT_MAX;
    while(v!=s){
        int u=parent[v];
        min1=min(min1,residual_graph[u][v]);
        v=u;
    }
    v=t;
    while(v!=s){
        int u=parent[v];
        residual_graph[u][v]-=min1;
        residual_graph[v][u]+=min1;
        flow[u][v]+=min1;
        // flow[v][u]-=min1;
        v=u;
    }
    max_flow+=min1;
}

int main(){
    int N,K,M;
    cin>>N>>K>>M;
    
    vector<vector<int>> capacity(N+2,vector<int>(N+2,0));
    vector<edge> edges;
    for(int i=0;i<M;i++){
        int u,v;
        cin>>u>>v;
        edges.push_back(edge(u,v,1));
        capacity[u][v]=1;
    }
    for(int i=0;i<K;i++){
        capacity[N][i]=1;
    }
    for(int i=K;i<N;i++){
        capacity[i][N+1]=1;
    }
    int s=N;
    int t=N+1;
    vector<vector<int>> residual_graph=capacity;;
    vector<vector<int>> flow(N+2,vector<int>(N+2,0));
    int max_flow=0;
    while(true){
        vector<int> parent(N+2,-1);
        bfs_for_ford_Fulkerson(residual_graph,s,t,parent);
        if(parent[t]==-1){
            break;
        }
        augment_path(residual_graph,parent,s,t,max_flow,flow);
    }
    cout<<max_flow<<endl;
    for(int i=0;i<edges.size();i++){
        int u=edges[i].u;
        int v=edges[i].v;
        if(flow[u][v]>0){
            cout<<u<<" "<<v<<endl;
        }
    }
        
}