#include <cstdlib>
#include <iostream>
#include <ctime>
#include <fstream>
struct Node {
    int data;
    Node* next;
    Node(int value) : data(value), next(nullptr) {}
};

struct OneWayList {
    Node* head;
    int size;

    OneWayList() : head(nullptr), size(0) {}
};

void insert(OneWayList& list, int value) {
    Node* newNode = new Node(value);
    if (list.head == nullptr) {
        list.head = newNode;
        newNode->next = newNode;
    } else {
        Node* temp = list.head;
        while (temp->next != list.head) {
            temp = temp->next;
        }
        temp->next = newNode;
        newNode->next = list.head;
    }
    list.size++;
}


void merge(OneWayList& l1, OneWayList& l2) {
    if (l1.head == nullptr) {
        l1.head = l2.head;
        l1.size = l2.size;
        return;
    }
    if (l2.head == nullptr) {
        return;
    }
    Node* temp1 = l1.head;
    while (temp1->next != l1.head) {
        temp1 = temp1->next;
    }
    Node* temp2 = l2.head;
    while (temp2->next != l2.head) {
        temp2 = temp2->next;
    }
    temp1->next = l2.head;
    temp2->next = l1.head;

    l1.size += l2.size;
}

void print(const OneWayList& list) {
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

void clear(OneWayList& list){
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
    OneWayList list1;
    OneWayList list2;
    for(int i=20;i<30;i++){
        insert(list2,i);
    }
    for(int i=30;i<40;i++){
        insert(list1,i);
    }
    //merge i print po mergu
    merge(list1,list2);
    print(list1);
    print(list2);
    int T[10000];
    OneWayList L;
    for(int i=0;i<10000;i++){
        T[i]=std::rand()%100001;
    }

    for(int i=0;i<10000;i++){
        insert(L,T[i]);
    }

    int number=0;
    int counter=0;
    int total_comparisons = 0;
    std::ofstream file("wyniki.txt");
    if(file.is_open()){
    for(int i=0;i<1000;i++){
        number=T[std::rand()%10000];
        for(int j=0;j<10000;j++){
            counter++;
            if(number==L.head->data){
                break;
            }
            L.head=L.head->next;
        }
    file<<counter<<std::endl;
    total_comparisons += counter;
    counter=0;
    }
    double average_cost1 = (total_comparisons) / 1000.0;
    file<< average_cost1 << std::endl;
    }
    else{
        std::cerr<<"Error: File not opened!"<<std::endl;
    } 
    std::ofstream file2("wyniki2.txt");
    if(file2.is_open()){
        total_comparisons=0;
        for(int i=0;i<1000;i++){
            number=std::rand()%1000001;
            for(int j=0;j<10000;j++){
                counter++;
                if(number==L.head->data){
                    break;
                }
                L.head=L.head->next;
            }
            total_comparisons += counter;
            file2<<counter<<std::endl;
            counter=0;
        }
    double average_cost2 = (total_comparisons) / 1000.0;
    file2 << average_cost2 << std::endl;
    }
    else{
        std::cerr<<"Error: File not opened!"<<std::endl;
    }
    clear(list1);
    clear(list2);
    clear(L);

    return 0;
    }