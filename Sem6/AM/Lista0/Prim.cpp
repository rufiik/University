#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <functional>
#include "Prim.hpp"
#include "functions.hpp"

int main(int argc, char *argv[])
{
    std::string filename;
    if (argc != 2)
    {
        std::cout << "./randomTSP <filename> \n";
        return 1;
    }
    else
    {
        filename = argv[1];
    }
    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cout << "Nie można otworzyć pliku: " << filename << " !\n";
        return 1;
    }

    // Wczytaj dane
    std::string line;
    while (std::getline(file, line))
    {
        if (line == "NODE_COORD_SECTION")
        {
            break;
        }
    }

    int idx;
    double x, y;
    std::vector<coords> coordinations;
    while (file >> idx >> x >> y)
    {
        coordinations.push_back({x, y});
    }
    file.close();
    int n = coordinations.size();
    std::vector<std::vector<double>> matrix(n, std::vector<double>(n));

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            matrix[i][j] = distance(coordinations[i], coordinations[j]);
        }
    }
    // 1. Wyznacz MST
    primMST(matrix);
    auto mst_edges = primMST_edges(matrix);

    // 2. Zbuduj listę sąsiedztwa
    std::vector<std::vector<int>> adj(n);
    for (auto &e : mst_edges)
    {
        adj[e.first].push_back(e.second);
        adj[e.second].push_back(e.first);
    }

    // 3. DFS
    std::vector<int> tour;
    std::vector<bool> visited(n, false);
    dfs(0, adj, visited, tour);
    double cycle_weight = 0.0;
    for (int i = 0; i < n - 1; ++i)
        cycle_weight += matrix[tour[i]][tour[i + 1]];
    cycle_weight += matrix[tour[n - 1]][tour[0]];

    std::cout << "Cykl komiwojażera (DFS po MST): ";
    for (int v : tour)
        std::cout << v << " ";
    std::cout << tour[0] << "\n"; // zamknięcie cyklu
    std::cout << "Waga cyklu: " << cycle_weight << "\n";
    std::string out_name = filename;
    size_t pos = filename.find_last_of("/\\");
    if (pos != std::string::npos)
    {
        out_name = filename.substr(pos + 1);
    }
    std::ofstream primbest_file("best_MST_" + out_name);
    primbest_file << std::fixed << std::setprecision(4);

    // Zapisz cykl (indeksy od 1)
    for (int v : tour)
    {
        primbest_file << v + 1 << " ";
    }
    primbest_file << "\n";
    // Zapisz współrzędne w kolejności cyklu
    for (int v : tour)
    {
        primbest_file << coordinations[v].x << " " << coordinations[v].y << "\n";
    }

    primbest_file.close();
}