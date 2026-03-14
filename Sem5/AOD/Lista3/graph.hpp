#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <iostream>
#include <bits/stdc++.h>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <queue>
#include <limits>
#include <stdexcept>
#include <filesystem>
struct Graph {
    long long numNodes; 
    long long numEdges; 
    long long minWeight;
    long long maxWeight; 
    std::vector<std::vector<std::pair<long long, long long>>> adjacencyList; 
};
class RadixHeap {
private:
    using pii = std::pair<long long, long long>;
    static const long long B = 65;              
    std::vector<pii> buckets[B];
    long long size;
    long long last;

public:
    RadixHeap() : size(0), last(0) {}

    void push(long long distance, long long vertex) {
        long long idx = getBucketIndex(distance);
        buckets[idx].push_back({distance, vertex});
        size++;
    }

    pii pop() {
        if (buckets[0].empty()) {
            long long i = 1;
            while (i < B && buckets[i].empty()) i++;
            
            if (i >= B) return {-1, -1}; 
            auto minIt = std::min_element(buckets[i].begin(), buckets[i].end(),
                [](const pii& a, const pii& b) { return a.first < b.first; });
            
            last = minIt->first;
            std::vector<pii> temp;
            temp.swap(buckets[i]);
            for (auto &p : temp) {
                long long idx = getBucketIndex(p.first);
                buckets[idx].push_back(p);
            }
        }

        pii res = buckets[0].back();
        buckets[0].pop_back();
        size--;
        last = res.first; 
        return res;
    }

    long long getBucketIndex(long long value) {
        if (value == last) return 0;
        long long diff = value ^ last;
        return 64 - __builtin_clzll(diff);
    }

    bool empty() const {
        return size == 0;
    }
};

std::vector<long long> dijkstra(const Graph& graph, long long source, long long target = -1);
std::vector<long long> dial(const Graph& graph, long long source, long long target = -1);
std::vector<long long> radixHeapDijkstra(const Graph& graph, long long source, long long target = -1);
void loadGraph(Graph& graph, const std::string& filename);
void shortestPathsFromSources(const Graph& graph,const std::string& dataFile, const std::string& inputFile, const std::string& outputFile, const std::string& algorithm);
void shortestPathsPairs(const Graph& graph, const std::string& dataFile, const std::string& inputFile, const std::string& outputFile, const std::string& algorithm);


#endif 