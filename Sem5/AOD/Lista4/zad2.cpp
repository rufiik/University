#include "BipartiteGraph.hpp"
using namespace std;
int main(int argc, char *argv[]) {
    if (argc < 5) {
        cerr << "Użycie: " << argv[0] << " --size k --degree i [--printMatching]" << endl;
        return 1;
    }

    int k = 0, i = 0;
    bool printMatching = false;
    string glpkFilename;

    for (int j = 1; j < argc; ++j) {
        string arg = argv[j];
        if (arg == "--size" && j + 1 < argc) {
            k = stoi(argv[++j]);
        } else if (arg == "--degree" && j + 1 < argc) {
            i = stoi(argv[++j]);
        } else if (arg == "--printMatching") {
            printMatching = true;
        } else if (arg == "--glpk" && j + 1 < argc) {
            glpkFilename = argv[++j];
        }
    }

    if (k < 1 || k > 16 || i < 1 || i > (1 << k)) {
        cerr << "Nieprawidłowe wartości k lub i." << endl;
        cerr << "k musi być od 1 do 16, i od 1 do 2^k (" << (1 << k) << ")" << endl;
        return 1;
    }

    auto start_time = chrono::steady_clock::now();

    BipartiteGraph graph(k, i);
    graph.generateGraph();
    int max_matching = graph.hopcroftKarp();

    auto end_time = chrono::steady_clock::now();
    chrono::duration<double> elapsed = end_time - start_time;

    if (!glpkFilename.empty()) {
        graph.generateLP(glpkFilename);
        cout << "Wygenerowano plik LP: " << glpkFilename << endl;
    }

    cout << "Maksymalne skojarzenie: " << max_matching << endl;
    cout << "Liczba wierzchołków: " << (1 << k) << endl;
    cerr << "Czas działania: " << elapsed.count() << " s" << endl;

    if (printMatching) {
        graph.printMatching();
    }

    return 0;
}