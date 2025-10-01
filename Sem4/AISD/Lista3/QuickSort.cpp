#include "Utils.hpp"

int Select(std::vector<int>& arr, int left, int right, int n) {
    if (left == right) {
        return arr[left];
    }
    std::vector<int> medians;
    for (int i = left; i <= right; i += 5) {
        int groupEnd = std::min(i + 4, right); 
        std::sort(arr.begin() + i, arr.begin() + groupEnd + 1); 
        int medianIndex = i + (groupEnd - i) / 2; 
        medians.push_back(arr[medianIndex]); 
    }
    int medianOfMedians;
    if (medians.size() == 1) {
        medianOfMedians = medians[0];
    } else {
        medianOfMedians = Select(medians, 0, medians.size() - 1, n);
    }
    return medianOfMedians; 
}

int SelectPartition(std::vector<int>& arr, int p, int q, int n){
    int i,j,pivot;
    int pivotValue = Select(arr, p, q, n);
    int pivotIndex = p;
    while (pivotIndex <= q && arr[pivotIndex] != pivotValue) {
        pivotIndex++;
    }
    pivot = arr[pivotIndex];
    swap(arr[pivotIndex], arr[q]);
    i=p-1;
    j=q+1;
    while (true) {
        do {
            i++;
        } while (compare_r(arr[i], pivot)); 

        do {
            j--;
        } while (compare_l(arr[j], pivot));
        if (compare_equal_l(i, j)) { 
            return j;
        }
        else{
           swap(arr[i], arr[j]);
        }
      
    }
    if(n<30) { 
        std::cerr << "Po zakończeniu SelectPartition z pivotem "<<pivot<<": ";
        printArray(arr);
    }
}
void QuickSort(std::vector<int>& arr, int p, int q, int n) {
    if (p < q) {
        int r = SelectPartition(arr, p, q, n);
        if(n<40){
            std::cerr << "Po podziale (pivot na pozycji " << r << "): ";
            printArray(arr);
        }
        QuickSort(arr, p, r, n);
        QuickSort(arr, r + 1, q, n);
    }

}
int main(int argc, char* argv[]) {
    int n;
    std::cin >> n;
    std::vector<int> arr(n);
    for (int i = 0; i < n; i++) {
        std::cin >> arr[i];
    }
    std::vector<int> originalArr = arr;
    std::chrono::time_point<std::chrono::high_resolution_clock> start;
    if (n < 40) {
        std::cerr << "Tablica wejściowa: ";
        printArray(arr);
        comparisonCount = 0;
        swapCount = 0;
        QuickSort(arr, 0, n - 1, n);
        std::cerr << "Tablica wejściowa: ";
        printArray(originalArr);
        std::cerr << "Tablica posortowana: ";
        printArray(arr);
    } else {
        comparisonCount = 0;
        swapCount = 0;
        start = std::chrono::high_resolution_clock::now();
        QuickSort(arr, 0, n - 1, n);
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
        std::cerr << "Liczba porównań: " << comparisonCount << std::endl;
        std::cerr << "Liczba zamian: " << swapCount << std::endl;
        std::cerr << "Czy tablica jest posortowana? " << (isSorted(arr, n) ? "Tak" : "Nie") << std::endl;
        std::cerr << "Czas wykonania: " << elapsed.count() << " s" << std::endl;

    return 0;
}