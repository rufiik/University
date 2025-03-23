#include <cstdlib>
#include <iostream>
#include <ctime>
#include <fstream>

struct Node {
    int data;
    Node* next;
    Node* prev;  
    Node(int value) : data(value), next(nullptr), prev(nullptr) {}
};

struct TwoWayList {
    Node* head;
    int size;

    TwoWayList() : head(nullptr), size(0) {}
};

void insert(TwoWayList& list, int value) {
    Node* newNode = new Node(value);
    if (list.head == nullptr) {
        list.head = newNode;
        newNode->next = newNode;
        newNode->prev = newNode;
    } else {
        Node* temp = list.head;
        while (temp->next != list.head) {
            temp = temp->next;
        }
        temp->next = newNode;
        newNode->prev = temp;
        newNode->next = list.head;
        list.head->prev = newNode;
    }
    list.size++;
}

void print(const TwoWayList& list) {
    if (list.head == nullptr) {
        std::cout << "List is empty." << std::endl;
        return;
    }

    Node* temp = list.head;
    do {
        std::cout << temp->data << " ";
        temp = temp->next;
    } while (temp != list.head);
    std::cout << std::endl;
}

void merge(TwoWayList& l1, TwoWayList& l2) {
    if (l1.head == nullptr) {
        l1.head = l2.head;
        l1.size = l2.size;
        return;
    }
    if (l2.head == nullptr) {
        return;
    }

    Node* temp1 = l1.head->prev;
    Node* temp2 = l2.head->prev;

    temp1->next = l2.head;
    l2.head->prev = temp1;

    temp2->next = l1.head;
    l1.head->prev = temp2;

    l1.size += l2.size;
}

void clear(TwoWayList& list) {
    if (list.head == nullptr) {
        return;
    }

    Node* temp = list.head;
    Node* temp2 = nullptr;

    do {
        temp2 = temp->next;
        delete temp;
        temp = temp2;
    } while (temp != list.head);

    list.head = nullptr;
    list.size = 0;
}

int main() {
    srand(time(NULL));
    TwoWayList list1;
    TwoWayList list2;
    for(int i = 20; i < 30; i++) {
        insert(list2, i);
    }
    for(int i = 30; i < 40; i++) {
        insert(list1, i);
    }
    merge(list1, list2);
    print(list1);
    print(list2);

    int T[10000];
    TwoWayList L;

    for(int i = 0; i < 10000; i++) {
        T[i] = std::rand() % 100001;
    }

    for(int i = 0; i < 10000; i++) {
        insert(L, T[i]);
    }

    int number = 0;
    int counter = 0;
    int total_comparisons = 0;
    std::ofstream file("wyniki3.txt");
    if(file.is_open()) {
        for(int i = 0; i < 1000; i++) {
            number = T[std::rand() % 10000];
            Node* current = L.head;
            bool direction = std::rand() % 2;
            for(int j = 0; j < 10000; j++) {
                counter++;
                if(number == current->data) {
                    break;
                }
                current = direction ? current->prev : current->next;
            }
            file << counter << std::endl;
            total_comparisons += counter;
            counter = 0;
        }
        double average_cost1 = static_cast<double>(total_comparisons) / 1000.0;
        file << average_cost1 << std::endl;
    } else {
        std::cerr << "Error: File not opened!" << std::endl;
    }

    std::ofstream file2("wyniki4.txt");
    if(file2.is_open()) {
        total_comparisons = 0;
        for(int i = 0; i < 1000; i++) {
            number = std::rand() % 1000001;
            Node* current = L.head;
            bool direction = std::rand() % 2;
            for(int j = 0; j < 10000; j++) {
                counter++;
                if(number == current->data) {
                    break;
                }
                current = direction ? current->prev : current->next;
            }
            total_comparisons += counter;
            file2 << counter << std::endl;
            counter = 0;
        }
        double average_cost2 = static_cast<double>(total_comparisons) / 1000.0;
        file2 << average_cost2 << std::endl;
    } else {
        std::cerr << "Error: File not opened!" << std::endl;
    }

    clear(list1);
    clear(list2);
    clear(L);

    return 0;
}