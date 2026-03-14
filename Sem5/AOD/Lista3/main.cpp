#include "graph.hpp"
#include <iostream>
#include <string>
Graph graph;
int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Użycie: ./program -d <plik_danych> [-ss <plik_zrodel> -oss <plik_wynikowy>] [-p2p <plik_zapytan> -op2p <plik_wynikowy>][-alg <algorytm>]\n";
        return 1;
    }

    try {
        std::string dataFile;
        std::string ssInputFile;
        std::string ssOutputFile;
        std::string p2pInputFile;
        std::string p2pOutputFile;
        std::string algorithm="dijkstra";

        for (int i = 1; i < argc; ++i) {
            std::string arg = argv[i];
            if (arg == "-d") {
                dataFile = argv[++i];
            } else if (arg == "-ss") {
                ssInputFile = argv[++i];
            } else if (arg == "-oss") {
                ssOutputFile = argv[++i];
            } else if (arg == "-p2p") {
                p2pInputFile = argv[++i];
            } else if (arg == "-op2p") {
                p2pOutputFile = argv[++i];
            } else if (arg == "-alg") {
                algorithm = argv[++i];
        }
    }
        if (algorithm == "dijkstra") {
            } else if (algorithm == "dial") {
            } else if (algorithm == "radix") {
            } else { 
                throw std::runtime_error("Nieznany algorytm: " + algorithm);
            }

        if (!dataFile.empty()) {
            loadGraph(graph, dataFile);
        }

        if (!ssInputFile.empty() && !ssOutputFile.empty()) {
            shortestPathsFromSources(graph,dataFile, ssInputFile, ssOutputFile, algorithm);
        }

        if (!p2pInputFile.empty() && !p2pOutputFile.empty()) {
            shortestPathsPairs(graph, dataFile, p2pInputFile, p2pOutputFile, algorithm);
        }

    } catch (const std::exception& e) {
        std::cerr << "Błąd: " << e.what() << "\n";
        return 1;
    }

    return 0;
}