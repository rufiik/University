#include "Utils.hpp"

bool BinarySearch(const std::vector<int>& arr, int target, int left, int right) {
    if (left> right) {
        return false;
    }
    int mid=left+(right-left)/2;
    if(target == arr[mid]) return true;
    else if( compare_r(target , arr[mid])) return BinarySearch(arr, target, left, mid-1);
    else return BinarySearch(arr, target, mid+1, right);
}



int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Użycie: ./BinarySearch <v> " << std::endl;
        return 1;
    }
    int v = std::stoi(argv[1]); 
    bool found = false;
    int n;
    std::cin >> n;
    std::vector<int> arr(n);
    for (int i = 0; i < n; i++) {
        std::cin >> arr[i];
    }
    std::uniform_int_distribution<int> dist(0,n-1);
   
    comparisonCount = 0; // Zresetuj licznik porównań
    auto start = std::chrono::high_resolution_clock::now();
    found = BinarySearch(arr, v, 0, n-1); 
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    // Wyświetl wyniki
    sort(arr.begin(), arr.end());
    std::cerr << "Tablica posortowana: ";
    printArray(arr);
    std::cout << "Znaleziono: " << found << std::endl;
    std::cout << "Liczba porównań: " << comparisonCount << std::endl;
    std::cout << "Czas wykonania: " << elapsed.count() << " s" << std::endl;


}