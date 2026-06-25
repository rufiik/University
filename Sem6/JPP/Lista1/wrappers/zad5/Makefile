# Kompilator i flagi
ADA_COMPILER = gnatmake
LDFLAGS_DYNAMIC = -largs -L../../C -lmathC -L../../ADA -lmathADA -L../../GO -lmathGO
LDFLAGS_STATIC = -largs ../../C/libmathC.a ../../ADA/libmathADA.a ../../GO/libmathGO.a

# Nazwy wynikowych programów
OUTPUT_DYNAMIC = test_program_dynamic
OUTPUT_STATIC = test_program_static

# Plik źródłowy Ada
SRC = test_all.adb

# Domyślny cel
all: $(OUTPUT_DYNAMIC) $(OUTPUT_STATIC)

# Budowanie programu dynamicznego
$(OUTPUT_DYNAMIC): $(SRC)
	$(ADA_COMPILER) $(SRC) -o $@ $(LDFLAGS_DYNAMIC)

# Budowanie programu statycznego
$(OUTPUT_STATIC): $(SRC)
	$(ADA_COMPILER) $(SRC) -o $@ $(LDFLAGS_STATIC)

# Czyszczenie plików wynikowych
clean:
	rm -f $(OUTPUT_DYNAMIC) $(OUTPUT_STATIC) *.ali *.o