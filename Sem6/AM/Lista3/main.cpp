#include "functions.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <algorithm>
#include <numeric>
#include <chrono>
#include <thread>
#include <atomic>
#include <iomanip>
#include <cstring>
#include <tuple>

int random_int(int min, int max, std::mt19937 &gen)
{
    std::uniform_int_distribution<> dist(min, max);
    return dist(gen);
}

double random_double(std::mt19937 &gen)
{
    std::uniform_real_distribution<> dist(0.0, 1.0);
    return dist(gen);
}

void mutate_invert(std::vector<int> &route, double mutation_rate, std::mt19937 &gen)
{
    if (random_double(gen) < mutation_rate)
    {
        int i = random_int(0, route.size() - 2, gen);
        int j = random_int(i + 1, route.size() - 1, gen);
        invert(route, i, j);
    }
}

void pmx_crossover(const std::vector<int> &p1, const std::vector<int> &p2,
                   std::vector<int> &c1, std::vector<int> &c2,
                   std::mt19937 &gen)
{
    int n = p1.size();
    int a = random_int(0, n - 2, gen);
    int b = random_int(a + 1, n - 1, gen);

    c1.assign(n, -1);
    c2.assign(n, -1);

    for (int i = a; i <= b; ++i)
    {
        c1[i] = p1[i];
        c2[i] = p2[i];
    }

    for (int i = 0; i < n; ++i)
    {
        if (c1[i] != -1)
            continue;
        int val = p2[i];
        while (std::find(c1.begin() + a, c1.begin() + b + 1, val) != c1.begin() + b + 1)
        {
            int pos = std::find(p1.begin(), p1.end(), val) - p1.begin();
            val = p2[pos];
        }
        c1[i] = val;
    }

    for (int i = 0; i < n; ++i)
    {
        if (c2[i] != -1)
            continue;
        int val = p1[i];
        while (std::find(c2.begin() + a, c2.begin() + b + 1, val) != c2.begin() + b + 1)
        {
            int pos = std::find(p2.begin(), p2.end(), val) - p2.begin();
            val = p1[pos];
        }
        c2[i] = val;
    }
}

int tournament_selection(const std::vector<std::vector<int>> &population,
                         const std::vector<int> &distance,
                         int tournament_size,
                         std::mt19937 &gen)
{
    int best = random_int(0, population.size() - 1, gen);
    for (int i = 1; i < tournament_size; ++i)
    {
        int idx = random_int(0, population.size() - 1, gen);
        if (distance[idx] < distance[best])
        {
            best = idx;
        }
    }
    return best;
}

void generate_initial_population(std::vector<std::vector<int>> &population,
                                 std::vector<int> &distance,
                                 int pop_size,
                                 int n_cities,
                                 const std::vector<coords> &points,
                                 std::mt19937 &gen)
{
    std::vector<int> base(n_cities);
    std::iota(base.begin(), base.end(), 0);

    population.resize(pop_size);
    distance.resize(pop_size);

    for (int i = 0; i < pop_size; ++i)
    {
        population[i] = base;
        std::shuffle(population[i].begin(), population[i].end(), gen);
        distance[i] = calculate_route_length(population[i], points);
    }

    std::vector<int> indices(pop_size);
    std::iota(indices.begin(), indices.end(), 0);
    std::sort(indices.begin(), indices.end(), [&](int a, int b)
              { return distance[a] < distance[b]; });

    std::vector<std::vector<int>> sorted_pop(pop_size);
    std::vector<int> sorted_distance(pop_size);
    for (int i = 0; i < pop_size; ++i)
    {
        sorted_pop[i] = population[indices[i]];
        sorted_distance[i] = distance[indices[i]];
    }
    population = std::move(sorted_pop);
    distance = std::move(sorted_distance);
}

void replacement_optimized(std::vector<std::vector<int>> &population,
                           std::vector<int> &distance,
                           std::vector<std::vector<int>> &combined,
                           std::vector<int> &combined_distance,
                           const std::vector<std::vector<int>> &offspring,
                           const std::vector<int> &offspring_distance)
{
    int pop_size = population.size();
    
    for (int i = 0; i < pop_size; ++i)
    {
        combined[i] = population[i];
        combined_distance[i] = distance[i];
    }
    
    for (int i = 0; i < pop_size; ++i)
    {
        combined[pop_size + i] = offspring[i];
        combined_distance[pop_size + i] = offspring_distance[i];
    }
    
    std::vector<int> indices(2 * pop_size);
    std::iota(indices.begin(), indices.end(), 0);
    std::nth_element(indices.begin(), indices.begin() + pop_size, indices.end(),
                     [&](int a, int b) { return combined_distance[a] < combined_distance[b]; });
    
    for (int i = 0; i < pop_size; ++i)
    {
        population[i] = combined[indices[i]];
        distance[i] = combined_distance[indices[i]];
    }
}

void evolve_island(std::vector<std::vector<int>> &population,
                   std::vector<int> &distance,
                   const std::vector<coords> &points,
                   int generations,
                   int tournament_size,
                   double crossover_rate,
                   double mutation_rate,
                   double local_search_rate,
                   int stagnation_limit,
                   int island_id,
                   std::atomic<bool> &time_limit_reached)
{
    std::random_device rd;
    std::mt19937 gen(rd() + island_id);
    int stagnation = 0;
    int best_distance = distance[0];
    int pop_size = population.size();
    int n_cities = points.size();
    
    int n_neighbors = std::min(n_cities, 500);
    
    std::vector<std::vector<int>> offspring(pop_size);
    std::vector<int> offspring_distance(pop_size);
    std::vector<std::vector<int>> combined(2 * pop_size);
    std::vector<int> combined_distance(2 * pop_size);
    
    for (int i = 0; i < 2 * pop_size; ++i)
    {
        combined[i].resize(n_cities);
    }
    
    for (int g = 0; g < generations && !time_limit_reached.load(); ++g)
    {
        int offspring_count = 0;
        
        while (offspring_count < pop_size)
        {
            int p1_idx = tournament_selection(population, distance, tournament_size, gen);
            int p2_idx = tournament_selection(population, distance, tournament_size, gen);
            
            if (random_double(gen) < crossover_rate)
            {
                pmx_crossover(population[p1_idx], population[p2_idx], 
                             offspring[offspring_count], offspring[offspring_count + 1], gen);
                
                mutate_invert(offspring[offspring_count], mutation_rate, gen);
                mutate_invert(offspring[offspring_count + 1], mutation_rate, gen);
                
                if (random_double(gen) < local_search_rate)
                {
                    auto [c1_opt, len1, _] = local_search_random(offspring[offspring_count], points, 
                                                                 n_neighbors, gen, time_limit_reached);
                    auto [c2_opt, len2, __] = local_search_random(offspring[offspring_count + 1], points, 
                                                                  n_neighbors, gen, time_limit_reached);
                    offspring[offspring_count] = std::move(c1_opt);
                    offspring[offspring_count + 1] = std::move(c2_opt);
                    offspring_distance[offspring_count] = len1;
                    offspring_distance[offspring_count + 1] = len2;
                }
                else
                {
                    offspring_distance[offspring_count] = calculate_route_length(offspring[offspring_count], points);
                    offspring_distance[offspring_count + 1] = calculate_route_length(offspring[offspring_count + 1], points);
                }
            }
            else
            {
                offspring[offspring_count] = population[p1_idx];
                offspring[offspring_count + 1] = population[p2_idx];
                mutate_invert(offspring[offspring_count], mutation_rate, gen);
                mutate_invert(offspring[offspring_count + 1], mutation_rate, gen);
                
                if (random_double(gen) < local_search_rate)
                {
                    auto [c1_opt, len1, _] = local_search_random(offspring[offspring_count], points, 
                                                                 n_neighbors, gen, time_limit_reached);
                    auto [c2_opt, len2, __] = local_search_random(offspring[offspring_count + 1], points, 
                                                                  n_neighbors, gen, time_limit_reached);
                    offspring[offspring_count] = std::move(c1_opt);
                    offspring[offspring_count + 1] = std::move(c2_opt);
                    offspring_distance[offspring_count] = len1;
                    offspring_distance[offspring_count + 1] = len2;
                }
                else
                {
                    offspring_distance[offspring_count] = calculate_route_length(offspring[offspring_count], points);
                    offspring_distance[offspring_count + 1] = calculate_route_length(offspring[offspring_count + 1], points);
                }
            }
            offspring_count += 2;
        }
        
        replacement_optimized(population, distance, combined, combined_distance, 
                            offspring, offspring_distance);
        
        if (distance[0] < best_distance)
        {
            best_distance = distance[0];
            stagnation = 0;
        }
        else
        {
            stagnation++;
        }
        
        if (stagnation >= stagnation_limit)
            break;
    }
}

std::pair<std::vector<int>, int> genetic_algorithm_island(
    const std::vector<coords> &points,
    int pop_size,
    int generations,
    int tournament_size,
    double crossover_rate,
    double mutation_rate,
    double local_search_rate,
    int stagnation_limit,
    int num_islands,
    int migration_interval,
    int num_migrants)
{
    int n_cities = points.size();
    std::vector<std::vector<std::vector<int>>> islands(num_islands);
    std::vector<std::vector<int>> island_distance(num_islands);
    std::random_device rd;
    std::atomic<bool> time_limit_reached(false);
    
    for (int i = 0; i < num_islands; ++i)
    {
        std::mt19937 gen(rd() + i);
        generate_initial_population(islands[i], island_distance[i], pop_size, n_cities, points, gen);
    }
    
    for (int gen = 0; gen < generations; gen += migration_interval)
    {
        std::vector<std::thread> threads;
        for (int i = 0; i < num_islands; ++i)
        {
            threads.emplace_back(evolve_island, std::ref(islands[i]), std::ref(island_distance[i]),
                                 std::ref(points), migration_interval, tournament_size,
                                 crossover_rate, mutation_rate, local_search_rate, stagnation_limit, i,
                                 std::ref(time_limit_reached));
        }
        for (auto &t : threads)
            t.join();
        
        if (gen + migration_interval < generations)
        {
            std::vector<int> best_distance_per_island(num_islands);
            for (int i = 0; i < num_islands; ++i)
            {
                best_distance_per_island[i] = island_distance[i][0];
            }
            
            std::vector<int> order(num_islands);
            std::iota(order.begin(), order.end(), 0);
            std::sort(order.begin(), order.end(), [&](int a, int b)
                      { return best_distance_per_island[a] < best_distance_per_island[b]; });
            
            for (int m = 0; m < num_migrants && m < num_islands / 2; ++m)
            {
                int src = order[m];
                int dst = order[num_islands - 1 - m];
                islands[dst][pop_size - 1 - m] = islands[src][m];
                island_distance[dst][pop_size - 1 - m] = island_distance[src][m];

                std::vector<int> idx(pop_size);
                std::iota(idx.begin(), idx.end(), 0);
                std::nth_element(idx.begin(), idx.begin() + pop_size / 2, idx.end(),
                                 [&](int a, int b) { return island_distance[dst][a] < island_distance[dst][b]; });
                
                std::vector<std::vector<int>> sorted_pop(pop_size);
                std::vector<int> sorted_fit(pop_size);
                for (int k = 0; k < pop_size; ++k)
                {
                    sorted_pop[k] = islands[dst][idx[k]];
                    sorted_fit[k] = island_distance[dst][idx[k]];
                }
                islands[dst] = std::move(sorted_pop);
                island_distance[dst] = std::move(sorted_fit);
            }
        }
    }
    
    int best_idx = 0;
    int best_len = island_distance[0][0];
    for (int i = 1; i < num_islands; ++i)
    {
        if (island_distance[i][0] < best_len)
        {
            best_len = island_distance[i][0];
            best_idx = i;
        }
    }
    
    return {islands[best_idx][0], best_len};
}

std::tuple<std::vector<int>, int, double> run_single_test(const std::vector<coords> &points, int test_id)
{
    auto start = std::chrono::high_resolution_clock::now();
    
    const int POP_SIZE = 100;                 
    const int GENERATIONS = 200;                
    const int TOURNAMENT_SIZE = 3;
    const double CROSSOVER_RATE = 0.8;
    const double MUTATION_RATE = 0.05;
    const double LOCAL_SEARCH_RATE = 0.15;     
    const int STAGNATION_LIMIT = 200;           
    const int NUM_ISLANDS = std::thread::hardware_concurrency(); 
    const int MIGRATION_INTERVAL = 50;
    const int NUM_MIGRANTS = 2;
    
    auto [best_route, best_len] = genetic_algorithm_island(points, POP_SIZE, GENERATIONS,
                                                           TOURNAMENT_SIZE, CROSSOVER_RATE,
                                                           MUTATION_RATE, LOCAL_SEARCH_RATE,
                                                           STAGNATION_LIMIT, NUM_ISLANDS,
                                                           MIGRATION_INTERVAL, NUM_MIGRANTS);
    
    auto end = std::chrono::high_resolution_clock::now();
    double duration = std::chrono::duration<double>(end - start).count();
    
    return {best_route, best_len, duration}; 
}

void run_experiment(const std::vector<coords> &points, const std::string &filename)
{
    const int TOTAL_RUNS = 100; 
    
    std::cout << "\nRunning " << TOTAL_RUNS << " tests\n";
    std::cout << "Using " << std::thread::hardware_concurrency() << " hardware threads per test\n\n";
    
    std::vector<int> results;
    std::vector<double> times;
    
    std::vector<int> global_best_route;
    int global_best_len = std::numeric_limits<int>::max();
    
    for (int i = 0; i < TOTAL_RUNS; ++i)
    {
        std::cout << "Test " << i + 1 << "/" << TOTAL_RUNS << "... " << std::flush;
        
        auto [route, len, duration] = run_single_test(points, i);
        results.push_back(len);
        times.push_back(duration);
        
        if (len < global_best_len) {
            global_best_len = len;
            global_best_route = std::move(route);
        }
        
        std::cout << "len=" << len << " time=" << std::fixed << std::setprecision(2) << duration << "s\n";
    }
    
    double avg_len = std::accumulate(results.begin(), results.end(), 0.0) / results.size();
    double avg_time = std::accumulate(times.begin(), times.end(), 0.0) / times.size();
    int best_len = *std::min_element(results.begin(), results.end());
    int worst_len = *std::max_element(results.begin(), results.end());
    
    std::sort(results.begin(), results.end());
    
    std::cout << "\nRESULTS for: " << filename << "\n";
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "Number of cities: " << points.size() << "\n";
    std::cout << "Average length: " << avg_len << "\n";
    std::cout << "Best length:    " << best_len << "\n";
    std::cout << "Worst length:   " << worst_len << "\n";
    std::cout << "Average time per test: " << avg_time << " s\n";
    std::cout << "Total time: " << (avg_time * TOTAL_RUNS / 60.0) << " min\n";
    
    std::string out_name = filename;
    size_t pos = filename.find_last_of("/\\");
    if (pos != std::string::npos) {
        out_name = filename.substr(pos + 1);
    }
    
    std::string suffix = "best_gen_";
    std::string out_path = suffix + out_name;
    
    std::ofstream best_file(out_path);
    if (best_file.is_open()) {
        best_file << std::fixed << std::setprecision(4); 
        
        for (int idx : global_best_route) {
            best_file << idx + 1 << " "; 
        }
        best_file << "\n";
        
        for (int idx : global_best_route) {
            best_file << points[idx].x << " " << points[idx].y << "\n";
        }
        
        best_file.close();
        std::cout << "\nZapisano wyniki prawidlowo do pliku: " << out_path << "\n";
    } else {
        std::cout << "\nZAPIS NIE POWIODL SIE: Upewnij sie, ze katalog docelowy istnieje!\n";
    }
}

int main(int argc, char *argv[])
{
    std::string filename;
    
    if (argc != 2)
    {
        std::cout << "Uzycie: ./main <filename>\n";
        return 1;
    }
    else
    {
        filename = argv[1];
    }
    
    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cout << "Nie mozna otworzyc pliku: " << filename << "!\n";
        return 1;
    }
    
    std::string line;
    while (std::getline(file, line))
    {
        if (line == "NODE_COORD_SECTION" || line.find("NODE_COORD_SECTION") != std::string::npos)
            break;
    }
    
    int idx;
    double x, y;
    std::vector<coords> points;
    while (file >> idx >> x >> y)
    {
        points.push_back({x, y});
    }
    file.close();
    
    std::cout << "Liczba miast: " << points.size() << std::endl;
    
    run_experiment(points, filename); 
    
    return 0;
}