#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <cmath>
#include <algorithm>

using namespace std;

// Using a safe infinity that won't overflow when added
const double INF = 1e15;
const double EPS = 1e-9;

/**
 * Standard Floyd-Warshall using the Min-Sum logic.
 * After transformation, a path weight is the sum of -log(exchange_rate).
 */
void floyd(int n, vector<vector<double>> &dist) {
    for (int k = 1; k <= n; k++) {
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= n; j++) {
                if (dist[i][k] < INF && dist[k][j] < INF) {
                    if (dist[i][k] + dist[k][j] < dist[i][j]) {
                        dist[i][j] = dist[i][k] + dist[k][j];
                    }
                }
            }
        }
    }
}

bool solve(int n, int caseNum) {
    unordered_map<string, int> currencyMap;
    for (int i = 1; i <= n; i++) {
        string name;
        cin >> name;
        currencyMap[name] = i;
    }

    // Initialize distance matrix with Infinity
    vector<vector<double>> dist(n + 1, vector<double>(n + 1, INF));

    // Distance to self is 0 (log(1.0) = 0)
    for (int i = 1; i <= n; i++) {
        dist[i][i] = 0.0;
    }

    int m;
    cin >> m;
    for (int i = 0; i < m; i++) {
        string u, v;
        double rate;
        cin >> u >> rate >> v;

        // Transformation: Multiplication of rates > 1 
        // becomes Sum of -log(rates) < 0
        double weight = -log(rate);
        
        // Use min to keep the best exchange rate between two currencies
        if (weight < dist[currencyMap[u]][currencyMap[v]]) {
            dist[currencyMap[u]][currencyMap[v]] = weight;
        }
    }

    floyd(n, dist);

    // Check the diagonal for any negative values
    bool possible = false;
    for (int i = 1; i <= n; i++) {
        if (dist[i][i] < -EPS) {
            possible = true;
            break;
        }
    }

    cout << "Case " << caseNum << ": " << (possible ? "Yes" : "No") << endl;
    return possible;
}

int main() {
    // Optimize I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, caseCount = 0;
    // Problem usually terminates when n = 0
    while (cin >> n && n != 0) {
        caseCount++;
        solve(n, caseCount);
    }

    return 0;
}