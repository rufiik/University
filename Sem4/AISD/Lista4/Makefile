CXX = g++
CXXFLAGS = -std=c++17 -O2 -Wall

all: rb bst splay

rb: RBTree.cpp
	$(CXX) $(CXXFLAGS) -o rb RBTree.cpp

bst: BST.cpp
	$(CXX) $(CXXFLAGS) -o bst BST.cpp

splay: SplayTree.cpp
	$(CXX) $(CXXFLAGS) -o splay SplayTree.cpp

clean:
	rm -f rb bst splay