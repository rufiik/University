#ifndef GRAPH_HPP
#define GRAPH_HPP
#include <bits/stdc++.h>

std::vector<std::vector<double>> generuj_graf_pelny(int n) {
    std::vector<std::vector<double>> graf(n, std::vector<double>(n, 0.0));
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dist(0.0, 1.0);

    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            double waga = dist(gen);
            graf[i][j] = waga;
            graf[j][i] = waga; 
        }
    }
    return graf;
}


#endif // GRAPH_HPP