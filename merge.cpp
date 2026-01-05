
//dijkstra algorithm

#include <bits/stdc++.h>
using namespace std;

using ll = long long;
const ll INF = 1e18;

int n, m;
vector<vector<pair<int,int>>> adj;
vector<ll> dist;

void dijkstra(int src) {
    dist.assign(n + 1, INF);

    priority_queue<pair<ll,int>,vector<pair<ll,int>>,greater<pair<ll,int>>> pq;

    dist[src] = 0;
    pq.push({0, src});

    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();

        if (d > dist[u]) continue;

        for (auto [v, w] : adj[u]) {
            if (dist[v] > d + w) {
                dist[v] = d + w;
                pq.push({dist[v], v});
            }
        }
    }
}
//bell
#include <bits/stdc++.h>
using namespace std;



const long long INF = 1e16; // Use 1e16 to prevent overflow when adding weights

struct Edge {
    int u, v;
    long long w;
};

struct BellmanFordResult {
    vector<long long> dist;
    bool hasNegativeCycle;
};

BellmanFordResult bellman_ford(int n, const vector<Edge>& edges, const vector<int>& sources) {
    vector<long long> dist(n + 1, INF);
    bool negativeCycle = false;

    // Multi-source initialization
    for (int src : sources) {
        dist[src] = 0;
    }

    // Relax all edges V-1 times
    for (int i = 1; i <= n - 1; ++i) {
        bool changed = false;
        for (const auto& e : edges) {
            if (dist[e.u] != INF && dist[e.u] + e.w < dist[e.v]) {
                dist[e.v] = dist[e.u] + e.w;
                changed = true;
            }
        }
        // Early Exit Optimization: If no distance changes, we are done
        if (!changed) break;
    }

    // Final pass to detect negative cycles
    // If we can still relax an edge, a negative cycle exists
    for (const auto& e : edges) {
        if (dist[e.u] != INF && dist[e.u] + e.w < dist[e.v]) {
            negativeCycle = true;
            break;
        }
    }

    return {dist, negativeCycle};
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;
    vector<Edge> edges(m);
    for (int i = 0; i < m; ++i) {
        cin >> edges[i].u >> edges[i].v >> edges[i].w;
    }

    // For single source, just pass {start_node}
    vector<int> sources = {1}; 
    BellmanFordResult res = bellman_ford(n, edges, sources);

    if (res.hasNegativeCycle) {
        cout << "Negative Cycle Detected" << endl;
    } else {
        for (int i = 1; i <= n; ++i) {
            if (res.dist[i] == INF) cout << "INF ";
            else cout << res.dist[i] << " ";
        }
        cout << endl;
    }

    return 0;
}


//with path prining

#include <bits/stdc++.h>
using namespace std;

using ll = long long;
const ll INF = 1e18;

int n, m;
vector<vector<pair<int, int>>> adj;
vector<ll> dist;
vector<int> parent; // Moved to global or could be passed by reference

void dijkstra(int src) {
    dist.assign(n + 1, INF);
    parent.assign(n + 1, -1);
    
    // Min-priority queue: {distance, node}
    priority_queue<pair<ll, int>, vector<pair<ll, int>>, greater<pair<ll, int>>> pq;

    dist[src] = 0;
    pq.push({0, src});

    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();

        // Standard Dijkstra optimization
        if (d > dist[u]) continue;

        for (auto [v, w] : adj[u]) {
            if (dist[v] > dist[u] + w) {
                dist[v] = dist[u] + w;
                parent[v] = u;
                pq.push({dist[v], v});
            }
        }
    }

    // Path reconstruction for node 1 to node n
    if (dist[n] == INF) {
        cout << -1 << endl;
        return;
    }

    vector<int> path;
    for (int v = n; v != -1; v = parent[v]) {
        path.push_back(v);
    }
    reverse(path.begin(), path.end());

    for (int i = 0; i < path.size(); i++) {
        cout << path[i] << (i == path.size() - 1 ? "" : " ");
    }
    cout << endl;
}

int main() {
    ios_base::sync_with_stdio(false); // Speed up I/O
    cin.tie(NULL);

    if (!(cin >> n >> m)) return 0;

    adj.assign(n + 1, vector<pair<int, int>>());
    
    for (int i = 0; i < m; i++) {
        int u, v, w;
        cin >> u >> v >> w;
        adj[u].push_back({v, w});
        adj[v].push_back({u, w}); // Assuming undirected graph
    }

    dijkstra(1);

    return 0;
}





//chess problem

#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
const ll INF = 1e18;

int dr[] = {-2, -2, -1, -1, 1, 1, 2, 2};
int dc[] = {-1, 1, -2, 2, -2, 2, -1, 1};

void solve(int startX, int startY, int endX, int endY) {
    
    vector<vector<ll>> dist(8, vector<ll>(8, INF));
    

    priority_queue<pair<ll, pair<int, int>>, 
                   vector<pair<ll, pair<int, int>>>, 
                   greater<pair<ll, pair<int, int>>>> pq;

    dist[startX][startY] = 0;
    pq.push({0, {startX, startY}});

    while (!pq.empty()) {
        ll d = pq.top().first;
        int r = pq.top().second.first;
        int c = pq.top().second.second;
        pq.pop();

        
        if (d > dist[r][c]) continue;
        
       
        if (r == endX && c == endY) break;

       
        for (int i = 0; i < 8; i++) {
            int nr = r + dr[i];
            int nc = c + dc[i];

            
            if (nr >= 0 && nr < 8 && nc >= 0 && nc < 8) {
                
                ll weight = (ll)r * nr + (ll)c * nc;
                
                if (dist[nr][nc] > d + weight) {
                    dist[nr][nc] = d + weight;
                    pq.push({dist[nr][nc], {nr, nc}});
                }
            }
        }
    }

    if (dist[endX][endY] == INF) {
        cout << -1 << endl;
    } else {
        cout << dist[endX][endY] << endl;
    }
}

int main() {
    
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int a, b, c, d;
    
    while (cin >> a >> b >> c >> d) {
        solve(a, b, c, d);
    }

    return 0;
}




//tution lord


#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
const ll INF = 1e18;

struct Edge {
    int to;
    int time;
    int cost;
};

void solve() {
    ll K;
    int N, X;
    if (!(cin >> K >> N >> X)) return;

    vector<vector<Edge>> adj(N + 1);
    for (int i = 0; i < X; i++) {
        int u, v, t, c;
        cin >> u >> v >> t >> c;
        // Roads are bidirectional
        adj[u].push_back({v, t, c});
        adj[v].push_back({u, t, c});
    }

    int S, D;
    cin >> S >> D;

    // dist[i] stores the minimum Total Cost to reach city i
    vector<ll> dist(N + 1, INF);
    vector<int> parent(N + 1, -1);
    vector<int> time_at_node(N + 1, 0);

    // Min-priority queue: {current_cost, current_node}
    priority_queue<pair<ll, int>, vector<pair<ll, int>>, greater<pair<ll, int>>> pq;

    dist[S] = 0;
    pq.push({0, S});

    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();

        if (d > dist[u]) continue;
        if (u == D) break;

        for (auto& edge : adj[u]) {
            // Formula: Travel Cost + Opportunity Cost of (Travel Time + 1 hour wait)
            // We subtract the extra K at the very end of the algorithm
            ll edge_weight = (ll)edge.cost + K * (edge.time + 1);
            
            if (dist[edge.to] > dist[u] + edge_weight) {
                dist[edge.to] = dist[u] + edge_weight;
                parent[edge.to] = u;
                pq.push({dist[edge.to], edge.to});
            }
        }
    }

    if (dist[D] == INF) {
        cout << "Error" << endl;
        return;
    }

    // Path Reconstruction
    vector<int> path;
    for (int v = D; v != -1; v = parent[v]) {
        path.push_back(v);
    }
    reverse(path.begin(), path.end());

    // Calculate Total Time and Total Cost
    ll total_time = 0;
    ll total_travel_cost = 0;
    for (int i = 0; i < (int)path.size() - 1; i++) {
        int u = path[i];
        int v = path[i+1];
        for (auto& e : adj[u]) {
            if (e.to == v) {
                total_time += e.time;
                total_travel_cost += e.cost;
                break;
            }
        }
    }
    
    // Add 1 hour for each transfer (number of nodes - 2)
    if (path.size() > 1) {
        total_time += (path.size() - 2);
    }

    ll final_cost = total_travel_cost + (K * total_time);

    // Output Formatting: S->[C1->...]->D T C
    for (int i = 0; i < path.size(); i++) {
        cout << path[i] << (i == path.size() - 1 ? "" : "->");
    }
    cout << " " << total_time << " " << final_cost << endl;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    solve();
    return 0;
}




//magic


#include<bits/stdc++.h>
using namespace std;

const long long INF = 1e16; // 1e18 can sometimes cause overflow when adding weights

struct Edge {
    int u, v;
    long long weight;
};

void solve() {
    int n, m, k, b, e_count;
    if (!(cin >> n >> m >> k >> b >> e_count)) return;

    vector<int> capitals(k);
    for (int i = 0; i < k; i++) cin >> capitals[i];

    vector<int> blk(n + 1, 0);
    for (int i = 0; i < b; i++) {
        int tmp;
        cin >> tmp;
        blk[tmp] = 1;
    }

    vector<int> emergency_posts(e_count);
    for (int i = 0; i < e_count; i++) cin >> emergency_posts[i];

    vector<Edge> edges;
    for (int i = 0; i < m; i++) {
        int u, v;
        long long w;
        cin >> u >> v >> w;
        // Only consider edges where neither city is blocked
        if (!blk[u] && !blk[v]) {
            edges.push_back({u, v, w});
        }
    }

    // Single distance vector for all capitals
    vector<long long> dist(n + 1, INF);
    
    // MULTI-SOURCE INITIALIZATION
    for (int cap : capitals) {
        if (!blk[cap]) dist[cap] = 0;
    }

    // Bellman-Ford: Relax edges N-1 times
    for (int i = 1; i < n; i++) {
        bool changed = false;
        for (const auto& edge : edges) {
            if (dist[edge.u] != INF && dist[edge.u] + edge.weight < dist[edge.v]) {
                dist[edge.v] = dist[edge.u] + edge.weight;
                changed = true;
            }
        }
        if (!changed) break; 
    }

    // Negative cycle detection
    for (const auto& edge : edges) {
        if (dist[edge.u] != INF && dist[edge.u] + edge.weight < dist[edge.v]) {
            cout << "Abyss Detected" << endl;
            return;
        }
    }

    // Output formatted correctly
    for (int post : emergency_posts) {
        if (blk[post] || dist[post] >= (INF / 2)) {
            cout << "INF" << endl;
        } else {
            cout << dist[post] << endl;
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    solve();
    return 0;
}


//kth shortest path djikstra

#include <bits/stdc++.h>
using namespace std;

typedef pair<long long, int> pll;

vector<long long> kth_shortest_paths(int n, int k, int start, int target, vector<vector<pair<int, int>>>& adj) {
    // count[i] stores how many times node i has been extracted from the PQ
    vector<int> count(n + 1, 0);
    vector<long long> results;
    
    // Min-priority queue: {distance, node}
    priority_queue<pll, vector<pll>, greater<pll>> pq;
    
    pq.push({0, start});
    
    while (!pq.empty() && count[target] < k) {
        long long d = pq.top().first;
        int u = pq.top().second;
        pq.pop();
        
        count[u]++;
        
        // If we popped the target, we found one of the shortest paths
        if (u == target) {
            results.push_back(d);
        }
        
        // If we've already found k paths through this node, don't expand further
        if (count[u] > k) continue;
        
        for (auto& edge : adj[u]) {
            int v = edge.first;
            int weight = edge.second;
            pq.push({d + weight, v});
        }
    }
    
    return results; // Contains distances from 1st to k-th shortest
}

//k edges

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

// Use a large INF that won't overflow when adding weights
const long long INF = 1e16; 

struct Edge {
    int u, v;
    long long w;
};

void solve() {
    int n, m, k_edges, src, dest;
    if (!(cin >> n >> m >> k_edges >> src >> dest)) return;

    vector<Edge> edges;
    for (int i = 0; i < m; ++i) {
        int u, v;
        long long w;
        cin >> u >> v >> w;
        edges.push_back({u, v, w});
    }

    // dp[i][v] = min distance to node v using exactly i edges
    // We use n + 1 to handle 1-based indexing of cities
    vector<vector<long long>> dp(k_edges + 1, vector<long long>(n + 1, INF));

    // Base Case: 0 edges used
    dp[0][src] = 0;

    // Transition: Iterate through each hop count from 1 to K
    for (int i = 1; i <= k_edges; ++i) {
        for (const auto& edge : edges) {
            // If the start node of this edge was reachable in (i-1) steps
            if (dp[i - 1][edge.u] != INF) {
                // Update the destination node for exactly i steps
                dp[i][edge.v] = min(dp[i][edge.v], dp[i - 1][edge.u] + edge.w);
            }
        }
    }

    // Output result
    if (dp[k_edges][dest] >= INF / 2) {
        cout << "No path found with exactly " << k_edges << " edges." << endl;
    } else {
        cout << "Shortest distance with exactly " << k_edges << " edges: " << dp[k_edges][dest] << endl;
    }
}

int main() {
    // Optimization for fast I/O
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    solve();
    return 0;
}



//dsa lab

#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

const long long INF = 1e18;

struct Edge {
    int to;
    long long weight;
};

struct Lab {
    int id;
    long long total_cost;
    int capacity;
};

// Comparator to sort labs by total cost
bool compareLabs(const Lab& a, const Lab& b) {
    if (a.total_cost != b.total_cost)
        return a.total_cost < b.total_cost;
    return a.id < b.id;
}

void solve() {
    int n, m;
    long long f;
    if (!(cin >> n >> m >> f)) return;

    vector<int> capacities(n + 1);
    for (int i = 1; i <= n; i++) cin >> capacities[i];

    vector<vector<Edge>> adj(n + 1);
    for (int i = 0; i < m; i++) {
        int u, v;
        long long w;
        cin >> u >> v >> w;
        adj[u].push_back({v, w});
        adj[v].push_back({u, w});
    }

    int k_students;
    cin >> k_students;

    // --- Phase 1: Dijkstra from Lab 1 ---
    vector<long long> dist(n + 1, INF);
    priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<pair<long long, int>>> pq;

    dist[1] = 0;
    pq.push({0, 1});

    while (!pq.empty()) {
        long long d = pq.top().first;
        int u = pq.top().second;
        pq.pop();

        if (d > dist[u]) continue;

        for (auto& edge : adj[u]) {
            if (dist[u] + edge.weight < dist[edge.to]) {
                dist[edge.to] = dist[u] + edge.weight;
                pq.push({dist[edge.to], edge.to});
            }
        }
    }

    // --- Phase 2: Collect and Sort Labs by Cost ---
    vector<Lab> available_labs;
    for (int i = 1; i <= n; i++) {
        if (dist[i] != INF) {
            available_labs.push_back({i, dist[i] + f, capacities[i]});
        }
    }
    sort(available_labs.begin(), available_labs.end(), compareLabs);

    // --- Phase 3: Allocate Students ---
    vector<long long> results;
    int current_lab_idx = 0;

    for (int s = 0; s < k_students; s++) {
        // Move to the next lab if the current one is full
        while (current_lab_idx < available_labs.size() && available_labs[current_lab_idx].capacity == 0) {
            current_lab_idx++;
        }

        if (current_lab_idx < available_labs.size()) {
            results.push_back(available_labs[current_lab_idx].total_cost);
            available_labs[current_lab_idx].capacity--;
        } else {
            results.push_back(-1);
        }
    }

    // Output formatted results
    for (int i = 0; i < results.size(); i++) {
        cout << results[i] << (i == results.size() - 1 ? "" : " ");
    }
    cout << endl;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    solve();
    return 0;
}


//tax

#include <iostream>
#include <vector>
#include <queue>

using namespace std;

const long long INF = 1e18;

struct Edge {
    int to;
};

void solve() {
    int n, m;
    if (!(cin >> n >> m)) return;

    // Read taxes for each city
    vector<long long> tax(n + 1);
    for (int i = 1; i <= n; i++) {
        cin >> tax[i];
    }

    // Build adjacency list (roads are bidirectional)
    vector<vector<int>> adj(n + 1);
    for (int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    // --- Dijkstra's Algorithm ---
    vector<long long> dist(n + 1, INF);
    // Min-priority queue: {total_tax, current_city}
    priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<pair<long long, int>>> pq;

    // Starting at City 1: Initial cost is the tax of City 1
    dist[1] = tax[1];
    pq.push({dist[1], 1});

    while (!pq.empty()) {
        long long d = pq.top().first;
        int u = pq.top().second;
        pq.pop();

        // Standard Dijkstra optimization
        if (d > dist[u]) continue;

        // If we reached the destination, we can stop early
        if (u == n) break;

        for (int v : adj[u]) {
            // The cost to move to city 'v' is the tax of city 'v'
            if (dist[u] + tax[v] < dist[v]) {
                dist[v] = dist[u] + tax[v];
                pq.push({dist[v], v});
            }
        }
    }

    // Output the minimum tax to reach City N
    if (dist[n] == INF) cout << -1 << endl;
    else cout << dist[n] << endl;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    solve();
    return 0;
}



//friend visit


#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

const long long INF = 1e18;

struct Edge {
    int to;
    int weight;
};

// Dijkstra function that returns distances and parent pointers for path reconstruction
void dijkstra(int n, int start, const vector<vector<Edge>>& adj, vector<long long>& dist, vector<int>& parent) {
    dist.assign(n, INF);
    parent.assign(n, -1);
    dist[start] = 0;
    priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<pair<long long, int>>> pq;
    pq.push({0, start});

    while (!pq.empty()) {
        long long d = pq.top().first;
        int u = pq.top().second;
        pq.pop();

        if (d > dist[u]) continue;

        for (auto& edge : adj[u]) {
            if (dist[u] + edge.weight < dist[edge.to]) {
                dist[edge.to] = dist[u] + edge.weight;
                parent[edge.to] = u;
                pq.push({dist[edge.to], edge.to});
            }
        }
    }
}

// Helper to reconstruct path from parent pointers
vector<int> getPath(int target, const vector<int>& parent) {
    vector<int> path;
    for (int v = target; v != -1; v = parent[v]) {
        path.push_back(v);
    }
    reverse(path.begin(), path.end());
    return path;
}

int main() {
    int s, r;
    if (!(cin >> s >> r)) return 0;

    vector<vector<Edge>> adj(s), rev_adj(s);
    for (int i = 0; i < r; i++) {
        int u, v, t;
        cin >> u >> v >> t;
        adj[u].push_back({v, t});
        rev_adj[v].push_back({u, t});
    }

    int f;
    cin >> f;

    vector<long long> distForward, distBackward;
    vector<int> parentForward, parentBackward;

    // Run Dijkstra on original and reversed graph
    dijkstra(s, f, adj, distForward, parentForward);
    dijkstra(s, f, rev_adj, distBackward, parentBackward);

    long long minTotalTime = INF;
    int bestFriendState = -1;

    // Find state v (v != f) that minimizes distForward[v] + distBackward[v]
    for (int i = 0; i < s; i++) {
        if (i == f) continue;
        if (distForward[i] != INF && distBackward[i] != INF) {
            long long total = distForward[i] + distBackward[i];
            if (total < minTotalTime) {
                minTotalTime = total;
                bestFriendState = i;
            }
        }
    }

    // Output formatting
    cout << "Minimum total time: " << minTotalTime << endl;
    cout << "Paths:" << endl;

    // Forward Path: f -> ... -> bestFriendState
    vector<int> path1 = getPath(bestFriendState, parentForward);
    for (int i = 0; i < path1.size(); i++) {
        cout << path1[i] << (i == path1.size() - 1 ? "" : " -> ");
    }
    cout << " (time: " << distForward[bestFriendState] << ")" << endl;

    // Backward Path: bestFriendState -> ... -> f
    // Note: Reconstructing path from f in rev_adj gives f -> ... -> v in reversed.
    // We reverse that sequence to get v -> ... -> f in original.
    vector<int> path2 = getPath(bestFriendState, parentBackward);
    reverse(path2.begin(), path2.end()); 
    for (int i = 0; i < path2.size(); i++) {
        cout << path2[i] << (i == path2.size() - 1 ? "" : " -> ");
    }
    cout << " (time: " << distBackward[bestFriendState] << ")" << endl;

    return 0;
}