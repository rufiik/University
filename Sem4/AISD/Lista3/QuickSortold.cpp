#include "Utils.hpp"
  
int HoarePartition(std::vector<int>& arr, int p, int q){
    int i,j,pivot;
    pivot=arr[p];
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
}
void QuickSort(std::vector<int>& arr, int p, int q, int n) {
    if (p < q) {
        int r = HoarePartition(arr, p, q);
        if(n<40){
            std::cerr << "Po podziale (pivot na pozycji " << r << "): ";
            printArray(arr);
        }
        QuickSort(arr, p, r, n);
        QuickSort(arr, r + 1, q, n);
    }

}
int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Użycie: ./QuickSort <nazwa_pliku_wynikowego>" << std::endl;
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