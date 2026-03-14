#ifndef HYPERCUBE_GRAPH_HPP
#define HYPERCUBE_GRAPH_HPP

#include <iostream>
#include <vector>
#include <queue>
#include <random>
#include <chrono>
#include <algorithm>
#include <climits>
#include <fstream>

struct Edge {
    int to, capacity, flow, reverse_index;
};

class HypercubeGraph {
    int k, num_vertices;
    std::vector<std::vector<Edge>> adj;

public:
    HypercubeGraph(int k);

    void addEdge(int u, int v, int capacity);
    void generateEdges();
    int edmondsKarp(int source, int sink, int& augmenting_paths);
    int dinicMaxFlow(int source, int sink, int& augmenting_paths);
    void generateLP(const std::string& filename, int source, int sink);
    void printFlow() const;
    bool buildLevelGraph(int source, int sink, std::vector<int>& level);
    int sendFlow(int u, int flow, int sink, std::vector<int>& level, std::vector<int>& start, int& augmenting_paths );
};

#endif // HYPERCUBE_GRAPH_HPP