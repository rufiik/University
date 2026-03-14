#include "HypercubeGraph.hpp"

using namespace std;

HypercubeGraph::HypercubeGraph(int k) : k(k), num_vertices(1 << k), adj(num_vertices) {}

void HypercubeGraph::addEdge(int u, int v, int capacity) {
    adj[u].push_back({v, capacity, 0, (int)adj[v].size()});
    adj[v].push_back({u, 0, 0, (int)adj[u].size() - 1}); // Reverse edge
}

void HypercubeGraph::generateEdges() {
    random_device rd;
    mt19937 mt(rd());

    for (int i = 0; i < num_vertices; ++i) {
        for (int j = 0; j < k; ++j) {
            int neighbor = i ^ (1 << j); // Flip the j-th bit
            if (__builtin_popcount(i) < __builtin_popcount(neighbor)) {
                int pop_i = __builtin_popcount(i);
                int pop_n = __builtin_popcount(neighbor);
                int l = max({
                    pop_i,
                    k - pop_i,
                    pop_n,
                    k - pop_n
                });
                uniform_int_distribution<int> dist(1, (1 << l));
                int capacity = dist(mt);
                addEdge(i, neighbor, capacity);
            }
        }
    }
    
}

int HypercubeGraph::edmondsKarp(int source, int sink, int& augmenting_paths) {
    int max_flow = 0;
    vector<vector<int>> flow(num_vertices, vector<int>(num_vertices, 0));
    augmenting_paths = 0;

    while (true) {
        vector<int> parent(num_vertices, -1);
        vector<Edge*> path(num_vertices, nullptr);
        queue<int> q;
        q.push(source);
        parent[source] = source;

        while (!q.empty() && parent[sink] == -1) {
            int u = q.front();
            q.pop();

            for (Edge &e : adj[u]) {
                if (parent[e.to] == -1 && e.capacity > e.flow) {
                    parent[e.to] = u;
                    path[e.to] = &e;
                    q.push(e.to);
                    if (e.to == sink) break;
                }
            }
        }

        if (parent[sink] == -1) break;

        int augment = INT_MAX;
        for (int v = sink; v != source; v = parent[v]) {
            augment = min(augment, path[v]->capacity - path[v]->flow);
        }

        for (int v = sink; v != source; v = parent[v]) {
            path[v]->flow += augment;
            adj[path[v]->to][path[v]->reverse_index].flow -= augment;
        }

        max_flow += augment;
        augmenting_paths++;
    }
    return max_flow;
}

void HypercubeGraph::generateLP(const string& filename, int source, int sink) {
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Nie można otworzyć pliku: " << filename << endl;
        return;
    }

    // Komentarz wprowadzający
    file << "\\ Model LP dla maksymalnego przepływu w grafie hiperkostki\n";
    file << "\\ Funkcja celu: maksymalizacja całkowitego przepływu wychodzącego ze źródła\n";
    file << "maximize\n";
    file << "total_flow\n\n";

    // Komentarz dla sekcji ograniczeń
    file << "\\ Ograniczenia:\n";
    file << "\\ 1. Przepustowość każdej krawędzi nie może zostać przekroczona\n";
    file << "\\ 2. Zasada zachowania przepływu w każdym wierzchołku\n";
    file << "subject to\n";

    // Ograniczenia przepustowości
    for (int u = 0; u < num_vertices; ++u) {
        for (const Edge& e : adj[u]) {
            if (e.capacity > 0) {
                file << "cap_" << u << "_" << e.to << ": f_" << u << "_" << e.to
                     << " <= " << e.capacity << "\n";
            }
        }
    }

    file << "\n";

    // Zasada zachowania przepływu
    for (int u = 0; u < num_vertices; ++u) {
        file << "flow_" << u << ": ";

        // Krawędzie wychodzące
        int count = 0;
        for (const Edge& e : adj[u]) {
            if (e.capacity > 0) {
                if (count > 0) file << " + ";
                file << "f_" << u << "_" << e.to;
                count++;
            }
        }

        // Krawędzie wchodzące
        for (int v = 0; v < num_vertices; ++v) {
            for (const Edge& e : adj[v]) {
                if (e.capacity > 0 && e.to == u) {
                    if (count > 0) {
                        file << " - f_" << v << "_" << u;
                    } else {
                        file << "-f_" << v << "_" << u;
                        count++;
                    }
                }
            }
        }

        // Prawa strona równania
        if (u == source) {
            file << "-total_flow = 0 \n";
        } else if (u == sink) {
            file << "+ total_flow = 0 \n";
        } else {
            if (count == 0) {
                file << "0 = 0\n";  // dla izolowanego wierzchołka
            } else {
                file << " = 0\n";
            }
        }
    }

    // Komentarz dla sekcji ograniczeń dolnych
    file << "\n\\ Ograniczenia dolne dla zmiennych przepływu\n";
    file << "bounds\n";

    for (int u = 0; u < num_vertices; ++u) {
        for (const Edge& e : adj[u]) {
            if (e.capacity > 0) {
                file << "f_" << u << "_" << e.to << " >= 0\n";
            }
        }
    }

    file << "total_flow >= 0\n";
    file << "\nend\n";

    file.close();
}

void HypercubeGraph::printFlow() const {
    for (int u = 0; u < num_vertices; ++u) {
        for (const Edge &e : adj[u]) {
            if (e.capacity > 0) {
                cout << "Przepływ (" << u << " -> " << e.to << "): " << e.flow << " / " << e.capacity << endl;
            }
        }
    }
}
bool HypercubeGraph::buildLevelGraph(int source, int sink, vector<int>& level) {
    fill(level.begin(), level.end(), -1);
    level[source] = 0;

    queue<int> q;
    q.push(source);

    while (!q.empty()) {
        int u = q.front();
        q.pop();

        for (const Edge& e : adj[u]) {
            if (level[e.to] == -1 && e.flow < e.capacity) {
                level[e.to] = level[u] + 1;
                q.push(e.to);
            }
        }
    }

    return level[sink] != -1; // Czy dotarliśmy do ujścia
}

int HypercubeGraph::sendFlow(int u, int flow, int sink, vector<int>& level, vector<int>& start, int& augmenting_paths) {
    if (u == sink) {
    augmenting_paths++;
    return flow;
}
    for (; start[u] < (int)adj[u].size(); ++start[u]) {
        Edge& e = adj[u][start[u]];

        if (level[e.to] == level[u] + 1 && e.flow < e.capacity) {
            int curr_flow = min(flow, e.capacity - e.flow);
            int temp_flow = sendFlow(e.to, curr_flow, sink, level, start, augmenting_paths);

            if (temp_flow > 0) {
                e.flow += temp_flow;
                adj[e.to][e.reverse_index].flow -= temp_flow;
                return temp_flow;
            }
        }
    }

    return 0;
}

int HypercubeGraph::dinicMaxFlow(int source, int sink, int& augmenting_paths) {
    if (source == sink) return 0;

    int max_flow = 0;
    augmenting_paths = 0;
    vector<int> level(num_vertices);

    while (buildLevelGraph(source, sink, level)) {
        vector<int> start(num_vertices, 0);

        while (int flow = sendFlow(source, INT_MAX, sink, level, start, augmenting_paths)) {
            max_flow += flow;
            
        }
    }

    return max_flow;
}