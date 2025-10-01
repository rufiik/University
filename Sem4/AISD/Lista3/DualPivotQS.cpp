#include "Utils.hpp"
int Partition(std::vector<int>& arr, int left, int right, int pivotIndex) {
    int pivotValue = arr[pivotIndex];
    swap(arr[pivotIndex], arr[right]); 
    int storeIndex = left;
    for (int i = left; i < right; ++i) {
        if (compare_r(arr[i], pivotValue)) {
            swap(arr[storeIndex], arr[i]);
            storeIndex++;
        }
    }
    swap(arr[storeIndex], arr[right]);
    return storeIndex; 
}

int Select(std::vector<int>& arr, int left, int right, int k) {
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
        medianOfMedians = Select(medians, 0, medians.size() - 1, medians.size() / 2);
    }
    int pivotIndex = std::find(arr.begin() + left, arr.begin() + right + 1, medianOfMedians) - arr.begin();
    pivotIndex = Partition(arr, left, right, pivotIndex);
    int length = pivotIndex - left + 1;
    if (k == length) {
        return arr[pivotIndex]; 
    } else if (k < length) {
        return Select(arr, left, pivotIndex - 1, k); 
    } else {
        return Select(arr, pivotIndex + 1, right, k - length); 
    }
}

int DualPivotPartition(std::vector<int>& arr, int low, int high, int& pivot1, int& pivot2) {

    int n = high - low ;
    int k1 = n / 3;
    int k2 = 2 * n / 3;
    if (k1 == 0) k1 = 1;
    if (k2 == 0) k2 = 1;

    int pivotVal1 = Select(arr, low, high, k1);
    int pivotVal2 = Select(arr, low, high, k2);

    auto it1 = std::find(arr.begin() + low, arr.begin() + high + 1, pivotVal1);
    auto it2 = std::find(arr.begin() + low, arr.begin() + high + 1, pivotVal2);
    int pivotIndex1 = it1 - arr.begin();
    int pivotIndex2 = it2 - arr.begin();
    
    swap(arr[pivotIndex1], arr[low]);
    swap(arr[pivotIndex2], arr[high]);
    if(compare_l(arr[low], arr[high])) {
        swap(arr[low], arr[high]);
    }

    pivot1 = arr[low];
    pivot2 = arr[high];

    int i = low + 1, lt = low + 1, gt = high - 1;

    int smallCount = 0; 
    int largeCount = 0; 

    while (i <= gt) {
        if (largeCount > smallCount) {
            if (compare_l(arr[i], pivot2)) {
                swap(arr[i], arr[gt]);
                gt--;
                largeCount++;
            } else if (compare_r(arr[i], pivot1)) {
                swap(arr[i], arr[lt]);
                lt++;
                i++;
                smallCount++;
            } else {
                i++;
            }
        } else {
            if (compare_r(arr[i], pivot1)) {
                swap(arr[i], arr[lt]);
                lt++;
                i++;
                smallCount++;
            } else if (compare_l(arr[i], pivot2)) {
                swap(arr[i], arr[gt]);
                gt--;
                largeCount++;
            } else {
                i++;
            }
        }
    }

    lt--;
    gt++;
    swap(arr[low], arr[lt]);
    swap(arr[high], arr[gt]);

    return lt; 
}

void DualPivotQuickSort(std::vector<int>& arr, int low, int high, int n) {
    if (low < high) {
        int pivot1, pivot2;
        int lt = DualPivotPartition(arr, low, high, pivot1, pivot2);
        if(n<30){
            std::cerr << "Po podziale (low=" << low << ", high=" << high << "): ";
            printArray(arr);
            std::cerr << "Pivot1: " << pivot1 << ", Pivot2: " << pivot2 << std::endl;
        }
        DualPivotQuickSort(arr, low, lt - 1, n);
        DualPivotQuickSort(arr, lt + 1, high, n);
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
        DualPivotQuickSort(arr, 0, n - 1, n);
        std::cerr << "Tablica wejściowa: ";
        printArray(originalArr);
        std::cerr << "Tablica posortowana: ";
        printArray(arr);
    } else {

        comparisonCount = 0;
        swapCount = 0;
        start = std::chrono::high_resolution_clock::now();
        DualPivotQuickSort(arr, 0, n - 1, n);
    }
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end - start;

        std::cerr << "Liczba porównań: " << comparisonCount << std::endl;
        std::cerr << "Liczba zamian: " << swapCount << std::endl;
        std::cerr << "Czy tablica jest posortowana? " << (isSorted(arr, n) ? "Tak" : "Nie") << std::endl;

    return 0;
}