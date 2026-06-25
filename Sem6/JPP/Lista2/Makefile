.PHONY: all d ada cpp clean

all: d ada cpp

# Ścieżki do katalogów
D_DIR = D
CPP_DIR = CPP
ADA_DIR = ADA

# Kompilacja D
d:
	cd $(D_DIR) && gdc test_ring.d ring.d -o test_ring

# Kompilacja Ada
ada:
	cd $(ADA_DIR) && gnatmake test_ring.adb mathlib.adb

# Kompilacja C++
cpp:
	cd $(CPP_DIR) && g++ -o test_ring test_ring.cpp mathlib.cpp

# Sprzątanie plików wynikowych
clean:
	cd $(D_DIR) && rm -f test_ring *.o *.ali
	cd $(ADA_DIR) && gnatclean test_ring
	cd $(CPP_DIR) && rm -f test_ring *.o