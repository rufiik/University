#include "Utils.hpp"

void InsertionSort(std::vector<int>& arr,int n){
    int key,i;
    for(int j=2;j<n;j++){
        key=arr[j];
        i=j-1;
        while(i>=0 && compare_l(arr[i],key)){
            swap(arr[i+1], arr[i]); 
            i-=1;
        }
        arr[i+1]=key;
        if(n<40){
            std::cerr << "Po wstawieniu " << key << " na pozycję " << i+1 << ": ";
            printArray(arr);
        }
    }
}

int HoarePartition(std::vector<int>& arr, int p, int q){
    int i,j,pivot;
    pivot=arr[(p+q)/2];
    i=p-1;
    j=q+1;
    while (true) {
        do {
            i++;
        } while (compare_r(arr[i], pivot)); 

        do {
            j--;
        } while (compare_l(arr[j], pivot));
        if (compare_indices(i, j)) { 
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
    // if (argc < 2) {
    //     std::cerr << "Użycie: ./HybridSort <nazwa_pliku_wynikowego>" << std::endl;
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
        if(n<=10){
            InsertionSort(arr,n);
        }
        else{
            QuickSort(arr, 0, n - 1, n);
        }
        std::cerr << "Tablica wejściowa: ";
        printArray(originalArr);
        std::cerr << "Tablica posortowana: ";
        printArray(arr);
        std::cerr << "Liczba porównań: " << comparisonCount << std::endl;
        std::cerr << "Liczba zamian: " << swapCount << std::endl;
        std::cerr << "Czy tablica jest posortowana? " << (isSorted(arr, n) ? "Tak" : "Nie") << std::endl;

    } else {
        comparisonCount = 0;
        swapCount = 0;
        QuickSort(arr, 0, n - 1, n);
        std::cerr << "Liczba porównań: " << comparisonCount << std::endl;
        std::cerr << "Liczba zamian: " << swapCount << std::endl;
        std::cerr << "Czy tablica jest posortowana? " << (isSorted(arr, n) ? "Tak" : "Nie") << std::endl;
    }
    // std::ofstream outFile(outputFileName, std::ios::app); 
    // if (outFile.is_open()) {
    //     outFile << n << ", " << comparisonCount << ", " << swapCount << ", " << isSorted(arr, n) << std::endl;
    //     outFile.close();
    // } else {
    //     std::cerr << "Nie można otworzyć pliku do zapisu!" << std::endl;
    // }

    return 0;
}