#include <bits/stdc++.h>

using namespace std;

vector<long long> dijkstra(int src, vector<vector<pair<int, int>>> &adj, int n)
{
    priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<pair<long long, int>>> pq;

    vector<long long> cost(n+1, 1e18);
    cost[src] = 0;

    pq.push({cost[src], src});

    while (!pq.empty())
    {
        int u = pq.top().second;
        int dis=pq.top().first;
        pq.pop();
        //if(dis>cost[u]) continue;
        for(auto v: adj[u])
        {
            int edge_cost = v.first;

            if(cost[u]+edge_cost < cost[v.second])
            {
                cost[v.second] = cost[u] +edge_cost;
                pq.push({cost[v.second], v.second});
            }

        }

    }

    return cost;

}

int main()
{
    int n, m;
    cin >> n >> m;

    vector<vector<pair<int, int>>> adj(n+1);
    vector<vector<pair<int, int>>> reverse_adj(n+1);

    int a, b, c;

    for (int i = 0; i < m; i++)
    {
        cin >> a >> b >> c;
        adj[a].push_back({c, b}); // b node, c for cost
        reverse_adj[b].push_back({c, a});
    }

    vector<long long> cost_src = dijkstra(1, adj, n);
    vector<long long> cost_destination = dijkstra(n, reverse_adj, n);

    //halve each edge

    long long mincost = 1e18;

    for (int u = 1; u <= n; u++)
    {
        for(auto it: adj[u])
        {
            int v = it.second;

            mincost = min(mincost, cost_src[u] + (it.first/2) + cost_destination[v]);

        }
    }

    cout << mincost;


    return 0;
}