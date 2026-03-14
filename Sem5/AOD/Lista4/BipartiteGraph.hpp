#ifndef BIPARTITE_GRAPH_HPP
#define BIPARTITE_GRAPH_HPP

#include <iostream>
#include <vector>
#include <queue>
#include <random>
#include <chrono>
#include <algorithm>
#include <climits>
#include <fstream>
#include <sstream>

class BipartiteGraph {
    int k, i, size;
    std::vector<std::vector<int>> adj; // Lista sąsiedztwa
    std::vector<int> pairU, pairV, dist;
    const int NIL = 0; // 0 będzie naszym NIL-em

public:
    BipartiteGraph(int k, int i);

    void generateGraph();
    bool bfs();
    bool dfs(int u);
    int hopcroftKarp();
    void printMatching();
    void generateLP(const std::string& filename);
};

#endif // BIPARTITE_GRAPH_HPP