# Kompilatory
CC = gcc
GNATMAKE = gnatmake
GO = go

# Flagi
CFLAGS = -c -fPIC
ADAFLAGS = -c -fPIC

# Ścieżki do folderów
C_DIR = C
ADA_DIR = ADA
GO_DIR = GO

# Nazwy bibliotek
LIB_C_STATIC = libmathC.a
LIB_C_SHARED = libmathC.so
LIB_ADA_STATIC = libmathADA.a
LIB_ADA_SHARED = libmathADA.so
LIB_GO_STATIC = libmathGO.a
LIB_GO_SHARED = libmathGO.so

# Pliki źródłowe (z ścieżkami)
C_SRC = $(C_DIR)/mathlib.c
ADA_SRC = $(ADA_DIR)/mathlib.adb

# Obiekty (w swoich katalogach)
C_OBJ = $(C_DIR)/mathlib.o
ADA_OBJ = $(ADA_DIR)/mathlib.o
ADA_ALI = $(ADA_DIR)/mathlib.ali

# Cele domyślne
all: c_libs ada_libs go_libs

# ============================================
# C biblioteki
# ============================================
c_libs: $(LIB_C_STATIC) $(LIB_C_SHARED)

$(LIB_C_STATIC): $(C_OBJ)
	cp $< .
	ar rcs $@ mathlib.o
	rm -f mathlib.o

$(LIB_C_SHARED): $(C_OBJ)
	cp $< .
	gcc -shared -o $@ mathlib.o
	rm -f mathlib.o

$(C_OBJ): $(C_SRC)
	$(CC) $(CFLAGS) -o $@ $^

# ============================================
# Ada biblioteki
# ============================================
ada_libs: $(LIB_ADA_STATIC) $(LIB_ADA_SHARED)

$(LIB_ADA_STATIC): $(ADA_OBJ)
	cp $< .
	ar rcs $@ mathlib.o
	rm -f mathlib.o

$(LIB_ADA_SHARED): $(ADA_OBJ) $(ADA_ALI)
	cd $(ADA_DIR) && gnatbind -n mathlib.ali
	cd $(ADA_DIR) && gnatlink mathlib.ali -shared -o ../$@

$(ADA_OBJ) $(ADA_ALI): $(ADA_SRC)
	cd $(ADA_DIR) && $(GNATMAKE) $(ADAFLAGS) mathlib.adb

# ============================================
# Go biblioteki
# ============================================
go_libs: go_mod_init $(LIB_GO_STATIC) $(LIB_GO_SHARED)

$(LIB_GO_SHARED):
	cd $(GO_DIR) && $(GO) build -buildmode=c-shared -o ../$@ main.go

$(LIB_GO_STATIC):
	cd $(GO_DIR) && $(GO) build -buildmode=c-archive -o ../$@ main.go

# Inicjalizacja modułu Go (pierwsze użycie)
go_mod_init:
	cd $(GO_DIR) && $(GO) mod init mathl 2>/dev/null || true

clean:
	# C clean
	rm -f $(C_DIR)/*.o $(C_DIR)/*.a $(C_DIR)/*.so
	rm -f libmathC.*
	
	# Ada clean
	rm -f $(ADA_DIR)/*.o $(ADA_DIR)/*.ali $(ADA_DIR)/b~*.adb $(ADA_DIR)/b~*.ali
	rm -f libmathADA.*
	
	# Go clean
	rm -f $(LIB_GO_STATIC) $(LIB_GO_SHARED)
	cd $(GO_DIR) && rm -f go.mod 2>/dev/null || true
	
	# Główne pliki
	rm -f mathlib.o

help:
	@echo "  all        - buduje wszystkie biblioteki (domyślny)"
	@echo "  c_libs     - buduje biblioteki C"
	@echo "  ada_libs   - buduje biblioteki Ada"
	@echo "  go_libs    - buduje biblioteki Go"
	@echo "  clean      - usuwa wszystkie pliki tymczasowe i biblioteki"
	@echo "  go_mod_init - inicjalizuje moduł Go (pierwsze użycie)"