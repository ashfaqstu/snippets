#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
const double INF = 1e15;
const double EPS = 1e-9;

void floyd(int n, vector<vector<double>> &dist) {
    for (int k = 1; k <= n; k++) {
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= n; j++) {
                // Standard Floyd-Warshall update (finding shortest paths)
                if (dist[i][k] < INF && dist[k][j] < INF) {
                    if (dist[i][j] > dist[i][k] + dist[k][j]) {
                        dist[i][j] = dist[i][k] + dist[k][j];
                    }
                }
            }
        }
    }
}

void solve(int n, int caseNum) {
    unordered_map<string, int> name;
    for (int i = 1; i <= n; i++) {
        string s;
        cin >> s;
        name[s] = i;
    }

    int m;
    cin >> m;
    // Initialize with a large value (INF) for shortest path search
    vector<vector<double>> dist(n + 1, vector<double>(n + 1, INF));

    for (int i = 1; i <= n; i++) {
        dist[i][i] = 0.0;
    }

    for (int i = 0; i < m; i++) {
        string u, v;
        double rate;
        cin >> u >> rate >> v;
        // Edge weight becomes -log(rate)
        // Using min because we want the most negative path
        dist[name[u]][name[v]] = min(dist[name[u]][name[v]], -log(rate));
    }

    floyd(n, dist);

    bool possible = false;
    for (int i = 1; i <= n; i++) {
        // If dist[i][i] becomes negative, a negative cycle exists
        if (dist[i][i] < -EPS) {
            possible = true;
            break;
        }
    }

    cout << "Case " << caseNum << ": " << (possible ? "Yes" : "No") << endl;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, caseNum = 1;
    // Common format for this problem is reading N until it is 0
    while (cin >> n && n != 0) {
        solve(n, caseNum++);
    }
    return 0;
}