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
int main(int argc, char* argv[]) {
        if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <liczba wierzcholkow>\n";
        return 1;
    }
    int n;
    try {
        n = std::stoi(argv[1]);
        if (n <= 0) {
            throw std::invalid_argument("Liczba wierzchołków musi być dodatnia.");
        }
    } catch (const std::exception& e) {
        std::cerr << "Błąd: " << e.what() << "\n";
        return 1;
    }
    auto graf = generuj_graf_pelny(n);
    //wyswietl graf
    std::cout << std::fixed << std::setprecision(3); 
    std::cout << "Graf wygenerowany z " << n << " wierzchołkami:\n";
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (graf[i][j] != 0.0) {
                std::cout <<graf[i][j]<< " ";
            }
        }
        std::cout << "\n";
    }
    primMST(graf);
    kruskalMST(graf);

    return 0;
}