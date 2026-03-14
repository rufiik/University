#include <bits/stdc++.h>
using namespace std;

bool check_bipartite(const vector<vector<int>> &adj, vector<int> &color) {
    int n = (int)adj.size() - 1;
    color.assign(n+1, -1); // -1 = niepokolorowany, 0/1 kolory
    queue<int> q;
    for (int s = 1; s <= n; ++s) {
        if (color[s] != -1) continue;
        color[s] = 0;
        q.push(s);
        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (int v : adj[u]) {
                if (v == u) return false; 
                if (color[v] == -1) {
                    color[v] = 1 - color[u];
                    q.push(v);
                } else if (color[v] == color[u]) {
                    return false;
                }
            }
        }
    }
    return true;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    auto t0 = std::chrono::steady_clock::now();
    char type;
    int n;
    long long m;
    if (!(cin >> type >> n >> m)) return 0;

    vector<vector<int>> adj(n+1);
    for (long long i = 0; i < m; ++i) {
        int u, v; cin >> u >> v;
        if (u < 1 || u > n || v < 1 || v > n) continue; 
        if (type == 'U') {
            adj[u].push_back(v);
            adj[v].push_back(u);
        } else {
            adj[u].push_back(v);
        }
    }

    vector<int> color;
    bool ok = check_bipartite(adj, color);
    auto t1 = std::chrono::steady_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();
    auto us = std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0).count();
    auto total_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(t1 - t0).count();

    if (ok) {
        cout << "BIPARTITE\n";
        if (n <= 200) {
            vector<int> V0, V1;
            for (int i = 1; i <= n; ++i) {
                if (color[i] == 0) V0.push_back(i);
                else V1.push_back(i);
            }
            cout << V0.size() << '\n';
            for (size_t i = 0; i < V0.size(); ++i) {
                if (i) cout << ' ';
                cout << V0[i];
            }
            cout << '\n';
            cout << V1.size() << '\n';
            for (size_t i = 0; i < V1.size(); ++i) {
                if (i) cout << ' ';
                cout << V1[i];
            }
            cout << '\n';
        }
    } else {
        cout << "NOT_BIPARTITE\n";
    }
    std::cerr << "Czas wykonywania (ms): " << ms << '\n';
    std::cerr << "Czas wykonywania (us): " << us << '\n';
    std::cerr << "Czas wykonywania (ns): " << total_ns << '\n';
    return 0;
}