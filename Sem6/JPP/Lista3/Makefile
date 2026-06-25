CPP_DIR = CPP
ADA_DIR = ADA
D_DIR = D


CXX = g++
GNAT = gnatmake
GDC = gdc


CPP_SRC = $(CPP_DIR)/main.cpp $(CPP_DIR)/mathlib.cpp
CPP_SRC2 = $(CPP_DIR)/main2.cpp $(CPP_DIR)/mathlib.cpp
ADA_SRC = $(ADA_DIR)/main.adb
ADA_SRC2 = $(ADA_DIR)/main2.adb
D_SRC = $(D_DIR)/main.d $(D_DIR)/ring.d $(D_DIR)/rsa.d
D_SRC2 = $(D_DIR)/main2.d $(D_DIR)/ring.d $(D_DIR)/dh.d $(D_DIR)/user.d


CPP_OUT = $(CPP_DIR)/main
CPP_OUT2 = $(CPP_DIR)/main2
ADA_OUT = $(ADA_DIR)/main
ADA_OUT2 = $(ADA_DIR)/main2
D_OUT = $(D_DIR)/main
D_OUT2 = $(D_DIR)/main2

all: cpp cpp2 ada ada2 d d2

cpp: $(CPP_OUT)

$(CPP_OUT): $(CPP_SRC)
	$(CXX) $(CPP_SRC) -o $(CPP_OUT)
cpp2: $(CPP_OUT2)

$(CPP_OUT2): $(CPP_SRC2)
	$(CXX) $(CPP_SRC2) -o $(CPP_OUT2)
ada: $(ADA_OUT)

$(ADA_OUT): $(ADA_SRC)
	$(GNAT) -aO$(ADA_DIR) -aI$(ADA_DIR) -o $(ADA_OUT) $(ADA_SRC)

ada2: $(ADA_OUT2)

$(ADA_OUT2): $(ADA_SRC2)
	$(GNAT) -aO$(ADA_DIR) -aI$(ADA_DIR) -o $(ADA_OUT2) $(ADA_SRC2)
d: $(D_OUT)

$(D_OUT): $(D_SRC)
	$(GDC) -I$(D_DIR) $(D_SRC) -o $(D_OUT)
d2: $(D_OUT2)

$(D_OUT2): $(D_SRC2)
	$(GDC) -I$(D_DIR) $(D_SRC2) -o $(D_OUT2)

clean:
	rm -f $(CPP_OUT) $(CPP_OUT2) $(ADA_OUT) $(ADA_OUT2) $(D_OUT) $(D_OUT2)
	rm -f $(ADA_DIR)/*.ali $(ADA_DIR)/*.o
	rm -f *.ali *.o