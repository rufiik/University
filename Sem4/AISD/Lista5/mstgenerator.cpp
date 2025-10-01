#include "Prim.hpp"
#include <random>
#include <iostream>

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
    auto mst_edges = primMST_edges(graf);

    // Wylosuj korzeń
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, n-1);
    int root = dist(gen);

    // Wypisz dane w formacie do zad2.cpp
    std::cout << n << " " << root << "\n";
    for (auto [u, v] : mst_edges) {
        std::cout << u << " " << v << "\n";
    }
}