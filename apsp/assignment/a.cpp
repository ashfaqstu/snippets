#include <bits/stdc++.h>
using namespace std;

const double EPS = 1e-9;

void floyd(int n, vector<vector<double>> &dist) {
    for (int k = 1; k <= n; k++)
        for (int i = 1; i <= n; i++)
            for (int j = 1; j <= n; j++)
                dist[i][j] = max(dist[i][j], dist[i][k] * dist[k][j]);
}

void solve(int n, int cs) {
    unordered_map<string, int> name;
    for (int i = 1; i <= n; i++) {
        string s;
        cin >> s;
        name[s] = i;
    }

    vector<vector<double>> dist(n + 1, vector<double>(n + 1, 0.0));
    for (int i = 1; i <= n; i++) dist[i][i] = 1.0;

    int m;
    cin >> m;
    while (m--) {
        string u, v;
        double w;
        cin >> u >> w >> v;
        dist[name[u]][name[v]] =
            max(dist[name[u]][name[v]], w);
    }

    floyd(n, dist);

    for (int i = 1; i <= n; i++) {
        if (dist[i][i] > 1.0 + EPS) {
            cout << "Case " << cs << ": Yes";
            return;
        }
    }
    cout << "Case " << cs << ": No";
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, cs = 0;
    while (cin >> n && n) {
        if (++cs > 1) cout << '\n';
        solve(n, cs);
    }
    return 0;
}
