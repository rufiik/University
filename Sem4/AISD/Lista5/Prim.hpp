#ifndef PRIM_HPP
#define PRIM_HPP
#include "Graph.hpp"

// Algorytm Prima
void primMST(const std::vector<std::vector<double>>& graf) {
    int n = graf.size();
    std::vector<bool> w_mst(n, false);
    std::vector<double> min_waga(n, std::numeric_limits<double>::max());
    std::vector<int> parent(n, -1);

    using Para = std::pair<double, int>;
    std::priority_queue<Para, std::vector<Para>, std::greater<Para>> pq;

    min_waga[0] = 0.0;
    pq.emplace(0.0, 0);
    double suma = 0.0;
    while (!pq.empty()) {
        auto [waga, u] = pq.top();
        pq.pop();

        if (w_mst[u]) continue;
        w_mst[u] = true;
        suma += waga;

        for (int v = 0; v < n; ++v) {
            if (!w_mst[v] && graf[u][v] < min_waga[v] && graf[u][v] != 0.0) {
                min_waga[v] = graf[u][v];
                parent[v] = u;
                pq.emplace(min_waga[v], v);
            }
        }
    }

    // Wypisz krawędzie MST i sume wag
    std::cout << "\nKrawedzie MST (Prim):\n";
    for (int v = 1; v < n; ++v) {
        if (parent[v] != -1) {
            std::cout << parent[v] << " - " << v << " (waga: " << graf[parent[v]][v] << ")\n";
        }
    }
    std::cout << "Suma wag MST: " << suma << "\n";
}

std::vector<std::pair<int, int>> primMST_edges(const std::vector<std::vector<double>>& graf) {
    int n = graf.size();
    std::vector<bool> w_mst(n, false);
    std::vector<double> min_waga(n, std::numeric_limits<double>::max());
    std::vector<int> parent(n, -1);

    using Para = std::pair<double, int>;
    std::priority_queue<Para, std::vector<Para>, std::greater<Para>> pq;

    min_waga[0] = 0.0;
    pq.emplace(0.0, 0);

    while (!pq.empty()) {
        auto [waga, u] = pq.top();
        pq.pop();

        if (w_mst[u]) continue;
        w_mst[u] = true;

        for (int v = 0; v < n; ++v) {
            if (!w_mst[v] && graf[u][v] < min_waga[v] && graf[u][v] != 0.0) {
                min_waga[v] = graf[u][v];
                parent[v] = u;
                pq.emplace(min_waga[v], v);
            }
        }
    }

    std::vector<std::pair<int, int>> mst_edges;
    for (int v = 1; v < n; ++v) {
        if (parent[v] != -1) {
            mst_edges.emplace_back(parent[v], v);
        }
    }
    return mst_edges;
}
#endif // PRIM_HPP