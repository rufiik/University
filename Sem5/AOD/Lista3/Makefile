# Makefile for compiling the project

# Kompilator i flagi
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2

# Pliki źródłowe i nagłówkowe
SRCS = main.cpp funkcje.cpp
HEADERS = graph.hpp

# Plik wynikowy
TARGET = main

# Reguła domyślna
all: $(TARGET)

# Kompilacja programu
$(TARGET): $(SRCS) $(HEADERS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS)

# Czyszczenie plików wynikowych
clean:
	rm -f $(TARGET)