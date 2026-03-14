#include "HypercubeGraph.hpp"
#include <iostream>
#include <chrono>
using namespace std;

int main(int argc, char *argv[]) {
    int k = 0;
    bool printFlow = false;

    for (int i = 1; i < argc; ++i) {
        string arg = argv[i];
        if (arg == "--size" && i + 1 < argc) {
            k = stoi(argv[++i]);
        } else if (arg == "--printFlow") {
            printFlow = true;
        }
    }

    if (k < 1 || k > 16) {
        cerr << "Wartość --size k musi być w zakresie [1, 16]." << endl;
        return 1;
    }

    HypercubeGraph graph(k);
    graph.generateEdges();

    int source = 0;
    int sink = (1 << k) - 1;

    // Porównanie Edmondsa-Karpa
    HypercubeGraph graphEdmonds = graph; // Kopia grafu dla Edmondsa-Karpa
    int augmenting_paths = 0;
    auto start_time_edmonds = chrono::steady_clock::now();
    int max_flow_edmonds = graphEdmonds.edmondsKarp(source, sink, augmenting_paths);
    auto end_time_edmonds = chrono::steady_clock::now();
    chrono::duration<double> elapsed_edmonds = end_time_edmonds - start_time_edmonds;

    // Porównanie Dinica
    HypercubeGraph graphDinic = graph; // Kopia grafu dla Dinica
    auto start_time_dinic = chrono::steady_clock::now();
    int augmenting_paths2 = 0;
    int max_flow_dinic = graphDinic.dinicMaxFlow(source, sink, augmenting_paths2);
    auto end_time_dinic = chrono::steady_clock::now();
    chrono::duration<double> elapsed_dinic = end_time_dinic - start_time_dinic;

    // Wyświetlenie wyników
    cout << k << ","
         << max_flow_edmonds << "," << elapsed_edmonds.count() << "," << augmenting_paths << ","
         << max_flow_dinic << "," << elapsed_dinic.count() << "," << augmenting_paths2 << endl;

    return 0;
}