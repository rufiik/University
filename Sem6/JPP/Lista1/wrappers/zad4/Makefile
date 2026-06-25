# Kompilator i flagi
CC = gcc
CFLAGS = -fPIC
LDFLAGS_DYNAMIC = -ldl -lpthread
LDFLAGS_STATIC = -static

# Ścieżki do bibliotek
LIB_C = ../../C
LIB_ADA = ../../ADA
LIB_GO = ../../GO

# Pliki obiektowe
OBJS = main.c go_wrapper.o ada_wrapper.o

# Nazwy wynikowych programów
OUTPUT_DYNAMIC = test_program_dynamic
OUTPUT_STATIC = test_program_static

# Domyślny cel
all: $(OUTPUT_DYNAMIC) $(OUTPUT_STATIC)

# Budowanie programu dynamicznego
$(OUTPUT_DYNAMIC): $(OBJS)
	$(CC) $(OBJS) \
        -L$(LIB_C) -lmathC \
        -L$(LIB_ADA) -lmathADA \
        -L$(LIB_GO) -lmathGO \
        -o $@ $(LDFLAGS_DYNAMIC)

# Budowanie programu statycznego
$(OUTPUT_STATIC): $(OBJS)
	$(CC) $(LDFLAGS_STATIC) $(OBJS) \
        $(LIB_C)/libmathC.a \
        $(LIB_ADA)/libmathADA.a \
        $(LIB_GO)/libmathGO.a \
        -o $@

# Czyszczenie plików wynikowych
clean:
	rm -f $(OUTPUT_DYNAMIC) $(OUTPUT_STATIC)