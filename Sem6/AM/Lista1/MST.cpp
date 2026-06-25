#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <functional>
#include <random>
#include <iomanip>
#include <omp.h>
#include <chrono>
#include <thread>
#include "Prim.hpp"
#include "functions.hpp"
#include <atomic>

int main(int argc, char *argv[])
{
    std::string filename;
    if (argc != 2)
    {
        std::cout << "./randomTSP <filename> \n";
        return 1;
    }
    else
    {
        filename = argv[1];
    }
    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cout << "Nie można otworzyć pliku: " << filename << " !\n";
        return 1;
    }

    // Wczytaj dane
    std::string line;
    while (std::getline(file, line))
    {
        if (line == "NODE_COORD_SECTION")
        {
            break;
        }
    }

    int idx;
    double x, y;
    std::vector<coords> coordinations;
    while (file >> idx >> x >> y)
    {
        coordinations.push_back({x, y});
    }
    file.close();
    int n = coordinations.size();
    std::vector<std::vector<double>> matrix(n, std::vector<double>(n));

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            matrix[i][j] = distance(coordinations[i], coordinations[j]);
        }
    }

    // 1. Wyznacz MST i zapisz wagę
    std::cout << "Wyznaczanie MST...\n";
    // primMST(matrix); 
    auto mst_edges = primMST_edges(matrix);

    // 2. Zbuduj listę sąsiedztwa
    std::vector<std::vector<int>> adj(n);
    for (auto &e : mst_edges)
    {
        adj[e.first].push_back(e.second);
        adj[e.second].push_back(e.first);
    }

    // 3. Powtarzaj ceil(sqrt(n)) razy: losuj start, DFS, Local Search
    int repeats = std::ceil(std::sqrt(n));

    double best_cycle = std::numeric_limits<double>::max();
    std::vector<int> best_tour;
    int total_steps = 0;
    double total_cycle = 0.0;
    int finished = 0;

    const double max_seconds = 86400.0; 
    auto start = std::chrono::steady_clock::now();
    std::atomic<bool> time_limit_reached(false); 
    std::thread watchdog([&]() {
        while (!time_limit_reached.load()) {
            auto now = std::chrono::steady_clock::now();
            double elapsed = std::chrono::duration<double>(now - start).count();
            if (elapsed > max_seconds) {
                time_limit_reached.store(true);
                break;
            }
            std::this_thread::sleep_for(std::chrono::seconds(1)); 
        }
    });

    #pragma omp parallel for reduction(+:total_cycle,total_steps,finished) shared(time_limit_reached)
    for (int rep = 0; rep < repeats; ++rep) {
        if (time_limit_reached.load()) continue;

        auto now = std::chrono::steady_clock::now();
        double elapsed = std::chrono::duration<double>(now - start).count();
        if (elapsed > max_seconds) {
            time_limit_reached.store(true);
            continue;
        }

        std::random_device rd_thread;
        std::mt19937 gen_thread(rd_thread() + rep + omp_get_thread_num());
        std::uniform_int_distribution<> dist(0, n - 1);
        int start_vertex = dist(gen_thread);

        // DFS od wylosowanego wierzchołka
        std::vector<int> tour;
        std::vector<bool> visited(n, false);
        dfs(start_vertex, adj, visited, tour);

        auto result = local_search(tour, coordinations, time_limit_reached);
        std::vector<int> improved_tour = std::get<0>(result);
        int improved_len = std::get<1>(result);
        int steps = std::get<2>(result);

        total_cycle += improved_len;
        total_steps += steps;
        finished++;

        #pragma omp critical
        {
            if (improved_len < best_cycle) {
                best_cycle = improved_len;
                best_tour = improved_tour;
            }
        }
    }

    double avg_cycle = finished > 0 ? total_cycle / finished : 0.0;
    double avg_steps = finished > 0 ? static_cast<double>(total_steps) / finished : 0.0;

    std::cout << "\n=== MST + DFS + Local Search (" << repeats << " planowanych powtórzeń) ===\n";
    std::cout << "Liczba wykonanych powtórzeń: " << finished << "\n";
    std::cout << "Średnia długość cyklu po Local Search: " << avg_cycle << "\n";
    std::cout << "Średnia liczba kroków poprawy: " << avg_steps << "\n";
    std::cout << "Najlepsza długość cyklu: " << best_cycle << "\n";

    if (time_limit_reached.load()) {
        std::cout << "Osiągnięto limit czasu (20h), przerywam obliczenia i zapisuję wyniki.\n";
    }

    // Zapisz najlepszy cykl do pliku
    std::string out_name = filename;
    size_t pos = filename.find_last_of("/\\");
    if (pos != std::string::npos)
    {
        out_name = filename.substr(pos + 1);
    }
    std::string results_dir = "wyniki/";
    std::ofstream best_file(results_dir + "best_MST_LS_" + out_name);
    best_file << std::fixed << std::setprecision(4);
    for (int v : best_tour)
        best_file << v + 1 << " ";
    best_file << "\n";
    for (int v : best_tour)
        best_file << coordinations[v].x << " " << coordinations[v].y << "\n";
    best_file << "# Wykonano powtórzeń: " << finished << " z " << repeats << "\n";
    best_file.close();
    time_limit_reached.store(true); 
    if (watchdog.joinable()) {
        watchdog.join();
    }

    return 0;
}