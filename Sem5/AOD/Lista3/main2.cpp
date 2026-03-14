#include "graph.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <random>
#include <set>

Graph graph;

// Funkcja do generowania losowych wierzchołków
std::vector<long long> generateRandomSources(long long numNodes, int count) {
    std::set<long long> sources;
    sources.insert(1); 
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<long long> dist(2, numNodes); 

    while (sources.size() < count + 1) { 
        sources.insert(dist(gen));
    }

    return std::vector<long long>(sources.begin(), sources.end());
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Użycie: ./program -d <plik_danych> -o <plik_wynikowy>\n";
        return 1;
    }

    try {
        std::string dataFile;
        std::string outputFile;

        for (int i = 1; i < argc; ++i) {
            std::string arg = argv[i];
            if (arg == "-d") {
                dataFile = argv[++i];
            } else if (arg == "-o") {
                outputFile = argv[++i];
            }
        }
        loadGraph(graph, dataFile);

        std::vector<long long> sources = generateRandomSources(graph.numNodes, 5);

        std::string ssFile = outputFile + ".ss";
        std::ofstream ssOut(ssFile);
        if (!ssOut.is_open()) {
            throw std::runtime_error("Nie można otworzyć pliku: " + ssFile);
        }

        ssOut << "p aux sp ss " << sources.size() << "\n";
        for (long long source : sources) {
            ssOut << "s " << source << "\n";
        }
        ssOut.close();

        std::vector<std::string> algorithms = {"dijkstra", "dial", "radix"};

        for (const std::string& algorithm : algorithms) {
            std::string resultFile = outputFile + "." + algorithm + ".res";
            shortestPathsFromSources(graph, dataFile, ssFile, resultFile, algorithm);
        }

    } catch (const std::exception& e) {
        std::cerr << "Błąd: " << e.what() << "\n";
        return 1;
    }

    return 0;
}