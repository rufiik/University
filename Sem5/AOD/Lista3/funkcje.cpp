#include "graph.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <queue>
#include <limits>
#include <stdexcept>
const long long INF = std::numeric_limits<long long>::max();
void addEdge(Graph& graph, long long u, long long v, long long w) {
    graph.adjacencyList[u].emplace_back(v, w);
}
void loadGraph(Graph& graph,const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Nie można otworzyć pliku: " + filename);
    }

    std::string line;
    graph.minWeight = INF; 
    graph.maxWeight = -INF; 

    while (std::getline(file, line)) {
        if (line[0] == 'c') continue; 
        if (line[0] == 'p') {
            std::istringstream iss(line);
            std::string tmp;
            long long n, m;
            iss >> tmp >> tmp >> n >> m;
            graph.numNodes = n;
            graph.numEdges = m;
            graph.adjacencyList.resize(n + 1); 
        } else if (line[0] == 'a') {
            std::istringstream iss(line);
            char tmp;
            long long u, v, w;
            iss >> tmp >> u >> v >> w;
            addEdge(graph, u, v, w);
            graph.minWeight = std::min(graph.minWeight, w); 
            graph.maxWeight = std::max(graph.maxWeight, w); 
        }
    }
}
// Funkcja do obsługi trybu -ss
void shortestPathsFromSources(const Graph& graph, const std::string& dataFile, const std::string& inputFile, const std::string& outputFile, const std::string& algorithm) {
    std::ifstream in(inputFile);
    if (!in.is_open()) {
        throw std::runtime_error("Nie można otworzyć pliku: " + inputFile);
    }

    std::ofstream out(outputFile);
    if (!out.is_open()) {
        throw std::runtime_error("Nie można otworzyć pliku wynikowego: " + outputFile);
    }

    std::string line;
    std::vector<long long> sources;

    while (std::getline(in, line)) {
        if (line[0] == 'c') continue; 
        if (line[0] == 'p') continue; 
        if (line[0] == 's') {
            std::istringstream iss(line);
            char tmp;
            long long source;
            iss >> tmp >> source;
            sources.push_back(source);
        }
    }

    out << "f " << std::filesystem::path(dataFile).filename() << " "<< std::filesystem::path(inputFile).filename() << "\n";
    out << "g " << graph.numNodes << " " << graph.numEdges << " " << graph.minWeight << " " << graph.maxWeight << "\n";

    for (long long source : sources) {
        std::vector<long long> dist;
        auto start = std::chrono::high_resolution_clock::now();
            if(algorithm == "dijkstra"){
            dist = dijkstra(graph, source);
            }
            else if(algorithm == "dial"){
            dist = dial(graph, source);
            }
            else if(algorithm == "radix"){
            dist = radixHeapDijkstra(graph, source);
            }
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> elapsed = end - start;

        for (std::size_t i = 1; i < dist.size(); ++i) {
            if (dist[i] == INF) {
                out << "d " << source << " " << i << " -1\n"; 
            } else {
                out << "d " << source << " " << i << " " << dist[i] << "\n";
            }
        }
        out << "t " << elapsed.count() << " ms\n"; 
    }
}
// Funkcja do obsługi trybu -p2p
void shortestPathsPairs(const Graph& graph, const std::string& dataFile, const std::string& inputFile, const std::string& outputFile, const std::string& algorithm) {
    std::ifstream in(inputFile);
    if (!in.is_open()) {
        throw std::runtime_error("Nie można otworzyć pliku: " + inputFile);
    }

    std::ofstream out(outputFile);
    if (!out.is_open()) {
        throw std::runtime_error("Nie można otworzyć pliku wynikowego: " + outputFile);
    }

    std::string line;
     out << "f " << std::filesystem::path(dataFile).filename() << " " << std::filesystem::path(inputFile).filename() << "\n";
    out << "g " << graph.numNodes << " " << graph.numEdges << " " << graph.minWeight << " " << graph.maxWeight << "\n";
    while (std::getline(in, line)) {
        if (line[0] == 'c') continue;
        if (line[0] == 'p') continue; 
        if (line[0] == 'q') {
            std::istringstream iss(line);
            char tmp;
            long long source, t;
            iss >> tmp >> source >> t;
            std::vector<long long> dist;
            auto start = std::chrono::high_resolution_clock::now();
            if(algorithm == "dijkstra"){
            dist = dijkstra(graph, source, t);
            }
            else if(algorithm == "dial"){
            dist = dial(graph, source, t);
            }
            else if(algorithm == "radix"){
            dist = radixHeapDijkstra(graph, source, t);
            }
            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double, std::milli> elapsed = end - start;
            out << "d " << source << " " << t << " " << dist[0] << "\n";
            out << "t " << elapsed.count() << " ms\n";
        }
    }
}
// Implementacja algorytmu Dijkstry
std::vector<long long> dijkstra(const Graph& graph, long long source, long long target) {
    long long n = graph.numNodes + 1;
    std::vector<long long> dist(n, INF);
    dist[source] = 0;
    using pii = std::pair<long long, long long>;
    std::priority_queue<pii, std::vector<pii>, std::greater<>> pq;
    pq.emplace(0, source);

    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();
        if (d > dist[u]) continue;

        if (target != -1 && u == target) {
            return {dist[u]}; 

        }

        for (auto [v, w] : graph.adjacencyList[u]) {
            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                pq.emplace(dist[v], v);
            }
        }
    }

    if (target != -1) {
        return {-1};
    }

    return dist; 
}
// Implementacja algorytmu Diala
std::vector<long long> dial(const Graph& graph, long long source, long long target) {
    long long n = graph.numNodes + 1; 
    long long C = graph.maxWeight;   
    std::vector<long long> dist(n, INF); 
    dist[source] = 0;
    std::vector<std::deque<long long>> buckets(C + 1);
    buckets[0].push_back(source);
    long long currentBucket = 0; 

    long long emptyBucketsInRow = 0;

    while (emptyBucketsInRow < C + 1) {
        if (buckets[currentBucket].empty()) {
            currentBucket = (currentBucket + 1) % (C + 1);
            emptyBucketsInRow++;
            continue;
        }

        emptyBucketsInRow = 0; 
        long long u = buckets[currentBucket].front();
        buckets[currentBucket].pop_front();

        if (target != -1 && u == target) {
            return {dist[u]}; 
        }

        for (auto [v, w] : graph.adjacencyList[u]) {
            long long newDist = dist[u] + w;
            if (newDist < dist[v]) {
                dist[v] = newDist;
                long long bucketIndex = newDist % (C + 1);
                buckets[bucketIndex].push_back(v);
            }
        }
    }
    if (target != -1) {
        return {-1};
    }

    return dist; 
}
// Implementacja algorytmu Radix Heap 
std::vector<long long> radixHeapDijkstra(const Graph& graph, long long source, long long target) {
    long long n = graph.numNodes + 1;
    std::vector<long long> dist(n, INF);
    dist[source] = 0;
    RadixHeap radixHeap;
    radixHeap.push(0, source);

    while (!radixHeap.empty()) {
        auto [d, u] = radixHeap.pop();

        if (d > dist[u]) continue;

        if (target != -1 && u == target) {
            return {dist[u]};
        }

        for (auto [v, w] : graph.adjacencyList[u]) {
            long long newDist = dist[u] + w;
            if (newDist < dist[v]) {
                dist[v] = newDist;
                radixHeap.push(newDist, v);
            }
        }
    }

    if (target != -1) {
        return {-1};
    }

    return dist;
}