#include "Graph.hpp"
#include "Prim.hpp"
#include "Kruskal.hpp"

// Funkcja wyznaczająca optymalną kolejność informowania dzieci
int dfs(int v, int parent, const std::vector<std::vector<int>>& tree, std::vector<std::vector<int>>& order) {
    std::vector<std::pair<int, int>> child_times; // {czas, dziecko}
    for (int u : tree[v]) {
        if (u != parent) {
            int t = dfs(u, v, tree, order);
            child_times.push_back({t, u});
        }
    }
    // Sortuj dzieci malejąco po czasie potrzebnym na poinformowanie ich poddrzewa
    std::sort(child_times.rbegin(), child_times.rend());
    // Zapamiętaj kolejność dzieci
    for (auto& p : child_times) order[v].push_back(p.second);

    // Wyznacz minimalną liczbę rund dla tego wierzchołka
    int max_time = 0;
    for (int i = 0; i < (int)child_times.size(); ++i) {
        max_time = std::max(max_time, child_times[i].first + i + 1);
    }
    return max_time;
}

int main() {
    int n, root;
    std::cin >> n >> root;
    std::vector<std::vector<int>> tree(n);
    // Wczytaj krawędzie drzewa (n-1 krawędzi)
    for (int i = 0; i < n-1; ++i) {
        int u, v;
        std::cin >> u >> v;
        tree[u].push_back(v);
        tree[v].push_back(u);
    }
std::cout << "Drzewo z " << n << " wierzchołkami, korzeń: " << root << "\n";
  for (int u = 0; u < n; ++u) {
    for (int v : tree[u]) {
        if (u < v) { // żeby nie powtarzać krawędzi
            std::cout << "Krawedz: " <<  v << " - " << u << "\n";
        }
    }
}

    std::vector<std::vector<int>> order(n);
    int rundy = dfs(root, -1, tree, order);

    std::cout << "Minimalna liczba rund: " << rundy << "\n";
    for (int v = 0; v < n; ++v) {
        if (!order[v].empty()) {
            std::cout << "Wierzcholek " << v << " informuje dzieci w kolejnosci: ";
            for (int u : order[v]) std::cout << u << " ";
            std::cout << "\n";
        }
    }
 
}