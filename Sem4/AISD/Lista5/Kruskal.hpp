#ifndef KRUSKAL_HPP
#define KRUSKAL_HPP
#include "Graph.hpp"

// Struktura do reprezentacji krawędzi
struct Krawedz {
    int u, v;
    double waga;
    bool operator<(const Krawedz& inna) const {
        return waga < inna.waga;
    }
};

// Struktury do Union-Find (DSU)
struct DSU {
    std::vector<int> parent, rank;
    DSU(int n) : parent(n), rank(n, 0) {
        for (int i = 0; i < n; ++i) parent[i] = i;
    }
    int find(int x) {
        if (parent[x] != x) parent[x] = find(parent[x]);
        return parent[x];
    }
    bool unite(int x, int y) {
        x = find(x); y = find(y);
        if (x == y) return false;
        if (rank[x] < rank[y]) std::swap(x, y);
        parent[y] = x;
        if (rank[x] == rank[y]) rank[x]++;
        return true;
    }
};

void kruskalMST(const std::vector<std::vector<double>>& graf) {
    int n = graf.size();
    std::vector<Krawedz> krawedzie;

    // Zbierz wszystkie krawędzie (dla grafu nieskierowanego tylko raz każdą)
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            if (graf[i][j] != 0.0) {
                krawedzie.push_back({i, j, graf[i][j]});
            }
        }
    }

    // Posortuj krawędzie rosnąco po wadze
    std::sort(krawedzie.begin(), krawedzie.end());

    DSU dsu(n);
    std::vector<Krawedz> mst;
    double suma = 0.0;

    for (const auto& k : krawedzie) {
        if (dsu.unite(k.u, k.v)) {
            mst.push_back(k);
            suma += k.waga;
            if (mst.size() == static_cast<std::size_t>(n - 1)) break;
        }
    }

    // Wypisz krawędzie MST i sume wag
    std::cout << "\nKrawedzie MST (Kruskal):\n";
    for (const auto& k : mst) {
        std::cout << k.u << " - " << k.v << " (waga: " << k.waga << ")\n";
    }
    std::cout << "Suma wag MST (Kruskal): " << suma << "\n";
}
#endif // KRUSKAL_HPP