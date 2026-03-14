#include <bits/stdc++.h>

void scc_process(const std::vector<std::vector<int>> &adj, const std::vector<std::vector<int>> &rev, int n) {
    // 1) pierwszy przebieg DFS (iteracyjny) - zbieramy postorder (finish order)
    std::vector<char> vis(n+1, 0);
    std::vector<int> order; order.reserve(n);
    std::vector<std::pair<int,int>> st; st.reserve(n);

 

    for(int s = 1; s <= n; ++s) {
        if(vis[s]) continue;
        vis[s] = 1;
        st.emplace_back(s, 0);
        while(!st.empty()) {
            auto &top = st.back();
            int u = top.first;
            int &i = top.second;
            if(i < (int)adj[u].size()) {
                int v = adj[u][i++];
                if(!vis[v]) {
                    vis[v] = 1;
                    st.emplace_back(v, 0);
                }
            } else {
                st.pop_back();
                order.push_back(u); 
            }
        }
    }

    // 2) drugi przebieg na grafie odwróconym: w kolejności malejących finish times
    std::fill(vis.begin(), vis.end(), 0);
    std::vector<std::vector<int>> comps;
    comps.reserve(n);

    for(int idx = (int)order.size()-1; idx >= 0; --idx) {
        int s = order[idx];
        if(vis[s]) continue;
        std::vector<int> comp;
        st.clear();
        vis[s] = 1;
        st.emplace_back(s, 0);
        while(!st.empty()) {
            auto &top = st.back();
            int u = top.first;
            int &i = top.second;
            if(i == 0) comp.push_back(u); 
            if(i < (int)rev[u].size()) {
                int v = rev[u][i++];
                if(!vis[v]) {
                    vis[v] = 1;
                    st.emplace_back(v, 0);
                }
            } else {
                st.pop_back();
            }
        }
        comps.push_back(std::move(comp));
    }

    std::cout << "Liczba składowych: " << comps.size() << '\n';
    for(size_t i = 0; i < comps.size(); ++i) {
        std::cout << "Rozmiar składowej " << i+1 << ": " << comps[i].size() << '\n';
        if(n <= 200) {
            for(size_t j = 0; j < comps[i].size(); ++j) {
                if(j) std::cout << ' ';
                std::cout << comps[i][j];
            }
            std::cout << '\n';
        }
    }
}
int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    auto t0 = std::chrono::steady_clock::now();

    char type;
    int n;
    long long m;
    if(!(std::cin >> type >> n >> m)) return 0;

    std::vector<std::vector<int>> adj(n+1), rev(n+1);
    for(long long i = 0; i < m; ++i) {
        int u,v; std::cin >> u >> v;
        adj[u].push_back(v);
        rev[v].push_back(u);
        if(type == 'U') { 
            adj[v].push_back(u);
            rev[u].push_back(v);
        }
    }

    scc_process(adj, rev, n);
    auto t1 = std::chrono::steady_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();
    auto us = std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0).count();
    auto total_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(t1 - t0).count();
    std::cerr << "Czas wykonywania (ms): " << ms << '\n';
    std::cerr << "Czas wykonywania (us): " << us << '\n';
    std::cerr << "Czas wykonywania (ns): " << total_ns << '\n';
}