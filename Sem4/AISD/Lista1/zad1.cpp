#include <iostream>

class Queue {
private:
    struct Node {
        int data;
        Node* next;
        Node(int value) : data(value), next(nullptr) {}
    };
    Node* front;
    Node* back;

public:
    Queue() : front(nullptr), back(nullptr) {}

    ~Queue() {
        while (front != nullptr) {
            Node* temp = front;
            front = front->next;
            delete temp;
        }
    }

    void enqueue(int value) {
        Node* newNode = new Node(value);
        if (back != nullptr) {
            back->next = newNode;
        }
        back = newNode;
        if (front == nullptr) {
            front = back;
        }
        std::cout << "Enqueued: " << value << std::endl;
    }

    void dequeue() {
        if (front != nullptr) {
            Node* temp = front;
            front = front->next;
            if (front == nullptr) {
                back = nullptr;
            }
            std::cout << "Dequeued: " << temp->data << std::endl;
            delete temp;
        } else {
            std::cerr << "Error: Attempt to dequeue from an empty queue!" << std::endl;
        }
    }

    bool is_empty() const {
        return front == nullptr;
    }
};

class Stack {
private:
    int* data;
    int capacity;
    int top;

    void resize(int new_capacity) {
        int* new_data = new int[new_capacity];
        for (int i = 0; i < top; ++i) {
            new_data[i] = data[i];
        }
        delete[] data;
        data = new_data;
        capacity = new_capacity;
    }

public:
    Stack() : data(new int[1]), capacity(1), top(0) {}

    ~Stack() {
        delete[] data;
    }

    void push(int value) {
        if (top == capacity) {
            resize(2 * capacity);
        }
        data[top++] = value;
        std::cout << "Pushed: " << value << std::endl;
    }

    void pop() {
        if (top > 0) {
            std::cout << "Popped: " << data[--top] << std::endl;
        } else {
            std::cerr << "Error: Attempt to pop from an empty stack!" << std::endl;
        }
    }

    bool is_empty() const {
        return top == 0;
    }
};

int main() {
    Queue queue;
    Stack stack;

    // Dodawanie elementow
    std::cout << "\nDodawanie elementow do kolejki i stosu:\n";
    for (int i = 1; i <= 50; ++i) {
        queue.enqueue(i);
        stack.push(i);
    }

    // Usuwanie elementow
    std::cout << "\nUsuwanie elementow z kolejki:\n";
    while (!queue.is_empty()) {
        queue.dequeue();
    }

    std::cout << "\nUsuwanie elementow ze stosu:\n";
    while (!stack.is_empty()) {
        stack.pop();
    }

    // Proba usuniecia z pustych struktur
    std::cout << "\nTest usuniecia elementu z pustej kolejki:\n";
    queue.dequeue();

    std::cout << "\nTest usuniecia elementu z pustego stosu:\n";
    stack.pop();

    return 0;
}