CXX = g++
CXXFLAGS = -O2 -std=c++17 -Wall -Wextra

# znajdź wszystkie pliki .cpp w katalogu bieżącym i jednym poziomie podkatalogów
SRCS := $(shell find . -maxdepth 2 -type f -name '*.cpp' -printf '%P\n')

# docelowe pliki wykonywalne = ścieżka źródła bez rozszerzenia .cpp
BINS := $(SRCS:.cpp=)

all: $(BINS)

# reguła budująca ./dir/name z ./dir/name.cpp
%: %.cpp
	@echo "Compiling $< -> $@"
	$(CXX) $(CXXFLAGS) -o $@ $<

clean:
	@echo "Cleaning executables..."
	-rm -f $(BINS)

.PHONY: all clean