#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <string>

void generateRandom(int n) {
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<int> dist(0, 2 * n - 1);

    for (int i = 0; i < n; ++i) {
        std::cout << dist(mt) << " ";
    }
    std::cout << std::endl;
}

void generateAscending(int n) {
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<int> dist(0, 2 * n - 1);
    std::vector<int> numbers(n);

    for(int i=0; i < n; ++i) {
        numbers[i] = dist(mt);
    }
    std::sort(numbers.begin(), numbers.end());
    for(int i=0; i < n; ++i) {
        std::cout << numbers[i] << " ";
    }
    std::cout << std::endl;
}

void generateDescending(int n) {
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<int> dist(0, 2 * n - 1);
    std::vector<int> numbers(n);

    for(int i=0; i < n; ++i) {
        numbers[i] = dist(mt);
    }
    std::sort(numbers.begin(), numbers.end(), std::greater<int>());
    for(int i=0; i < n; ++i) {
        std::cout << numbers[i] << " ";
    }
    std::cout << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Uzycie: " << argv[0] << " <n> <typ>" << std::endl;
        std::cerr << "typ: rand | asc | desc" << std::endl;
        return 1;
    }

    int n = std::stoi(argv[1]);
    std::string type = argv[2];
    std::cout<<n<<" ";
    if (type == "rand") {
        generateRandom(n);
    } else if (type == "asc") {
        generateAscending(n);
    } else if (type == "desc") {
        generateDescending(n);
    } else {
        std::cerr << "Nieznany typ: " << type << std::endl;
        return 1;
    }

    return 0;
}