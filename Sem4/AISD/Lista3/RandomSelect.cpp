#include "Utils.hpp"



int RandomPartition(std::vector<int>& arr, int left, int right, int n) {
    std::random_device rd;
    std::mt19937 mt(rd()); 
    std::uniform_int_distribution<int> dist(left, right); 
    int pivotIndex = dist(mt);
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
        std::cerr << "Po zakończeniu RandomPartition z pivotem "<<pivotValue<<": ";
        printArray(arr);
    }
    return storeIndex; 
}
int RandomSelect(std::vector<int>& arr, int left, int right, int k, int n) {
    if (left == right) {
        return arr[left]; 
    }
    int pivotIndex = RandomPartition(arr, left, right,n); 
    int length = pivotIndex - left + 1; 

    if (length == k) {
        return arr[pivotIndex]; 
    } else if (k < length) {
        return RandomSelect(arr, left, pivotIndex - 1, k,n); 
    } else {
        return RandomSelect(arr, pivotIndex + 1, right, k - length,n); 
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Użycie: ./RandomSelect <k>" << std::endl;
        return 1;
    }

    int k = std::stoi(argv[1]); 
    int statystyka;
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
        statystyka = RandomSelect(arr, 0, n - 1, k,n);
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
        RandomSelect(arr, 0, n - 1, k,n);

    }
    std::cerr<<k<<" Statystyka pozycyjna: "<< statystyka<< std::endl;
        std::cerr << "Liczba porównań: " << comparisonCount << std::endl;
        std::cerr << "Liczba zamian: " << swapCount << std::endl;

    return 0;
}