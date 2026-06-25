#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP
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
#include <climits>
struct coords {
    double x;
    double y;
};

int distance(const coords &a, const coords &b) {
    double dx = a.x - b.x;
    double dy = a.y - b.y;
    return static_cast<int>(std::ceil(std::sqrt(dx*dx + dy*dy)));
}

int calculate_route_length(const std::vector<int> &permutation, const std::vector<coords> &points) {
    int total = 0; 
    int n = permutation.size();
    for(int i=0; i<n-1; i++){
        total += distance(points[permutation[i]], points[permutation[i+1]]);
    }
    total += distance(points[permutation[n - 1]], points[permutation[0]]);
    return total;
}

void invert(std::vector<int>& perm, int i, int j) {
    while(i < j) {
        std::swap(perm[i], perm[j]);
        ++i;
        --j;
    }
}

// Struktura do przechowywania ruchu na liście tabu
struct TabuMove {
    int i;
    int j;
    
    bool operator==(const TabuMove& other) const {
        return (i == other.i && j == other.j);
    }
};


std::pair<std::vector<int>, int> best_invert_neighbor_with_tabu(
    const std::vector<int>& perm, 
    const std::vector<coords>& points,
    const std::vector<TabuMove>& tabu_list,
    int best_global_len,  
    const std::atomic<bool>& time_limit_reached) 
{
    int n = perm.size();
    int best_delta = 0; 
    int best_i = -1;
    int best_j = -1;

    for (int i = 0; i < n - 1; ++i) {
        if (time_limit_reached.load()) break;
        
        for (int j = i + 1; j < n; ++j) {
            if (i == 0 && j == n - 1) continue;

            // Sprawdź czy ruch jest na liście tabu
            TabuMove current_move = {i, j};
            bool is_tabu = false;
            for (const auto& move : tabu_list) {
                if (move == current_move) {
                    is_tabu = true;
                    break;
                }
            }
            
            if (is_tabu) {
                int prev_i = (i == 0) ? n - 1 : i - 1;
                int next_j = (j == n - 1) ? 0 : j + 1;
                
                int node_prev = perm[prev_i];
                int node_i = perm[i];
                int node_j = perm[j];
                int node_next = perm[next_j];
                
                int old_edges = distance(points[node_prev], points[node_i]) + 
                                distance(points[node_j], points[node_next]);
                int new_edges = distance(points[node_prev], points[node_j]) + 
                                distance(points[node_i], points[node_next]);
                
                int delta = new_edges - old_edges;
                int new_len = calculate_route_length(perm, points) + delta;
                
                if (new_len >= best_global_len) {
                    continue;  
                }
            }

            int prev_i = (i == 0) ? n - 1 : i - 1;
            int next_j = (j == n - 1) ? 0 : j + 1;
            
            int node_prev = perm[prev_i];
            int node_i = perm[i];
            int node_j = perm[j];
            int node_next = perm[next_j];
            
            int old_edges = distance(points[node_prev], points[node_i]) + 
                            distance(points[node_j], points[node_next]);
            int new_edges = distance(points[node_prev], points[node_j]) + 
                            distance(points[node_i], points[node_next]);
            
            int delta = new_edges - old_edges;

            if (delta < best_delta) {
                best_delta = delta;
                best_i = i;
                best_j = j;
            }
        }
    }

    std::vector<int> best_perm = perm;
    int current_len = calculate_route_length(perm, points);

    if (best_delta < 0) {
        invert(best_perm, best_i, best_j);
        current_len += best_delta; 
    }
    return {best_perm, current_len};
}

std::pair<std::vector<int>, int> best_random_invert_neighbor_with_tabu(
    const std::vector<int>& perm,
    const std::vector<coords>& points,
    int n_neighbors,
    std::mt19937& gen,
    const std::vector<TabuMove>& tabu_list,
    int best_global_len,  
    const std::atomic<bool>& time_limit_reached) 
{
    int n = perm.size();
    int best_delta = 0;
    int best_i = -1;
    int best_j = -1;

    std::uniform_int_distribution<> dist(0, n - 1);

    for (int k = 0; k < n_neighbors; ++k) {
        if (time_limit_reached.load()) break;

        int i = dist(gen);
        int j = dist(gen);
        if (i > j) std::swap(i, j);
        if (i == j || (i == 0 && j == n - 1)) continue;

        TabuMove current_move = {i, j};
        bool is_tabu = false;
        for (const auto& move : tabu_list) {
            if (move == current_move) {
                is_tabu = true;
                break;
            }
        }
        
        if (is_tabu) {
            int prev_i = (i == 0) ? n - 1 : i - 1;
            int next_j = (j == n - 1) ? 0 : j + 1;
            
            int node_prev = perm[prev_i];
            int node_i = perm[i];
            int node_j = perm[j];
            int node_next = perm[next_j];
            
            int old_edges = distance(points[node_prev], points[node_i]) + 
                            distance(points[node_j], points[node_next]);
            int new_edges = distance(points[node_prev], points[node_j]) + 
                            distance(points[node_i], points[node_next]);
            
            int delta = new_edges - old_edges;
            int new_len = calculate_route_length(perm, points) + delta;
            
            if (new_len >= best_global_len) {
                continue; 
            }
        }

        int prev_i = (i == 0) ? n - 1 : i - 1;
        int next_j = (j == n - 1) ? 0 : j + 1;

        int node_prev = perm[prev_i];
        int node_i = perm[i];
        int node_j = perm[j];
        int node_next = perm[next_j];

        int old_edges = distance(points[node_prev], points[node_i]) + distance(points[node_j], points[node_next]);
        int new_edges = distance(points[node_prev], points[node_j]) + distance(points[node_i], points[node_next]);
        
        int delta = new_edges - old_edges;

        if (delta < best_delta) {
            best_delta = delta;
            best_i = i;
            best_j = j;
        }
    }

    std::vector<int> best_perm = perm;
    int current_len = calculate_route_length(perm, points);

    if (best_delta < 0) {
        invert(best_perm, best_i, best_j);
        current_len += best_delta;
    }
    return {best_perm, current_len};
}

std::tuple<std::vector<int>, int, double> simulated_annealing(
    const std::vector<coords>& points,
    double T0,
    double alpha,
    int epochs,
    int attempts_per_epoch,
    std::mt19937& gen);

std::tuple<std::vector<int>, int, int> tabu_search(
    const std::vector<int>& start_perm,
    const std::vector<coords>& points,
    int tabu_tenure,
    int max_stagnation,
    int n_neighbors,
    std::mt19937* gen,
    const std::atomic<bool>& time_limit_reached);    






#endif