#include <bits/stdc++.h>
using namespace std;

#define int long long

void solve() {
    int n;
    cin >> n;

    // Use a fixed-size array or a vector for the distance matrix
    vector<vector<int>> dist(n + 1, vector<int>(n + 1));
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
            cin >> dist[i][j];
        }
    }

    // Order of removal
    vector<int> removal_order(n);
    for (int i = 0; i < n; i++) {
        cin >> removal_order[i];
    }

    // Reverse the order to "add" nodes instead of removing them
    reverse(removal_order.begin(), removal_order.end());

    vector<int> results;
    vector<bool> added(n + 1, false);

    for (int k_node : removal_order) {
        added[k_node] = true;

        // Standard Floyd-Warshall step: Use the newly added k_node as an intermediate bridge
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= n; j++) {
                dist[i][j] = min(dist[i][j], dist[i][k_node] + dist[k_node][j]);
            }
        }

        // Sum up distances between all pairs of nodes that have been "added" so far
        int current_sum = 0;
        for (int i = 1; i <= n; i++) {
            if (!added[i]) continue;
            for (int j = 1; j <= n; j++) {
                if (!added[j]) continue;
                current_sum += dist[i][j];
            }
        }
        results.push_back(current_sum);
    }

    // Since we added nodes in reverse, the results are also in reverse
    reverse(results.begin(), results.end());
    for (int res : results) {
        cout << res << " ";
    }
    cout << endl;
}

int32_t main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    solve();
    return 0;
}