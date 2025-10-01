#include "Graph.hpp"
#include "Kruskal.hpp"
#include "Prim.hpp"
#include <thread>
#include <mutex>
#include <vector>
#include "Graph.hpp"
#include "Kruskal.hpp"
#include "Prim.hpp"
#include <chrono>
#include <fstream>

std::mutex mtx; 

void testuj(int n, int rep_start, int rep_end, std::ofstream& wyniki) {
    for (int i = rep_start; i < rep_end; ++i) {
        auto graf = generuj_graf_pelny(n);

        auto start = std::chrono::high_resolution_clock::now();
        kruskalMST(graf);
        auto end = std::chrono::high_resolution_clock::now();
        double czas_kruskal = std::chrono::duration<double, std::milli>(end - start).count();

        start = std::chrono::high_resolution_clock::now();
        primMST(graf);
        end = std::chrono::high_resolution_clock::now();
        double czas_prim = std::chrono::duration<double, std::milli>(end - start).count();

        std::lock_guard<std::mutex> lock(mtx);
        wyniki << n << "," << i << "," << czas_kruskal << "," << czas_prim << "\n";
    }
}
int main() {
        // if (argc < 2) {
    //     std::cerr << "Usage: " << argv[0] << " <liczba wierzcholkow>\n";
    //     return 1;
    // }
    // int n;
    // try {
    //     n = std::stoi(argv[1]);
    //     if (n <= 0) {
    //         throw std::invalid_argument("Liczba wierzchołków musi być dodatnia.");
    //     }
    // } catch (const std::exception& e) {
    //     std::cerr << "Błąd: " << e.what() << "\n";
    //     return 1;
    // }
 int nMin = 100, nMax = 10000, step = 100, rep = 8;
    int num_threads = std::thread::hardware_concurrency(); // liczba rdzeni

    std::ofstream wyniki("czasy2.csv");
    wyniki << "n,rep_id,kruskal_ms,prim_ms\n";

    for (int n = nMin; n <= nMax; n += step) {
        std::vector<std::thread> watki;
        int rep_per_thread = rep / num_threads;
        int reszta = rep % num_threads;
        int start = 0;

        for (int t = 0; t < num_threads; ++t) {
            int end = start + rep_per_thread + (t < reszta ? 1 : 0);
            if (start < end) {
                watki.emplace_back(testuj, n, start, end, std::ref(wyniki));
            }
            start = end;
        }
        for (auto& w : watki) w.join();
    }

    wyniki.close();
    return 0;
}