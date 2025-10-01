#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>
#include <vector>
#include <iomanip>
#include <fstream>
#include <random>
extern long long comparisonCount;
extern long long swapCount;
// Deklaracje funkcji
bool isSorted(const std::vector<int>& arr, int n);
void printArray(const std::vector<int>& arr);
bool compare_l(int a, int b);
bool compare_r(int a, int b);
bool compare_indices(int i, int j);
bool compare_e(int a, int b); 
void swap(int& a, int& b);

#endif