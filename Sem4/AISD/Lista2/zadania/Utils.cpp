#include "Utils.hpp"
#include <iostream>
#include <iomanip>
long long comparisonCount = 0;
long long swapCount = 0;
// Definicja funkcji isSorted
bool isSorted(const std::vector<int>& arr, int n) {
    for (int i = 0; i < n - 1; i++) {
        if (arr[i] > arr[i + 1]) {
            return false;
        }
    }
    return true;
}

// Definicja funkcji printArray
void printArray(const std::vector<int>& arr) {
    for (int num : arr) {
        std::cerr << std::setw(2) << std::setfill('0') << num << " ";
    }
    std::cerr << std::endl;
}
bool compare_l(int a, int b) {
    comparisonCount++;
    return a > b;
}
bool compare_r(int a, int b) {
    comparisonCount++;
    return a < b;
}
bool compare_indices(int i, int j) {
    comparisonCount++;
    return i >= j;
}
bool compare_e(int a, int b) {
    comparisonCount++;
    return a <= b; // Porównanie dla scalania
}
void swap(int& a, int& b) {
    swapCount++;
    int temp = a;
    a = b;
    b = temp;
}