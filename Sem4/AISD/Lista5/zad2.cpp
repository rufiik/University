#include "Graph.hpp"
#include "Prim.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <thread>
#include <mutex>

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

std::mutex mtx;

void testuj(int n, int reps_start, int reps_end, std::vector<std::vector<int>> tree, std::ofstream& plik, std::mt19937& gen) {
    for (int i = reps_start; i < reps_end; ++i) {
        int root = gen() % n;
        std::vector<std::vector<int>> order(n);
        int rundy = dfs(root, -1, tree, order);
        std::lock_guard<std::mutex> lock(mtx);
        plik << n << " " << rundy << " " << root << "\n";
    }
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
    return 0;
    // int nMin = 100, nMax = 10000, step = 100, reps = 16;
    // int num_threads = std::thread::hardware_concurrency();
    // std::random_device rd;

    // std::ofstream plik("wyniki2.csv");
    // plik << "n rundy root\n"; 
    // if (!plik) {
    //     std::cerr << "Nie można otworzyć pliku do zapisu!\n";
    //     return 1;
    // }

    // for (int n = nMin; n <= nMax; n += step) {
    //     auto graf = generuj_graf_pelny(n);
    //     auto mst_edges = primMST_edges(graf);

    //     std::vector<std::vector<int>> tree(n);
    //     for (auto [u, v] : mst_edges) {
    //         tree[u].push_back(v);
    //         tree[v].push_back(u);
    //     }

    //     std::vector<std::thread> watki;
    //     int reps_per_thread = reps / num_threads;
    //     int reszta = reps % num_threads;
    //     int start = 0;

    //     // Każdy wątek dostaje własny generator losowy (ważne!)
    //     std::vector<std::mt19937> gens;
    //     for (int t = 0; t < num_threads; ++t) {
    //         gens.emplace_back(rd());
    //     }

    //     for (int t = 0; t < num_threads; ++t) {
    //         int end = start + reps_per_thread + (t < reszta ? 1 : 0);
    //         if (start < end) {
    //             watki.emplace_back(testuj, n, start, end, tree, std::ref(plik), std::ref(gens[t]));
    //         }
    //         start = end;
    //     }
    //     for (auto& w : watki) w.join();
    // }
    // plik.close();
    // return 0;
}
   
