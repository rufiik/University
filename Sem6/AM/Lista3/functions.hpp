#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP
#include <cmath>
#include <vector>
#include <random>
#include <atomic>
#include <tuple>

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

std::pair<std::vector<int>, int> best_invert_neighbor(
    const std::vector<int>& perm, 
    const std::vector<coords>& points,
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

std::tuple<std::vector<int>, int, int> local_search(
    const std::vector<int>& start_perm, 
    const std::vector<coords>& points,
    const std::atomic<bool>& time_limit_reached) 
{
    std::vector<int> current = start_perm;
    int current_len = calculate_route_length(current, points);
    int steps = 0;

    while(true) {
        if (time_limit_reached.load()) break;

        auto result = best_invert_neighbor(current, points, time_limit_reached);
        auto neighbor = result.first;
        auto neighbor_len = result.second;
        
        if(neighbor_len < current_len) {
            current = neighbor;
            current_len = neighbor_len;
            ++steps;
        } else {
            break;
        }
    }
    return {current, current_len, steps};
}

std::pair<std::vector<int>, int> best_random_invert_neighbor(
    const std::vector<int>& perm,
    const std::vector<coords>& points,
    int n_neighbors,
    std::mt19937& gen,
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

std::tuple<std::vector<int>, int, int> local_search_random(
    const std::vector<int>& start_perm,
    const std::vector<coords>& points,
    int n_neighbors,
    std::mt19937& gen,
    const std::atomic<bool>& time_limit_reached) 
{
    std::vector<int> current = start_perm;
    int current_len = calculate_route_length(current, points);
    int steps = 0;

    while (true) {
        if (time_limit_reached.load()) break;

        auto result = best_random_invert_neighbor(current, points, n_neighbors, gen, time_limit_reached);
        auto neighbor = result.first;
        auto neighbor_len = result.second;
        
        if (neighbor_len < current_len) {
            current = neighbor;
            current_len = neighbor_len;
            ++steps;
        } else {
            break;
        }
    }
    return {current, current_len, steps};
}


#endif