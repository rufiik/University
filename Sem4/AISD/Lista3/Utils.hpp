#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>
#include <vector>
#include <iomanip>
#include <fstream>
#include <random>
#include <algorithm>
#include <string>
#include <chrono>

extern long long comparisonCount;
extern long long swapCount;

bool isSorted(const std::vector<int>& arr, int n);
void printArray(const std::vector<int>& arr);
bool compare_l(int a, int b);
bool compare_r(int a, int b);
bool compare_equal_l(int a, int b);
bool compare_equal_r(int a, int b); 
void swap(int& a, int& b);

#endif