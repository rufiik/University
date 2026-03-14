# Kompilator i flagi
CXX = g++
CXXFLAGS = -std=c++23 -Wall -Wextra -O2

# Pliki źródłowe i nagłówkowe
HYPERCUBE_SRC = HypercubeGraph.cpp
HYPERCUBE_HDR = HypercubeGraph.hpp
BIPARTITE_SRC = BipartiteGraph.cpp
BIPARTITE_HDR = BipartiteGraph.hpp

# Programy
PROGRAMS = zad1 zad2 zad4

# Reguła domyślna
all: $(PROGRAMS)

# Reguły dla każdego programu
zad1: zad1.cpp $(HYPERCUBE_SRC) $(HYPERCUBE_HDR)
	$(CXX) $(CXXFLAGS) -o $@ zad1.cpp $(HYPERCUBE_SRC)

zad2: zad2.cpp $(BIPARTITE_SRC) $(BIPARTITE_HDR)
	$(CXX) $(CXXFLAGS) -o $@ zad2.cpp $(BIPARTITE_SRC)

zad4: zad4.cpp $(HYPERCUBE_SRC) $(HYPERCUBE_HDR)
	$(CXX) $(CXXFLAGS) -o $@ zad4.cpp $(HYPERCUBE_SRC)

# Reguła czyszczenia plików wynikowych
clean:
	rm -f $(PROGRAMS)