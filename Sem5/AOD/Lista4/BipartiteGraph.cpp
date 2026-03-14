#include "BipartiteGraph.hpp"

using namespace std;

BipartiteGraph::BipartiteGraph(int k, int i) : k(k), i(i), size(1 << k), adj(size + 1) {}

void BipartiteGraph::generateGraph() {
    random_device rd;
    mt19937 mt(rd());
    uniform_int_distribution<int> dist(0, size - 1);

    for (int u = 1; u <= size; ++u) {
        vector<int> neighbors;
        while ((int)neighbors.size() < i) {
            int v = dist(mt) + 1; // +1 bo wierzchołki od 1 do size
            if (find(neighbors.begin(), neighbors.end(), v) == neighbors.end()) {
                neighbors.push_back(v);
            }
        }
        adj[u] = neighbors;
    }
}

bool BipartiteGraph::bfs() {
    queue<int> q;

    for (int u = 1; u <= size; ++u) {
        if (pairU[u] == NIL) {
            dist[u] = 0;
            q.push(u);
        } else {
            dist[u] = INT_MAX;
        }
    }

    dist[NIL] = INT_MAX;

    while (!q.empty()) {
        int u = q.front();
        q.pop();

        if (dist[u] < dist[NIL]) {
            for (int v : adj[u]) {
                if (dist[pairV[v]] == INT_MAX) {
                    dist[pairV[v]] = dist[u] + 1;
                    q.push(pairV[v]);
                }
            }
        }
    }

    return dist[NIL] != INT_MAX;
}

bool BipartiteGraph::dfs(int u) {
    if (u != NIL) {
        for (int v : adj[u]) {
            if (dist[pairV[v]] == dist[u] + 1) {
                if (dfs(pairV[v])) {
                    pairV[v] = u;
                    pairU[u] = v;
                    return true;
                }
            }
        }
        dist[u] = INT_MAX;
        return false;
    }
    return true;
}

int BipartiteGraph::hopcroftKarp() {
    pairU.assign(size + 1, NIL);
    pairV.assign(size + 1, NIL);
    dist.assign(size + 1, 0);

    int matching = 0;

    while (bfs()) {
        for (int u = 1; u <= size; ++u) {
            if (pairU[u] == NIL && dfs(u)) {
                ++matching;
            }
        }
    }
    return matching;
}

void BipartiteGraph::printMatching() {
    for (int u = 1; u <= size; ++u) {
        if (pairU[u] != NIL) {
            cout << "V1[" << (u - 1) << "] -- V2[" << (pairU[u] - 1) << "]" << endl;
        }
    }
}

void BipartiteGraph::generateLP(const string& filename) {
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Nie można otworzyć pliku: " << filename << endl;
        return;
    }

    file << "\\ Model LP dla maksymalnego skojarzenia w grafie dwudzielnym\n";
    file << "\\ Funkcja celu: maksymalizacja liczby skojarzonych krawędzi\n";
    file << "maximize\n";
    file << " obj: ";
    bool first = true;

    for (int u = 1; u <= size; ++u) {
        for (int v : adj[u]) {
            if (!first) file << " + ";
            file << "x_" << u << "_" << v;
            first = false;
        }
    }
    file << "\n\n";

    file << "\\ Ograniczenia:\n";
    file << "\\ 1. Każdy wierzchołek w V1 może być incydentny z co najwyżej jedną krawędzią\n";
    file << "\\ 2. Każdy wierzchołek w V2 może być incydentny z co najwyżej jedną krawędzią\n";
    file << "subject to\n";

    for (int u = 1; u <= size; ++u) {
        file << " c1_" << u << ": ";
        first = true;
        for (int v : adj[u]) {
            if (!first) file << " + ";
            file << "x_" << u << "_" << v;
            first = false;
        }
        file << " <= 1\n";
    }

    for (int v = 1; v <= size; ++v) {
        bool hasEdges = false;
        stringstream constraint;
        constraint << " c2_" << v << ": ";
        first = true;

        for (int u = 1; u <= size; ++u) {
            if (find(adj[u].begin(), adj[u].end(), v) != adj[u].end()) {
                if (!first) constraint << " + ";
                constraint << "x_" << u << "_" << v;
                first = false;
                hasEdges = true;
            }
        }

        if (hasEdges) {
            constraint << " <= 1\n";
            file << constraint.str();
        }
    }

    file << "\n\\ Zmienne binarne: każda krawędź może być częścią skojarzenia (1) lub nie (0)\n";
    file << "\nbinary\n";

    for (int u = 1; u <= size; ++u) {
        for (int v : adj[u]) {
            file << "x_" << u << "_" << v << "\n";
        }
    }

    file << "\nend\n";
    file.close();
}