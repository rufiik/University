#include <iostream>
#include <vector>
#include <climits>
#include <bits/stdc++.h>

struct BinomialNode {
    int key;
    int degree;
    BinomialNode* parent;
    BinomialNode* child;
    BinomialNode* sibling;

    BinomialNode(int k) : key(k), degree(0), parent(nullptr), child(nullptr), sibling(nullptr) {}
};

class BinomialHeap {
    BinomialNode* head;
    int comparisions = 0;

    // Łączy dwa drzewa tego samego stopnia
    BinomialNode* mergeTrees(BinomialNode* a, BinomialNode* b) {
        comparisions++;
        if (a->key > b->key) std::swap(a, b);
        b->parent = a;
        b->sibling = a->child;
        a->child = b;
        a->degree++;
        return a;
    }

    // Łączy dwa kopce
    BinomialNode* merge(BinomialNode* h1, BinomialNode* h2) {
        if (!h1) return h2;
        if (!h2) return h1;
        BinomialNode* res = nullptr;
        BinomialNode** pos = &res;

        BinomialNode* a = h1;
        BinomialNode* b = h2;
        while (a && b) {
            if (a->degree < b->degree) {
                *pos = a;
                a = a->sibling;
            } else {
                *pos = b;
                b = b->sibling;
            }
            pos = &((*pos)->sibling);
        }
        *pos = (a ? a : b);
        return res;
    }

public:
        BinomialHeap() : head(nullptr), comparisions(0) {}
    int getComparisions() const {
        return comparisions;
    }
    void resetComparisions() {
        comparisions = 0;
    }
    // Wstawianie
    void insert(int key) {
        BinomialNode* node = new BinomialNode(key);
        node->sibling = nullptr; // ważne!
        head = unionHeaps(head, node);
    }
       void unionWith(const BinomialHeap& other) {
        head = unionHeaps(head, other.head);
    }
    void printRootTree() {
        printTree(head);
        std::cout << "\n";
    }
    void printHeads() {
    BinomialNode* curr = head;
    std::cout << "HEADS: ";
    while (curr) {
        std::cout << curr->key << "(deg:" << curr->degree << ") ";
        curr = curr->sibling;
    }
    std::cout << "\n";
}
    // Łączenie kopców
    BinomialNode* unionHeaps(BinomialNode* h1, BinomialNode* h2) {
        BinomialNode* newHead = merge(h1, h2);
        if (!newHead) return nullptr;

        BinomialNode* prev = nullptr;
        BinomialNode* curr = newHead;
        BinomialNode* next = curr->sibling;

        while (next) {
            // porównanie stopni, nie kluczy
            if (curr->degree != next->degree ||
                (next->sibling && next->sibling->degree == curr->degree)) {
                prev = curr;
                curr = next;
            } else {
                comparisions++; // porównanie kluczy curr->key <= next->key
                if (curr->key <= next->key) {
                    curr->sibling = next->sibling;
                    curr = mergeTrees(curr, next);
                } else {
                    if (prev) prev->sibling = next;
                    else newHead = next;
                    curr = mergeTrees(next, curr);
                }
            }
            next = curr->sibling;
        }
        return newHead;
    }

    // Znajdź minimum
    int getMin() {
        if (!head) return INT_MAX;
        int minVal = head->key;
        BinomialNode* curr = head->sibling;
        while (curr) {
            comparisions++;
            if (curr->key < minVal) minVal = curr->key;
            curr = curr->sibling;
        }
        return minVal;
    }

    // Usuń minimum
    void extractMin() {
        if (!head) return;
        BinomialNode* minNode = head;
        BinomialNode* minPrev = nullptr;
        BinomialNode* prev = nullptr;
        BinomialNode* curr = head;
        int minVal = head->key;

        while (curr) {
            comparisions++;
            if (curr->key < minVal) {
                minVal = curr->key;
                minNode = curr;
                minPrev = prev;
            }
            prev = curr;
            curr = curr->sibling;
        }

        // Usuń minNode z listy
        if (minPrev) minPrev->sibling = minNode->sibling;
        else head = minNode->sibling;

        // Odwróć listę dzieci minNode
        BinomialNode* child = minNode->child;
        BinomialNode* rev = nullptr;
        while (child) {
            BinomialNode* next = child->sibling;
            child->sibling = rev;
            child->parent = nullptr;
            rev = child;
            child = next;
        }

        head = unionHeaps(head, rev);
        delete minNode;
    }

    // Debug: wypisz kopiec
    void print() {
        BinomialNode* curr = head;
        while (curr) {
            std::cout << "Drzewo stopnia " << curr->degree << ": ";
            printSingleTree(curr);
            std::cout << "\n";
            curr = curr->sibling;
        }
    }
    void printSingleTree(BinomialNode* node) {
        if (!node) return;
        std::cout << node->key << " ";
        BinomialNode* child = node->child;
        while (child) {
            printSingleTree(child);
            child = child->sibling;
        }
    }

    void printTree(BinomialNode* node) {
        if (!node) return;
        std::cout << node->key << " ";
        printTree(node->child);
        printTree(node->sibling);
    }
};
BinomialHeap makeHeap() {
    return BinomialHeap();
}
 int main() {
    const int n = 500;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(1, 1000000);

    // 1. Utwórz dwa puste kopce
    BinomialHeap H1 = makeHeap();
    BinomialHeap H2 = makeHeap();

    // 2. Do każdego kopca wstaw losowy ciąg elementów długości n
    std::vector<int> seq1, seq2;
    for (int i = 0; i < n; ++i) {
        int val1 = dist(gen);
        int val2 = dist(gen);
        seq1.push_back(val1);
        seq2.push_back(val2);
        H1.insert(val1);
        H2.insert(val2);
    }

    // 3. Scal H1 i H2 w jeden kopiec H
    BinomialHeap H = makeHeap();
    // Union: H = H1 ∪ H2
    // (w tej implementacji możesz zrobić: H = H1; H.head = H.unionHeaps(H1.head, H2.head);)
    H = H1;
    H.unionWith(H2);

    // 4. Wykonaj 2n razy Extract-Min i sprawdź, czy wyniki są posortowane
    std::vector<int> extracted;
    for (int i = 0; i < 2 * n; ++i) {
        int minVal = H.getMin();
        if (minVal == INT_MAX) break; // kopiec pusty
        extracted.push_back(minVal);
        H.extractMin();
    }
    bool sorted = std::is_sorted(extracted.begin(), extracted.end());
    std::cout << "Czy ciąg usuwanych elementów jest posortowany? " << (sorted ? "TAK" : "NIE") << "\n";
    std::cout << "Liczba usuniętych elementów: " << extracted.size() << "\n";
    std::cout << "Czy kopiec pusty po wszystkich Extract-Min? " << (H.getMin() == INT_MAX ? "TAK" : "NIE") << "\n";
    std::cout << "Porównań przy całości: " << H.getComparisions() << "\n";
    return 0;
    
       // std::ofstream out("total_comparisons_vs_n1.csv");
    // out << "n,total_comparisons\n";
    // std::random_device rd;
    // std::mt19937 gen(rd());
    // std::uniform_int_distribution<> dist(1, 1000000);
    //     int experiments = 5; // lub więcej dla uśrednienia
    // for (int n = 10000; n <= 10000; n += 100) {
    //     for (int exp = 0; exp < experiments; ++exp) {
    //     int total_comparisons = 0;
        



    //         BinomialHeap H1, H2;
    //         for (int i = 0; i < n; ++i) {
    //             H1.insert(dist(gen));
    //             total_comparisons += H1.getComparisions();
    //             H1.resetComparisions();
    //             H2.insert(dist(gen));
    //             total_comparisons += H2.getComparisions();
    //             H2.resetComparisions();
    //         }
    //         BinomialHeap H = H1;
    //         H.unionWith(H2);
    //         total_comparisons += H.getComparisions();
    //         H.resetComparisions();

    //         for (int i = 0; i < 2 * n; ++i) {
    //             int minVal = H.getMin();
    //             total_comparisons += H.getComparisions();
    //             H.resetComparisions();
    //             if (minVal == INT_MAX) break;
    //             H.extractMin();
    //             total_comparisons += H.getComparisions();
    //             H.resetComparisions();
    //         }
       
    //     out << n << "," << total_comparisons << "\n";
    //      }
    // }
    // out.close();
    // return 0;
}
