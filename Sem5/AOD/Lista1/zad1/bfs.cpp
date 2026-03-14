#include <bits/stdc++.h>

void bfs(const std::vector<std::vector<int>> &adj, bool tree) {
    int n = (int)adj.size() - 1;
    std::vector<char> visited(n+1, 0);
    std::vector<int> parent(n+1, 0);   
    std::vector<int> visit_order;
    visit_order.reserve(n);

    std::queue<int> q;
    for(int s = 1; s <= n; ++s) {
        if(visited[s]) continue;
        visited[s] = 1;
        parent[s] = 0;
        q.push(s);

        while(!q.empty()) {
            int u = q.front(); q.pop();
            visit_order.push_back(u);
            for(int v : adj[u]) {
                if(!visited[v]) {
                    visited[v] = 1;
                    parent[v] = u;
                    q.push(v);
                }
            }
        }
    }

    for(size_t i = 0; i < visit_order.size(); ++i) {
        if(i) std::cout << ' ';
        std::cout << visit_order[i];
    }
    std::cout << '\n';

    if(tree) {
        for(int v = 1; v <= n; ++v) {
            if(parent[v] != 0) std::cout << parent[v] << ' ' << v << '\n';
        }
    }
}

int main(int argc, char *argv[]) {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    auto t0 = std::chrono::steady_clock::now();
    //Drzewo przesukiwania 
    bool tree = false;
    for(int i = 1; i < argc; ++i) {
        std::string s = argv[i];
        if(s == "-t") tree = true;
        else if( s == "-nt") tree = false;
        else {
            std::cerr << "Unknown option: " << s << "\n";
            return 1;
        }
    }
    //Wczytywanie grafu
    char type;
    int n;
    long long m;
    if(!(std::cin >> type >> n >> m)) return 0;
    std::vector<std::vector<int>> adj(n+1);
    for(long long i = 0; i < m; ++i) {
        int u, v;
        std::cin >> u >> v;   
        adj[u].push_back(v);
        if(type == 'U') adj[v].push_back(u);
    }

    bfs(adj, tree);
    auto t1 = std::chrono::steady_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();
    auto us = std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0).count();
    auto total_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(t1 - t0).count();
    std::cerr << "Czas wykonywania (ms): " << ms << '\n';
    std::cerr << "Czas wykonywania (us): " << us << '\n';
    std::cerr << "Czas wykonywania (ns): " << total_ns << '\n';
}