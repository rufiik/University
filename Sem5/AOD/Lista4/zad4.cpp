#include "HypercubeGraph.hpp"
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
    int augmenting_paths = 0;

    auto start_time = chrono::steady_clock::now();
    int max_flow = graph.dinicMaxFlow(source, sink, augmenting_paths);
    auto end_time = chrono::steady_clock::now();

    chrono::duration<double> elapsed = end_time - start_time;

    cout << "Maksymalny przepływ: " << max_flow << endl;
    cerr << "Czas działania: " << elapsed.count() << " s" << endl;
    cerr << "Liczba ścieżek powiększających: " << augmenting_paths << endl;

    if (printFlow) {
        graph.printFlow();
    }

    return 0;
}