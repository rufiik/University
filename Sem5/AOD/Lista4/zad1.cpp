#include "HypercubeGraph.hpp"
using namespace std;

int main(int argc, char *argv[]) {
    int k = 0;
    bool printFlow = false;
    string glpkFilename;

    for (int i = 1; i < argc; ++i) {
        string arg = argv[i];
        if (arg == "--size" && i + 1 < argc) {
            k = stoi(argv[++i]);
        } else if (arg == "--printFlow") {
            printFlow = true;
        } else if (arg == "--glpk" && i + 1 < argc) {
            glpkFilename = argv[++i];
        }
    }

    if (k < 1 || k > 16) {
        cerr << "Wartość k musi być w zakresie [1, 16]." << endl;
        return 1;
    }


    HypercubeGraph graph(k);
    graph.generateEdges();

    if (!glpkFilename.empty()) {
        graph.generateLP(glpkFilename, 0, (1 << k) - 1);
        cout << "Wygenerowano plik LP: " << glpkFilename << endl;
    }
    int augmenting_paths = 0;
    auto start_time = chrono::steady_clock::now();
    int max_flow = graph.edmondsKarp(0, (1 << k) - 1, augmenting_paths);
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