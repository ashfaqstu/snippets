#include <iostream>
#include <vector>
#include <cmath>
#include <queue>
#include <algorithm>

using namespace std;

const int INF = 1e9;

// Helper function to check if a Member (u) and Partner (v) can form a partnership
bool is_compatible(
    int exp1, int age1, int rec1, int org1, int weap1, int trust1, int lang1,
    int exp2, int age2, int rec2, int org2, int weap2, int trust2, int lang2
) {
    // 1. Experience Gap <= 12 
    if (abs(exp1 - exp2) > 12) return false;

    // 2. Age Compatibility <= 5 
    if (abs(age1 - age2) > 5) return false;

    // 3. Criminal Record Matching (Both clean or both criminal) 
    if (rec1 != rec2) return false;

    // 4. Territory Familiarity (Both Birmingham or both outsiders) 
    if (org1 != org2) return false;

    // 5. Weapon Proficiency (At least one person proficient) 
    if ((weap1 | weap2) == 0) return false;

    // 6. Trust Level (Sum >= 10) 
    if (trust1 + trust2 < 10) return false;

    // 7. Language Compatibility (Share at least one language) 
    if ((lang1 & lang2) == 0) return false;

    return true;
}

// BFS to find augmenting path in residual graph
int bfs(int s, int t, vector<int>& parent, vector<vector<int>>& adj, vector<vector<int>>& capacity) {
    fill(parent.begin(), parent.end(), -1);
    parent[s] = -2;
    queue<pair<int, int>> q;
    q.push({s, INF});

    while (!q.empty()) {
        int cur = q.front().first;
        int flow = q.front().second;
        q.pop();

        for (int next : adj[cur]) {
            if (parent[next] == -1 && capacity[cur][next] > 0) {
                parent[next] = cur;
                int new_flow = min(flow, capacity[cur][next]);
                if (next == t) return new_flow;
                q.push({next, new_flow});
            }
        }
    }
    return 0;
}

void solve(int t_case) {
    int m, n;
    cin >> m >> n;

    // Storage for attributes
    // Indices 0 to m-1 for members
    // Indices 0 to n-1 for partners
    vector<int> m_exp(m), m_age(m), m_rec(m), m_org(m), m_weap(m), m_trust(m), m_lang(m);
    vector<int> p_exp(n), p_age(n), p_rec(n), p_org(n), p_weap(n), p_trust(n), p_lang(n);

    // Reading Gang Members
    for (int i = 0; i < m; i++) {
        cin >> m_exp[i] >> m_age[i] >> m_rec[i] >> m_org[i] >> m_weap[i] >> m_trust[i] >> m_lang[i];
    }

    // Reading Potential Partners
    for (int i = 0; i < n; i++) {
        cin >> p_exp[i] >> p_age[i] >> p_rec[i] >> p_org[i] >> p_weap[i] >> p_trust[i] >> p_lang[i];
    }

    // Building the Graph
    // Node 0: Source (S)
    // Nodes 1 to m: Gang Members
    // Nodes m+1 to m+n: Potential Partners
    // Node m+n+1: Sink (T)
    int S = 0;
    int T = m + n + 1;
    int num_nodes = T + 1;

    vector<vector<int>> capacity(num_nodes, vector<int>(num_nodes, 0));
    vector<vector<int>> adj(num_nodes);

    // Edges from Source to Members
    for (int i = 1; i <= m; i++) {
        adj[S].push_back(i);
        adj[i].push_back(S);
        capacity[S][i] = 1;
    }

    // Edges from Partners to Sink
    for (int j = 1; j <= n; j++) {
        int partner_node = m + j;
        adj[partner_node].push_back(T);
        adj[T].push_back(partner_node);
        capacity[partner_node][T] = 1;
    }

    // Edges from Members to Partners (if compatible)
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            if (is_compatible(
                m_exp[i], m_age[i], m_rec[i], m_org[i], m_weap[i], m_trust[i], m_lang[i],
                p_exp[j], p_age[j], p_rec[j], p_org[j], p_weap[j], p_trust[j], p_lang[j]
            )) {
                int u = i + 1;      // Member node index
                int v = m + j + 1;  // Partner node index
                
                adj[u].push_back(v);
                adj[v].push_back(u);
                capacity[u][v] = 1;
            }
        }
    }

    // Edmonds-Karp Algorithm
    int max_flow = 0;
    int new_flow;
    vector<int> parent(num_nodes);

    while (new_flow = bfs(S, T, parent, adj, capacity)) {
        max_flow += new_flow;
        int cur = T;
        while (cur != S) {
            int prev = parent[cur];
            capacity[prev][cur] -= new_flow;
            capacity[cur][prev] += new_flow;
            cur = prev;
        }
    }

    cout << "Case " << t_case << ": " << max_flow << endl;
}

int main() {
    // Optimize I/O operations
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int t;
    if (cin >> t) {
        for (int i = 1; i <= t; i++) {
            solve(i);
        }
    }
    return 0;
}