#include "Utils.hpp"

int Partition(std::vector<int>& arr, int left, int right, int pivotIndex, int n) {
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
    if (n<30) { 
        std::cerr << "Po zakończeniu Partition z pivotem "<<pivotValue<<": ";
        printArray(arr);
    }
    return storeIndex; 
}

int Select(std::vector<int>& arr, int left, int right, int k, int n) {
    if (left == right) {
        return arr[left]; 
    }

    // 1. Podziel tablicę na grupy po 5 elementów i znajdź mediany tych grup
    std::vector<int> medians;
    for (int i = left; i <= right; i += 5) {
        int groupEnd = std::min(i + 4, right); // Koniec grupy (maksymalnie 5 elementów)
        std::sort(arr.begin() + i, arr.begin() + groupEnd + 1); // Posortuj grupę
        int medianIndex = i + (groupEnd - i) / 2; // Znajdź indeks mediany
        medians.push_back(arr[medianIndex]); // Dodaj medianę do listy median
    }

   
    int medianOfMedians;
    if (medians.size() == 1) {
        medianOfMedians = medians[0];
    } else {
        medianOfMedians = Select(medians, 0, medians.size() - 1, medians.size() / 2, n);
    }

    int pivotIndex = std::find(arr.begin() + left, arr.begin() + right + 1, medianOfMedians) - arr.begin();
    pivotIndex = Partition(arr, left, right, pivotIndex, n);
    int length = pivotIndex - left + 1;
    if (k == length) {
        return arr[pivotIndex]; 
    } else if (k < length) {
        return Select(arr, left, pivotIndex - 1, k, n); 
    } else {
        return Select(arr, pivotIndex + 1, right, k - length, n); 
    }
}

int main(int argc, char* argv[]) {

    if (argc < 2) {
        std::cerr << "Użycie: ./Select <k>" << std::endl;
        return 1;
    }
    int statystyka;
    int k = std::stoi(argv[1]); // Odczytaj numer statystyki pozycyjnej
    int n;
    std::cin >> n;
    std::vector<int> arr(n);
    for (int i = 0; i < n; i++) {
        std::cin >> arr[i];
    }

    if (k < 1 || k > n) {
        std::cerr << "Błąd: k musi być w zakresie od 1 do " << n << "." << std::endl;
        return 1;
    }

    std::vector<int> originalArr = arr;
    if (n < 30) {
     
        std::cerr << "Tablica wejściowa: ";
        printArray(arr);
        comparisonCount = 0;
        swapCount = 0;
        statystyka = Select(arr, 0, n - 1, k, n);
        std::cerr << "Koncowy stan tablicy: ";
        printArray(arr);
        sort(arr.begin(), arr.end());
        std::cerr << "Tablica posortowana: ";
        printArray(arr);
        std::cerr << "Tablica wejściowa: ";
        printArray(originalArr);
    } else {
        comparisonCount = 0;
        swapCount = 0;
        Select(arr, 0, n - 1, k, n);

    }
    std::cerr << k << " Statystyka pozycyjna: " << statystyka << std::endl;
        std::cerr << "Liczba porównań: " << comparisonCount << std::endl;
        std::cerr << "Liczba zamian: " << swapCount << std::endl;
    return 0;
}