#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP
#include <cmath>
#include <vector>
struct coords
{
    double x;
    double y;
};

// Funkcja do obliczania odległości euklidesowej między dwoma punktami
int distance(const coords &a, const coords &b) {
    double dx = a.x - b.x;
    double dy = a.y - b.y;
    return static_cast<int>(std::ceil(std::sqrt(dx*dx + dy*dy)));
}
// Funkcja do obliczania długości całej trasy (permutacji)
int calculate_route_length(const std::vector<int> &permutation, const std::vector<coords> &points)
{
    int total = 0.0;
    int n = permutation.size();

    for(int i=0; i<n-1; i++){
        total += distance(points[permutation[i]], points[permutation[i+1]]);
    }
    total += distance(points[permutation[n - 1]], points[permutation[0]]);
    return total;
}
#endif 