# Kompilator Go i flagi
GO = go
LDFLAGS_STATIC = -ldflags '-extldflags "-static ../../C/libmathC.a ../../ADA/libmathADA.a"'

# Nazwy wynikowych programów
OUTPUT_DYNAMIC = test_program_dynamic
OUTPUT_STATIC = test_program_static

# Pliki źródłowe
SRC = main.go c_wrapper.go ada_wrapper.go

# Domyślny cel
all: $(OUTPUT_DYNAMIC) $(OUTPUT_STATIC)

# Budowanie programu dynamicznego
$(OUTPUT_DYNAMIC): $(SRC)
	$(GO) build -o $@ $(SRC)

# Budowanie programu statycznego
$(OUTPUT_STATIC): $(SRC)
	$(GO) build -o $@ $(LDFLAGS_STATIC) $(SRC)

# Czyszczenie plików wynikowych
clean:
	rm -f $(OUTPUT_DYNAMIC) $(OUTPUT_STATIC)