#include "Utils.hpp"

int DualPivotPartition(std::vector<int>& arr, int low, int high, int& pivot1, int& pivot2) {
    int rand1 = low + rand() % (high - low + 1);
    int rand2 = low + rand() % (high - low + 1);

    while (rand1 == rand2) {
        rand2 = low + rand() % (high - low + 1);
    }
    swap(arr[rand1], arr[low]);
    swap(arr[rand2], arr[high]);
    if (compare_l(arr[low], arr[high])) {
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
        if(n<40){
            std::cerr << "Po podziale (low=" << low << ", high=" << high << "): ";
            printArray(arr);
            std::cerr << "Pivot1: " << pivot1 << ", Pivot2: " << pivot2 << std::endl;
        }
        DualPivotQuickSort(arr, low, lt - 1, n);
        DualPivotQuickSort(arr, lt + 1, high, n);
    }
}

int main(int argc, char* argv[]) {
    srand(time(0)); 
    if (argc < 2) {
        std::cerr << "Użycie: ./DualPivotQS <nazwa_pliku_wynikowego>" << std::endl;
        return 1;
    }
    std::string outputFileName = argv[1];
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
        // std::cerr << "Liczba porównań: " << comparisonCount << std::endl;
        // std::cerr << "Liczba zamian: " << swapCount << std::endl;
        // std::cerr << "Czy tablica jest posortowana? " << (isSorted(arr, n) ? "Tak" : "Nie") << std::endl;
        std::ofstream outFile(outputFileName, std::ios::app); 
        if (outFile.is_open()) {
            outFile << n << ", " << comparisonCount << ", " << swapCount << ", " << elapsed.count() << std::endl;
            outFile.close();
        } else {
            std::cerr << "Nie można otworzyć pliku do zapisu!" << std::endl;
        }

    return 0;
}