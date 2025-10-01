#include "Utils.hpp"
void InsertionSort(std::vector<int>& arr,int n){
    int key,i;
    for(int j=1;j<n;j++){
        key=arr[j];
        i=j-1;
        while(i>=0 && compare_l(arr[i],key)){
            swap(arr[i+1], arr[i]); 
            i-=1;
        }
        arr[i+1]=key;
        if(n<40){
        std::cerr << "Stan tablicy po iteracji " << j << ": ";
        printArray(arr);
        }
    }
}
bool isSorted(std::vector<int>& arr, int n) {
    for (int i = 0; i < n - 1; i++) {
        if (arr[i] > arr[i + 1]) {
            return 0;
        }
    }
    return 1;
}
int main(int argc, char* argv[]) {
    // if (argc < 2) {
    //     std::cerr << "Użycie: ./InsertionSort <nazwa_pliku_wynikowego>" << std::endl;
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
        InsertionSort(arr, n);
        std::cerr << "Tablica wejściowa: ";
        printArray(originalArr);
        std::cerr << "Tablica posortowana: ";
        printArray(arr);
    } else {
        comparisonCount = 0;
        swapCount = 0;
        InsertionSort(arr, n);
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