#include <bits/stdc++.h>
using namespace std;
#define int long long

struct Point {
    int x, y;
};

int get_dist(Point a, Point b) {
    return abs(a.x - b.x) + abs(a.y - b.y);
}

void solve() {
    int n; 
    if (!(cin >> n)) return;
    vector<Point> pts(n);
    for (int i = 0; i < n; i++) cin >> pts[i].x >> pts[i].y;

    vector<int> min_dist(n, LLONG_MAX);
    vector<bool> vis(n, false);
    min_dist[0] = 0;
    int total_weight = 0;

    for (int i = 0; i < n; i++) {
        int u = -1;
        // 1. Find the nearest unvisited node
        for (int j = 0; j < n; j++) {
            if (!vis[j] && (u == -1 || min_dist[j] < min_dist[u])) {
                u = j;
            }
        }

        if (min_dist[u] == LLONG_MAX) break;

        vis[u] = true;
        total_weight += min_dist[u];

        // 2. Update distances to all other unvisited nodes
        for (int v = 0; v < n; v++) {
            if (!vis[v]) {
                int d = get_dist(pts[u], pts[v]);
                if (d < min_dist[v]) {
                    min_dist[v] = d;
                }
            }
        }
    }

    cout << "Minimum Manhattan MST Weight: " << total_weight << endl;
}

int32_t main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    solve();
    return 0;
}