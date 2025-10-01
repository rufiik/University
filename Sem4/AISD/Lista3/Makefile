# Kompilator i flagi
CXX = g++
CXXFLAGS = -std=c++11 -Wall

# Pliki źródłowe
UTILS = Utils.cpp
TARGETS = RandomSelect Select SelectGroups BinarySearch QuickSort DualPivotQS DataGenerator

# Reguła domyślna
all: $(TARGETS)

# Reguły dla każdego programu

DataGenerator: DataGenerator.cpp
	$(CXX) -o $@ $^ $(CXXFLAGS)
RandomSelect: RandomSelect.cpp $(UTILS)
	$(CXX) -o $@ $^ $(CXXFLAGS)
Select: Select.cpp $(UTILS)
	$(CXX) -o $@ $^ $(CXXFLAGS)
SelectGroups: SelectGroups.cpp $(UTILS)
	$(CXX) -o $@ $^ $(CXXFLAGS)
BinarySearch: BinarySearch.cpp $(UTILS)
	$(CXX) -o $@ $^ $(CXXFLAGS)
QuickSort: QuickSort.cpp $(UTILS)
	$(CXX) -o $@ $^ $(CXXFLAGS)
DualPivotQS: DualPivotQS.cpp $(UTILS)
	$(CXX) -o $@ $^ $(CXXFLAGS)
QuickSortold: QuickSortold.cpp $(UTILS)
	$(CXX) -o $@ $^ $(CXXFLAGS)
DualPivotQSold: DualPivotQSold.cpp $(UTILS)
	$(CXX) -o $@ $^ $(CXXFLAGS)
# Reguła czyszczenia plików binarnych
clean:
	rm -f $(TARGETS)