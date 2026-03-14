#include <bits/stdc++.h>

void dfs_detect(const std::vector<std::vector<int>> &adj, bool tree, bool directed) {
    int n = (int)adj.size() - 1;
    std::vector<int> color(n+1, 0); // 0 1 2 = unvisited, visiting, visited
    std::vector<int> parent(n+1, 0);
    std::vector<int> visit_order; visit_order.reserve(n);
    std::vector<int> topo; topo.reserve(n);

    bool cyclic = false;
    std::vector<std::pair<int,int>> st; st.reserve(n);

    for(int s = 1; s <= n && !cyclic; ++s) {
        if(color[s] != 0) continue;
        // start new component
        color[s] = 1;
        parent[s] = 0;
        st.emplace_back(s, 0);
        visit_order.push_back(s);

        while(!st.empty() && !cyclic) {
            auto &top = st.back();
            int u = top.first;
            int &i = top.second;

            if(i < (int)adj[u].size()) {
                int v = adj[u][i++];
                if(color[v] == 0) {
                    color[v] = 1;
                    parent[v] = u;
                    if(tree) std::cout << u << ' ' << v << '\n';
                    st.emplace_back(v, 0);
                    visit_order.push_back(v);
                } else {
                    if(directed) {
                        if(color[v] == 1) {
                            // back-edge -> cycle
                            cyclic = true;
                            break;
                        }
                    } else {
                        if(v != parent[u]) {
                        }
                    }
                }
            } else {
                color[u] = 2;
                topo.push_back(u); // postorder
                st.pop_back();
            }
        }
    }
    if(directed) {
        if(cyclic) {
            std::cout << "CYCLIC\n";
        } else {
            std::cout << "ACYCLIC\n";
            if(n <= 200) {
                std::reverse(topo.begin(), topo.end());
                for(size_t i = 0; i < topo.size(); ++i) {
                    if(i) std::cout << ' ';
                    std::cout << topo[i];
                }
                std::cout << '\n';
            }
        }
    } else {
        if(tree) {
            for(int v = 1; v <= n; ++v) {
                if(parent[v] != 0) std::cout << parent[v] << ' ' << v << '\n';
            }
        }
    }
}

int main(int argc, char *argv[]) {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
     auto t0 = std::chrono::steady_clock::now();
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
    bool directed = (type == 'D');
    dfs_detect(adj, tree, directed);
        auto t1 = std::chrono::steady_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();
    auto us = std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0).count();
    auto total_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(t1 - t0).count();
    std::cerr << "Czas wykonywania (ms): " << ms << '\n';
    std::cerr << "Czas wykonywania (us): " << us << '\n';
    std::cerr << "Czas wykonywania (ns): " << total_ns << '\n';
}
