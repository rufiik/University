# Kompilator i flagi
CXX = g++
CXXFLAGS = -std=c++11 -Wall

# Pliki źródłowe
UTILS = Utils.cpp
TARGETS = InsertionSort QuickSort MergeSort HybridSort DualPivotQS MyMergeSort DataGenerator

# Reguła domyślna
all: $(TARGETS)

# Reguły dla każdego programu
InsertionSort: InsertionSort.cpp $(UTILS)
	$(CXX) -o $@ $^ $(CXXFLAGS)
QuickSort: QuickSort.cpp $(UTILS)
	$(CXX) -o $@ $^ $(CXXFLAGS)
MergeSort: MergeSort.cpp $(UTILS)
	$(CXX) -o $@ $^ $(CXXFLAGS)
HybridSort: HybridSort.cpp $(UTILS)
	$(CXX) -o $@ $^ $(CXXFLAGS)
DualPivotQS: DualPivotQS.cpp $(UTILS)
	$(CXX) -o $@ $^ $(CXXFLAGS)
MyMergeSort: MyMergeSort.cpp $(UTILS)
	$(CXX) -o $@ $^ $(CXXFLAGS)
DataGenerator: DataGenerator.cpp
	$(CXX) -o $@ $^ $(CXXFLAGS)
# Reguła czyszczenia plików binarnych
clean:
	rm -f $(TARGETS)