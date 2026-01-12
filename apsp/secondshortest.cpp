#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

const long long INF = 1e15; 

void solve() {
    int N, M;
    cin >> N >> M;

    // dist1: shortest path, dist2: strictly second shortest path
    vector<vector<long long>> dist1(N + 1, vector<long long>(N + 1, INF));
    vector<vector<long long>> dist2(N + 1, vector<long long>(N + 1, INF));

    for (int i = 1; i <= N; i++) {
        dist1[i][i] = 0;
    }

    for (int i = 0; i < M; i++) {
        int u, v;
        long long w;
        cin >> u >> v >> w;

        // ONLY update u -> v (Directed Road)
        if (w < dist1[u][v]) {
            dist2[u][v] = dist1[u][v];
            dist1[u][v] = w;
        } else if (w > dist1[u][v] && w < dist2[u][v]) {
            dist2[u][v] = w;
        }
    }

    // Floyd-Warshall Triple Loop
    for (int k = 1; k <= N; k++) {
        for (int i = 1; i <= N; i++) {
            for (int j = 1; j <= N; j++) {
                // Potential paths to consider: 
                // 1. Shortest(i,k) + Shortest(k,j)
                // 2. Shortest(i,k) + SecondShortest(k,j)
                // 3. SecondShortest(i,k) + Shortest(k,j)
                long long candidate_paths[3];
                candidate_paths[0] = dist1[i][k] + dist1[k][j];
                candidate_paths[1] = dist1[i][k] + dist2[k][j];
                candidate_paths[2] = dist2[i][k] + dist1[k][j];

                for (int p = 0; p < 3; p++) {
                    long long d = candidate_paths[p];
                    if (d < dist1[i][j]) {
                        dist2[i][j] = dist1[i][j];
                        dist1[i][j] = d;
                    } else if (d > dist1[i][j] && d < dist2[i][j]) {
                        dist2[i][j] = d;
                    }
                }
            }
        }
    }

    // Example Output: Results for all pairs
    for (int i = 1; i <= N; i++) {
        for (int j = 1; j <= N; j++) {
            if (dist2[i][j] == INF) cout << "No second path from " << i << " to " << j << endl;
            else cout << "Second shortest " << i << "->" << j << ": " << dist2[i][j] << endl;
        }
    }
}

int main() {
    solve();
    return 0;
}