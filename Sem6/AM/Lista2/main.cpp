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
#include <atomic>
#include "functions.hpp"

void test_sa_parameters(const std::vector<coords>& points) {
    std::random_device rd;
    std::mt19937 gen(rd());
    
    std::cout << "\n=== TESTOWANIE PARAMETRÓW ===" << std::endl;
    std::cout << "Liczba miast: " << points.size() << std::endl;
    
    // Parametry do testowania
    std::vector<double> temps = {500, 1000, 2000, 5000};
    std::vector<double> alphas = {0.8, 0.9, 0.95, 0.99};
    std::vector<int> epochs_list = {50, 100, 200};
    std::vector<double> attempts_factors = {0.25, 0.5, 1, 2};
    
    int best_len = std::numeric_limits<int>::max();
    std::tuple<double, double, int, int> best_params;
    
    for (double T0 : temps) {
        for (double alpha : alphas) {
            for (int epochs : epochs_list) {
                for (double factor : attempts_factors) {
                    int attempts = static_cast<int>(points.size() * factor);
                    if (attempts < 5) attempts = 5; 
                    
                    // Wykonaj 5 prób dla każdej kombinacji
                    int sum_len = 0;
                    int min_len = std::numeric_limits<int>::max();
                    
                    for (int trial = 0; trial < 49; ++trial) {
                        auto [perm, len, time] = simulated_annealing(
                            points, T0, alpha, epochs, attempts, gen);
                        sum_len += len;
                        if (len < min_len) min_len = len;
                    }
                    int avg_len = sum_len / 50;
                    
                    std::cout << "T0=" << T0 << ", α=" << alpha 
                              << ", epoki=" << epochs 
                              << ", prób/ep=" << attempts
                              << "   avg=" << avg_len << ", best=" << min_len << std::endl;
                    
                    if (min_len < best_len) {
                        best_len = min_len;
                        best_params = {T0, alpha, epochs, attempts};
                    }
                }
            }
        }
    }
    
    std::cout << "\nNajlepsze parametry: T0=" << std::get<0>(best_params)
              << ", alpha=" << std::get<1>(best_params)
              << ", epoki=" << std::get<2>(best_params)
              << ", prób/ep=" << std::get<3>(best_params)
              << ", (długość: " << best_len << ")" 
              << std::endl;
}


std::vector<int> random_neighbor(const std::vector<int>& perm, std::mt19937& gen) {
    int n = perm.size();
    std::uniform_int_distribution<> dist(0, n - 1);
    std::vector<int> neighbor = perm;
    
    int i = dist(gen);
    int j = dist(gen);
    if (i > j) std::swap(i, j);
    if (i == j) {
        if (i > 0) i--;
        else if (j < n - 1) j++;
    }
    
    invert(neighbor, i, j);
    return neighbor;
}

std::tuple<std::vector<int>, int, double> simulated_annealing(
    const std::vector<coords>& points,
    double T0,
    double alpha,
    int epochs,
    int attempts_per_epoch,
    std::mt19937& gen)
{
    int n = points.size();
    std::vector<int> current(n);
    std::iota(current.begin(), current.end(), 0);
    std::shuffle(current.begin(), current.end(), gen);
    
    int current_len = calculate_route_length(current, points);
    int best_len = current_len;
    std::vector<int> best_perm = current;
    
    double T = T0;
    double elapsed_time = 0.0;
    auto start_time = std::chrono::steady_clock::now();
    
    std::uniform_real_distribution<> prob_dist(0.0, 1.0);
    
    for (int epoch = 0; epoch < epochs; ++epoch) {
        
        for (int attempt = 0; attempt < attempts_per_epoch; ++attempt) {
            
            auto neighbor = random_neighbor(current, gen);
            int neighbor_len = calculate_route_length(neighbor, points);
            
            if (neighbor_len < current_len) {
                current = neighbor;
                current_len = neighbor_len;
                if (current_len < best_len) {
                    best_len = current_len;
                    best_perm = current;
                }
            } else {
                double delta = current_len - neighbor_len; 
                double acceptance_prob = std::exp(delta / T);
                if (prob_dist(gen) < acceptance_prob) {
                    current = neighbor;
                    current_len = neighbor_len;
                }
            }
        }
        T *= alpha;
    }
    
    auto end_time = std::chrono::steady_clock::now();
    elapsed_time = std::chrono::duration<double>(end_time - start_time).count();
    
    return {best_perm, best_len, elapsed_time};
}

int main(int argc, char *argv[])
{
    std::string filename;
    
    if (argc != 2)
    {
        std::cout << "Użycie: ./main <filename>\n";
        return 1;
    }
    else
    {
        filename = argv[1];
    }

    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cout << "Nie można otworzyć pliku: " << filename << "!\n";
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
    std::vector<coords> points;
    while (file >> idx >> x >> y)
    {
        points.push_back({x, y});
    }
    file.close();
    
    int n = points.size();
    std::cout << "Liczba miast: " << n << std::endl;

    // testy
    //  test_sa_parameters(points);

    // Parametry SA 
    double T0 = 5000.0;           // temperatura początkowa
    double alpha = 0.95;          // współczynnik chłodzenia
    int epochs = 1000;            // liczba epok
    int attempts_per_epoch = 2 * n; // prób na epokę (2 * n)

    // Parametry do testów
    int num_trials = 100;
    std::random_device rd;
    std::mt19937 gen(rd());

    std::cout << "\n=== Symulowane wyżarzanie ===" << std::endl;
    std::cout << "Parametry: T0=" << T0 << ", alpha=" << alpha 
            << ", epochs=" << epochs << ", attempts=" << attempts_per_epoch << std::endl;
    std::cout << "Liczba prób: " << num_trials << std::endl;
    
    int overall_best = std::numeric_limits<int>::max();
    std::vector<int> overall_best_perm;
    double total_time = 0.0;
    std::vector<int> results;
    
    for (int trial = 1; trial <= num_trials; ++trial) {
        auto [perm, length, elapsed] = simulated_annealing(
            points, T0, alpha, epochs, attempts_per_epoch, gen);
        
        results.push_back(length);
        total_time += elapsed;
        
        if (length < overall_best) {
            overall_best = length;
            overall_best_perm = perm;
        }
        
        if (trial % 10 == 0) {
            std::cout << "Próba " << trial << "/" << num_trials 
                      << ", długość: " << length << ", najlepsza: " << overall_best << std::endl;
        }
    }
    
    // Oblicz statystyki
    double avg_length = std::accumulate(results.begin(), results.end(), 0.0) / num_trials;
    
    std::cout << "\n=== Wyniki symulowanego wyżarzania ===" << std::endl;
    std::cout << "Najlepsza długość: " << overall_best << std::endl;
    std::cout << "Średnia długość: " << std::fixed << std::setprecision(2) << avg_length << std::endl;
    std::cout << "Średni czas: " << total_time / num_trials << " s" << std::endl;
    
    return 0;
}