#include "Utils.hpp"
void merge(std::vector<int>& arr, int left, int mid, int right, int n) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    std::vector<int> L(n1), R(n2);
    for (int i = 0; i < n1; i++) {
        L[i] = arr[left + i];
    }
    for (int j = 0; j < n2; j++) {
        R[j] = arr[mid + 1 + j];
    }

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (compare_e(L[i], R[j])) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
        swapCount++;
    }

    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
        swapCount++;
    }

    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
        swapCount++;
    }
    if(n < 40) {
        std::cerr << "Po scaleniu (left=" << left << ", mid=" << mid << ", right=" << right << "): ";
        printArray(arr);
    }
}
std::vector<std::pair<int, int>> findIncreasingRuns(const std::vector<int>& arr) {
    std::vector<std::pair<int, int>> runs;
    int n = arr.size();
    int start = 0;

    for (int i = 1; i < n; i++) {
        if (compare_r(arr[i], arr[i - 1])) { 
            runs.push_back({start, i - 1});
            start = i;
        }
    }
    runs.push_back({start, n - 1}); 
    return runs;
}

void mergeRuns(std::vector<int>& arr, std::vector<std::pair<int, int>>& runs, int n) {
    while (runs.size() > 1) {
        std::vector<std::pair<int, int>> newRuns;

        for (size_t i = 0; i < runs.size(); i += 2) {
            if (i + 1 < runs.size()) {
                int left = runs[i].first;
                int mid = runs[i].second;
                int right = runs[i + 1].second;

                merge(arr, left, mid, right, n);
                newRuns.push_back({left, right});
            } else {
                newRuns.push_back(runs[i]); 
            }
        }

        runs = newRuns;
    }
}

void RunSort(std::vector<int>& arr, int n) {
    std::vector<std::pair<int, int>> runs = findIncreasingRuns(arr);
    mergeRuns(arr, runs, n);
}

int main(int argc, char* argv[]) {
    // if (argc < 2) {
    //     std::cerr << "Użycie: ./MyMergeSort <nazwa_pliku_wynikowego>" << std::endl;
    //     return 1;
    // }
    // std::string outputFileName = argv[1];
    int n;
    std::cin >> n;
    std::vector<int> arr(n);
    for (int i = 0; i < n; i++) {
        std::cin >> arr[i];
    }
    std::vector<int> originalArr = arr;
    if (n < 40) {
        std::cerr << "Tablica wejściowa: ";
        printArray(arr);
        comparisonCount = 0;
        swapCount = 0;
        RunSort(arr,n);
        std::cerr << "Tablica wejściowa: ";
        printArray(originalArr);
        std::cerr << "Tablica posortowana: ";
        printArray(arr);
    } else {
        comparisonCount = 0;
        swapCount = 0;
        RunSort(arr,n);
    }
        std::cerr << "Liczba porównań: " << comparisonCount << std::endl;
        std::cerr << "Liczba zamian: " << swapCount << std::endl;
        std::cerr << "Czy tablica jest posortowana? " << (isSorted(arr, n) ? "Tak" : "Nie") << std::endl;
    // std::ofstream outFile(outputFileName, std::ios::app); 
    // if (outFile.is_open()) {
    //     outFile << n << ", " << comparisonCount << ", " << swapCount << ", " << isSorted(arr, n) << std::endl;
    //     outFile.close();
    // } else {
    //     //std::cerr << "Nie można otworzyć pliku do zapisu!" << std::endl;
    // }

    return 0;
}