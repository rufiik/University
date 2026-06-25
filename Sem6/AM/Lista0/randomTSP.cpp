#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <random>
#include <algorithm>
#include <cmath>
#include <numeric>
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
    // Inicjalizacja generatora liczb losowych
    std::random_device rd;
    std::mt19937 gen(rd());
    std::vector<int> permutation(n);
    std::iota(permutation.begin(), permutation.end(), 0);

    // Wektor do przechowywania długości 1000 losowych tras
    std::vector<int> route_lengths;
    route_lengths.reserve(1000);
    std::vector<int> best_permutation;
    int best_length = std::numeric_limits<int>::max();

    for (int i = 0; i < 1000; i++)
    {
        std::shuffle(permutation.begin(), permutation.end(), gen);
        int length = calculate_route_length(permutation, coordinations);
        route_lengths.push_back(length);

        if (length < best_length) {
            best_length = length;
            best_permutation = permutation; // zapisz najlepszą trasę
        }
    }

    std::vector<int> min10;
    std::vector<int> min50;
    min10.reserve(100);
    min50.reserve(20);

    //a) średnia z minimum dla każdych 10 kolejnych losowań (100 grup po 10 losowań)
    for(int groups=0;groups<100;groups++){
        auto start = route_lengths.begin() + groups*10;
        auto end = start+10;
        int min = *std::min_element(start,end);
        min10.push_back(min);
    }
    //b) średnia z minimum dla każdych 50 kolejnych losowań (20 grup po 50 losowań)
    for(int groups=0;groups<20;groups++){
        auto start = route_lengths.begin() + groups*50;
        auto end = start+50;
        int min = *std::min_element(start,end);
        min50.push_back(min);

    }
    //c) minimalna wartość dla tych 1000 losowań.
    int global_min = *std::min_element(route_lengths.begin(),route_lengths.end());
    int avg10 = std::accumulate(min10.begin(), min10.end(), 0.0) / min10.size();
    int avg50 = std::accumulate(min50.begin(),min50.end(),0.0) /min50.size();    
    // wyniki
    std::cout << std::fixed << std::setprecision(4);
    std::cout << "\n=== WYNIKI DLA " << filename << " ===\n";
    std::cout << "Liczba miast: " << n << "\n";
    std::cout << "Liczba losowych tras: 1000\n\n";
    std::cout << "a) Średnia z minimów w grupach po 10: " << avg10 << "\n";
    std::cout << "b) Średnia z minimów w grupach po 50: " << avg50 << "\n";
    std::cout << "c) Globalne minimum z 1000 losowań: " << global_min << "\n";
    // best do pliku
    std::string out_name = filename;
    size_t pos = filename.find_last_of("/\\");
    if (pos != std::string::npos) {
        out_name = filename.substr(pos + 1);
    }
    std::ofstream best_file("best_" + out_name);
    best_file << std::fixed << std::setprecision(4); 
    for (int idx : best_permutation) {
        best_file << idx+1 << " ";
    }
    best_file << "\n";
    for (int idx : best_permutation) {
        best_file << coordinations[idx].x << " " << coordinations[idx].y << "\n";
    }

    best_file.close();




    return 0;
}