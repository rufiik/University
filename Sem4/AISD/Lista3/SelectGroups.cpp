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

int Select(std::vector<int>& arr, int left, int right, int k, int groupSize, int n) {
    if (left == right) {
        return arr[left];
    }

    // 1. Podziel tablicę na grupy o rozmiarze groupSize i znajdź mediany tych grup
    std::vector<int> medians;
    for (int i = left; i <= right; i += groupSize) {
        int groupEnd = std::min(i + groupSize - 1, right); // Koniec grupy (maksymalnie groupSize elementów)
        std::sort(arr.begin() + i, arr.begin() + groupEnd + 1); // Posortuj grupę
        int medianIndex = i + (groupEnd - i) / 2; // Znajdź indeks mediany
        medians.push_back(arr[medianIndex]); // Dodaj medianę do listy median
    }

    // 2. Znajdź medianę median rekurencyjnie
    int medianOfMedians;
    if (medians.size() == 1) {
        medianOfMedians = medians[0];
    } else {
        medianOfMedians = Select(medians, 0, medians.size() - 1, medians.size() / 2, groupSize, n);
    }

    // 3. Podziel tablicę wokół mediany median
    int pivotIndex = std::find(arr.begin() + left, arr.begin() + right + 1, medianOfMedians) - arr.begin();
    pivotIndex = Partition(arr, left, right, pivotIndex, n);
    int length = pivotIndex - left + 1;

    // 4. Rekurencyjnie znajdź k-tą statystykę pozycyjną
    if (k == length) {
        return arr[pivotIndex];
    } else if (k < length) {
        return Select(arr, left, pivotIndex - 1, k, groupSize, n);
    } else {
        return Select(arr, pivotIndex + 1, right, k - length, groupSize, n);
    }
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Użycie: ./Select <k> <groupSize>" << std::endl;
        return 1;
    }
    int statystyka;
    int k = std::stoi(argv[1]); // Odczytaj numer statystyki pozycyjnej
    int groupSize = std::stoi(argv[2]); // Odczytaj rozmiar grupy
    int n;
    std::cin >> n;
    std::vector<int> arr(n);
    for (int i = 0; i < n; i++) {
        std::cin >> arr[i];
    }
    std::vector<int> originalArr = arr; 
    if (k < 1 || k > n) {
        std::cerr << "Błąd: k musi być w zakresie od 1 do " << n << "." << std::endl;
        return 1;
    }
    if (n < 30) {
     
        std::cerr << "Tablica wejściowa: ";
        printArray(arr);
        comparisonCount = 0;
        swapCount = 0;
        statystyka = Select(arr, 0, n - 1, k, groupSize, n);
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
        Select(arr, 0, n - 1, k, groupSize, n);

    }
    std::cerr << k << " Statystyka pozycyjna: " << statystyka << std::endl;
        std::cerr << "Liczba porównań: " << comparisonCount << std::endl;
        std::cerr << "Liczba zamian: " << swapCount << std::endl;



    return 0;
}