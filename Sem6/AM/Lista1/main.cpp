#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <random>
#include <algorithm>
#include <cmath>
#include <numeric>
#include <thread>
#include <chrono>
#include <omp.h>
#include <atomic>
#include "functions.hpp"

int main(int argc, char *argv[])
{
    std::string filename;
    int algo_choice = 0;

    if (argc != 3)
    {
        std::cout << "Użycie: ./main_local_search <filename> <wersja_algorytmu>\n";
        std::cout << "1 - Klasyczny Local Search (pełne sąsiedztwo)\n";
        std::cout << "2 - Local Search z losowym sąsiedztwem\n";
        return 1;
    }
    else
    {
        filename = argv[1];
        algo_choice = std::stoi(argv[2]);
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
        if (line == "NODE_COORD_SECTION") break;
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

    std::vector<int> permutation(n);
    std::iota(permutation.begin(), permutation.end(), 0);

    int total_length = 0;
    int total_steps = 0;
    int best_length = std::numeric_limits<int>::max();
    std::vector<int> best_permutation;
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

    if (algo_choice == 1) {
        std::cout << "=== URUCHAMIAM KLASYCZNY LOCAL SEARCH ===\n";
    } else if (algo_choice == 2) {
        std::cout << "=== URUCHAMIAM LOCAL SEARCH (LOSOWE SĄSIEDZTWO) ===\n";
    } else {
        std::cout << "Nieznana wersja algorytmu! Wybierz 1 lub 2.\n";
        return 1;
    }

    #pragma omp parallel for reduction(+:total_length,total_steps,finished) shared(time_limit_reached)
    for (int i = 0; i < n; i++)
    {
        if (time_limit_reached.load()) continue;

        auto now = std::chrono::steady_clock::now();
        double elapsed = std::chrono::duration<double>(now - start).count();
        if (elapsed > max_seconds) {
            time_limit_reached.store(true);
            continue;
        }

        std::random_device rd_thread;
        std::mt19937 gen_thread(rd_thread() + i + omp_get_thread_num());
        std::vector<int> local_perm = permutation;
        std::shuffle(local_perm.begin(), local_perm.end(), gen_thread);

        int local_best_length = 0;
        int steps = 0;
        std::vector<int> local_best_tour;

        // Wybór algorytmu w zależności od argumentu
        if (algo_choice == 1) {
            auto result = local_search(local_perm, coordinations, time_limit_reached);
            local_best_tour = std::get<0>(result);
            local_best_length = std::get<1>(result);
            steps = std::get<2>(result);
        } else {
            int n_neighbors = n;
            auto result = local_search_random(local_perm, coordinations, n_neighbors, gen_thread, time_limit_reached);
            local_best_tour = std::get<0>(result);
            local_best_length = std::get<1>(result);
            steps = std::get<2>(result);
        }

        total_length += local_best_length;
        total_steps += steps;
        finished++;

        #pragma omp critical
        {
            if (local_best_length < best_length) {
                best_length = local_best_length;
                best_permutation = local_best_tour;
            }
        }
    }

    double avg_length = finished > 0 ? static_cast<double>(total_length) / finished : 0.0;
    double avg_steps = finished > 0 ? static_cast<double>(total_steps) / finished : 0.0;

    std::cout << "\n=== PODSUMOWANIE DLA " << filename << " ===\n";
    std::cout << "Algorytm: " << (algo_choice == 1 ? "Klasyczny" : "Losowy") << "\n";
    std::cout << "Liczba miast: " << n << "\n";
    std::cout << "Liczba wykonanych uruchomień: " << finished << " z " << n << "\n";
    std::cout << "Średnia długość trasy: " << avg_length << "\n";
    std::cout << "Średnia liczba kroków poprawy: " << avg_steps << "\n";
    std::cout << "Najlepsza długość trasy: " << best_length << "\n";

    if (time_limit_reached.load()) {
        std::cout << "Osiągnięto limit czasu (20h), przerywam obliczenia i zapisuję wyniki.\n";
    }

    std::string out_name = filename;
    size_t pos = filename.find_last_of("/\\");
    if (pos != std::string::npos) {
        out_name = filename.substr(pos + 1);
    }
    
    std::string results_dir = "wyniki/";
    std::string suffix = (algo_choice == 1) ? "best_local_" : "best_local2_";
    
    std::ofstream best_file(results_dir + suffix + out_name);
    best_file << std::fixed << std::setprecision(4); 
    for (int idx : best_permutation) {
        best_file << idx+1 << " ";
    }
    best_file << "\n";
    for (int idx : best_permutation) {
        best_file << coordinations[idx].x << " " << coordinations[idx].y << "\n";
    }
    best_file << "# Wykonano uruchomień: " << finished << " z " << n << "\n";
    best_file.close();

    time_limit_reached.store(true); 
        if (watchdog.joinable()) {
            watchdog.join();
        }

    return 0;
}