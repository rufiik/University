#include "functions.hpp"

void test_tabu_parameters(const std::vector<coords>& points) {
    int n = points.size();
    std::random_device rd;
    std::mt19937 gen(rd());
    std::atomic<bool> time_limit(false);
    
    std::cout << "\n=== TESTOWANIE PARAMETRÓW TABU SEARCH (n=" << n << ") ===" << std::endl;
    
    // Parametry do testowania
    std::vector<int> tabu_tenures = {7, 15, 30, 50, 100, 200};
    std::vector<int> stagnation_values = {5, 10, 20, 50, 100};
    
    int best_len_det = INT_MAX;
    int best_len_rand = INT_MAX;
    int best_tenure_det = 7;
    int best_stagnation_det = 20;
    std::tuple<int, int> best_params_rand;  // tenure, stagnation
    
    // Przygotuj kilka permutacji startowych dla testów
    std::vector<std::vector<int>> start_perms;
    for (int i = 0; i < 50; ++i) {
        std::vector<int> perm(n);
        std::iota(perm.begin(), perm.end(), 0);
        std::shuffle(perm.begin(), perm.end(), gen);
        start_perms.push_back(perm);
    }
    
    // === TESTY DETERMINISTYCZNE (wszyscy sąsiedzi) ===
    std::cout << "\n--- TRYB DETERMINISTYCZNY (wszyscy sąsiedzi) ---" << std::endl;
    
    for (int tenure : tabu_tenures) {
        for (int stagnation : stagnation_values) {
            int sum_len = 0;
            int config_best = INT_MAX;
            int total_steps = 0;
            
            std::cout << "Test: tenure=" << tenure << ", stagnation=" << stagnation << std::flush;
            
            for (int trial = 0; trial < 5; ++trial) {
                auto [perm, len, steps] = tabu_search(
                    start_perms[trial], points, tenure, stagnation, -1, nullptr, time_limit);
                sum_len += len;
                total_steps += steps;
                if (len < config_best) config_best = len;
                std::cout << "." << std::flush;
            }
            
            int avg_len = sum_len / 5;
            int avg_steps = total_steps / 5;
            std::cout << "   avg=" << avg_len << ", best=" << config_best 
                      << ", avg_steps=" << avg_steps << std::endl;
            
            if (config_best < best_len_det) {
                best_len_det = config_best;
                best_tenure_det = tenure;
                best_stagnation_det = stagnation;
            }
        }
    }
    
    // === TESTY LOSOWE (n losowych sąsiadów) ===
    std::cout << "\n--- TRYB LOSOWY (n=" << n << " losowych sąsiadów) ---" << std::endl;
    
    for (int tenure : tabu_tenures) {
        for (int stagnation : stagnation_values) {
            int sum_len = 0;
            int config_best = INT_MAX;
            int total_steps = 0;
            
            std::cout << "Test: tenure=" << tenure << ", stagnation=" << stagnation << std::flush;
            
            for (int trial = 0; trial < 50; ++trial) {
                auto [perm, len, steps] = tabu_search(
                    start_perms[trial], points, tenure, stagnation, n, &gen, time_limit);
                sum_len += len;
                total_steps += steps;
                if (len < config_best) config_best = len;
                std::cout << "." << std::flush;
            }
            
            int avg_len = sum_len / 50;
            int avg_steps = total_steps / 50;
            std::cout << "   avg=" << avg_len << ", best=" << config_best 
                      << ", avg_steps=" << avg_steps << std::endl;
            
            if (config_best < best_len_rand) {
                best_len_rand = config_best;
                best_params_rand = {tenure, stagnation};
            }
        }
    }
    
    // Wyniki
    std::cout << "\n=== NAJLEPSZE PARAMETRY ===" << std::endl;
    std::cout << "Deterministyczny: tenure=" << best_tenure_det 
              << ", stagnation=" << best_stagnation_det
              << "   długość: " << best_len_det << std::endl;
    std::cout << "Losowy (n=" << n << " sąsiadów): tenure=" << std::get<0>(best_params_rand) 
              << ", stagnation=" << std::get<1>(best_params_rand)
              << "   długość: " << best_len_rand << std::endl;
}


std::tuple<std::vector<int>, int, int> tabu_search(
    const std::vector<int>& start_perm,
    const std::vector<coords>& points,
    int tabu_tenure,
    int max_stagnation,
    int n_neighbors,              // -1 = wszyscy sąsiedzi, >0 = losowi
    std::mt19937* gen,
    const std::atomic<bool>& time_limit_reached)
{
    int n = points.size();
    std::vector<int> current = start_perm;
    int current_len = calculate_route_length(current, points);
    std::vector<int> best_perm = current;
    int best_len = current_len;
    
    std::vector<TabuMove> tabu_list;
    int steps = 0;
    int steps_without_improvement = 0;
    
    while (true) {
        if (time_limit_reached.load()) break;
        
        if (steps_without_improvement >= max_stagnation) break;
        
        std::vector<int> best_neighbor;
        int best_neighbor_len;
        
        if (n_neighbors == -1) {
            auto result = best_invert_neighbor_with_tabu(
                current, points, tabu_list, best_len, time_limit_reached);
            best_neighbor = result.first;
            best_neighbor_len = result.second;
        } else {
            auto result = best_random_invert_neighbor_with_tabu(
                current, points, n_neighbors, *gen, tabu_list, best_len, time_limit_reached);
            best_neighbor = result.first;
            best_neighbor_len = result.second;
        }
        
        int move_i = -1, move_j = -1;
        for (int i = 0; i < n; ++i) {
            if (current[i] != best_neighbor[i]) {
                int start_idx = i;
                while (i < n && current[i] != best_neighbor[start_idx]) i++;
                move_i = start_idx;
                move_j = i;
                break;
            }
        }
        
        current = best_neighbor;
        current_len = best_neighbor_len;
        steps++;
        
        if (move_i != -1 && move_j != -1) {
            tabu_list.push_back({move_i, move_j});
            while ((int)tabu_list.size() > tabu_tenure) {
                tabu_list.erase(tabu_list.begin());
            }
        }
        
        if (current_len < best_len) {
            best_len = current_len;
            best_perm = current;
            steps_without_improvement = 0;
        } else {
            steps_without_improvement++;
        }
    }
    
    return {best_perm, best_len, steps};
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
        if (line == "NODE_COORD_SECTION")
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

    int n = points.size();
    std::cout << "Liczba miast: " << n << std::endl;

    // testy
    // test_tabu_parameters(points);

    // Parametry Tabu Search
    int tabu_tenure = 100;    // długość listy tabu
    int max_stagnation = 100; // maksymalna liczba iteracji bez poprawy
    int n_neighbors = n;     // liczba losowych sąsiadów
    int num_trials = 100;     // liczba prób
    std::random_device rd;
    std::mt19937 gen(rd());
    std::atomic<bool> time_limit_reached(false);

    int overall_best = std::numeric_limits<int>::max();
    std::vector<int> overall_best_perm;
    std::vector<int> results;
    double total_time = 0.0;

    for (int trial = 1; trial <= num_trials; ++trial)
    {
        std::vector<int> start_perm(n);
        std::iota(start_perm.begin(), start_perm.end(), 0);
        std::shuffle(start_perm.begin(), start_perm.end(), gen);

        auto start_time = std::chrono::steady_clock::now();
        auto [best_perm, best_len, steps] = tabu_search(
            start_perm, points, tabu_tenure, max_stagnation, n_neighbors, &gen, time_limit_reached);
        auto end_time = std::chrono::steady_clock::now();
        double elapsed = std::chrono::duration<double>(end_time - start_time).count();

        results.push_back(best_len);
        total_time += elapsed;

        if (best_len < overall_best)
        {
            overall_best = best_len;
            overall_best_perm = best_perm;
        }

        if (trial % 10 == 0)
        {
            std::cout << "Próba " << trial << "/" << num_trials
                      << ", długość: " << best_len << ", najlepsza: " << overall_best << std::endl;
        }
    }

    // Oblicz statystyki
    double avg_length = std::accumulate(results.begin(), results.end(), 0.0) / num_trials;

    std::cout << "\n=== Wyniki Tabu Search ===" << std::endl;
    std::cout << "Najlepsza długość: " << overall_best << std::endl;
    std::cout << "Średnia długość: " << std::fixed << std::setprecision(2) << avg_length << std::endl;
    std::cout << "Średni czas: " << total_time / num_trials << " s" << std::endl;

    return 0;
}