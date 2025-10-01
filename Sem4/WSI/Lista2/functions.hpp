#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP

#include <bits/stdc++.h>

extern long long visitedCount;
// Funkcja do wyświetlania planszy
template <size_t N>
void printBoard(const std::array<int, N>& board) {
    int size = static_cast<int>(std::sqrt(N)); 
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            std::cout << board[i * size + j] << " ";
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}

// Funkcja do liczenia inwersji w planszy
template <size_t N>
int countInversions(const std::array<int, N>& tiles) {
    int inversions = 0;
    for (size_t i = 0; i < tiles.size(); ++i) {
        for (size_t j = i + 1; j < tiles.size(); ++j) {
            if (tiles[i] != 0 && tiles[j] != 0 && tiles[i] > tiles[j]) {
                ++inversions;
            }
        }
    }
    return inversions;
}

// Funkcja do znajdowania pozycji pustego miejsca (0)
template <size_t N>
int findEmptyTile(const std::array<int, N>& board) {
    for (size_t i = 0; i < board.size(); ++i) {
        if (board[i] == 0) {
            return static_cast<int>(i);
        }
    }
    return -1;
}
/// Funkcja do przesuwania kafelka w określonym kierunku
template <size_t N>
bool moveTile(std::array<int, N>& board, const std::string& direction) {
    int size = static_cast<int>(std::sqrt(N)); // Rozmiar planszy (np. 4 dla 4x4, 3 dla 3x3)
    int emptyIndex = findEmptyTile(board);
    int emptyRow = emptyIndex / size;
    int emptyCol = emptyIndex % size;

    int newRow = emptyRow, newCol = emptyCol;
    if (direction == "up") {
        newRow = emptyRow - 1;
    } else if (direction == "down") {
        newRow = emptyRow + 1;
    } else if (direction == "left") {
        newCol = emptyCol - 1;
    } else if (direction == "right") {
        newCol = emptyCol + 1;
    } else {
        return false;
    }

    if (newRow >= 0 && newRow < size && newCol >= 0 && newCol < size) {
        int newIndex = newRow * size + newCol;
        std::swap(board[emptyIndex], board[newIndex]);
        return true;
    }
    return false;
}

/// Funkcja do generowania losowej planszy NxN
template <size_t N>
std::array<int, N> generateRandomBoard() {
    std::array<int, N> tiles;
    for (size_t i = 0; i < N - 1; ++i) {
        tiles[i] = static_cast<int>(i + 1);
    }
    tiles[N - 1] = 0; 

    std::random_device rd;
    std::mt19937 g(rd());

    do {
        std::shuffle(tiles.begin(), tiles.end()-1, g);
    } while (countInversions(tiles) % 2 != 0);

    return tiles;
}



/// Funkcja do generowania losowej planszy NxN z ruchami
template <size_t N>
std::array<int, N> generateRandomBoardWithMoves(int k) {
    std::array<int, N> tiles;
    for (size_t i = 0; i < N - 1; ++i) {
        tiles[i] = static_cast<int>(i + 1);
    }
    tiles[N - 1] = 0; 

    std::vector<std::string> directions = {"up", "down", "left", "right"};
    std::unordered_map<std::string, std::string> opposite = {
        {"up", "down"}, {"down", "up"}, {"left", "right"}, {"right", "left"}
    };

    std::random_device rd;
    std::mt19937 g(rd());
    std::uniform_int_distribution<> dist(0, 3);

    std::string lastMove = ""; 

    for (int i = 0; i < k; ++i) {
        std::string direction;
        do {
            direction = directions[dist(g)];
        } while (!lastMove.empty() && direction == opposite[lastMove]); 

        if (moveTile<N>(tiles, direction)) {
            lastMove = direction; 
        }
    }

    return tiles;
}
// Funkcja obliczająca Manhattan Distance
template <size_t N>
int calculateManhattanDistance(const std::array<int, N>& board) {
    int distance = 0;
    int size = static_cast<int>(std::sqrt(N)); // Rozmiar planszy 

    for (size_t i = 0; i < board.size(); ++i) {
        if (board[i] != 0) {
            int targetRow = (board[i] - 1) / size;
            int targetCol = (board[i] - 1) % size;
            int currentRow = i / size;
            int currentCol = i % size;
            distance += std::abs(currentRow - targetRow) + std::abs(currentCol - targetCol);
        }
    }
    return distance;
}
// Funkcja obliczająca liczbę źle umiejscowionych kafelków
template <size_t N>
int calculateMisplacedTiles(const std::array<int, N>& board) {
    int misplaced = 0;
    for (size_t i = 0; i < N - 1; ++i) {
        if (board[i] != static_cast<int>(i + 1)) {
            ++misplaced;
        }
    }
    return misplaced;
}

template <size_t N>
double calculateEuclideanDistance(const std::array<int, N>& board) {
    double totalDistance = 0.0;
    int size = static_cast<int>(std::sqrt(N)); // Rozmiar planszy 

    for (int i = 0; i < N; ++i) {
        if (board[i] != 0) { 
            int targetRow = (board[i] - 1) / size;
            int targetCol = (board[i] - 1) % size;
            int currentRow = i / size;
            int currentCol = i % size;
            // Odległość euklidesowa
            double distance = std::sqrt(
                std::pow(targetRow - currentRow, 2) + std::pow(targetCol - currentCol, 2)
            );
            totalDistance += distance;
        }
    }

    return totalDistance;
}

// Funkcja sprawdzająca, czy plansza jest rozwiązana
template <size_t N>
bool isSolved(const std::array<int, N>& board) {
    for (size_t i = 0; i < N - 1; ++i) {
        if (board[i] != static_cast<int>(i + 1)) {
            return false;
        }
    }
    return board[N - 1] == 0; 
}

// Funkcja do serializacji planszy do stringa
template <size_t N>
std::string serializeBoard(const std::array<int, N>& board) {
    std::string result;
    result.reserve(N); // Rezerwujemy dokładnie N bajtów
    for (int tile : board) {
        result.push_back(static_cast<char>(tile));
    }
    return result;
}

template <size_t N>
struct State {
    std::shared_ptr<std::array<int, N>> board; // Wskaźnik do planszy
    int emptyRow, emptyCol;                   // Pozycja pustego miejsca
    int g;                                    // Koszt dojścia do tego stanu
    int h;                                    // Heurystyka
    std::shared_ptr<State<N>> parent;        // Wskaźnik do rodzica
    int movedTile;                           // Kafelek, który został przesunięty
    // Konstruktor domyślny
    State() : board(std::make_shared<std::array<int, N>>()), g(0), h(0), emptyRow(0), emptyCol(0), parent(nullptr), movedTile(0) {}

    int f() const { // Funkcja kosztu f(n) = g(n) + h(n)
        return g + h;
    }

    bool operator<(const State& other) const {
        return f() > other.f(); // Kolejka priorytetowa w porządku rosnącym
    }
};

// Funkcja generująca sąsiednie stany
template <size_t N>
std::vector<std::shared_ptr<State<N>>> generateNeighbors(const std::shared_ptr<State<N>>& current, const std::string& heuristic) {
    std::vector<std::shared_ptr<State<N>>> neighbors;
    std::vector<std::string> directions = {"down", "right", "left", "up"};
    for (const auto& direction : directions) {
        auto next = std::make_shared<State<N>>();
        next->board = std::make_shared<std::array<int, N>>(*current->board); // Kopiowanie planszy do nowego wskaźnika
        if (moveTile<N>(*next->board, direction)) {
            int emptyIndex = findEmptyTile<N>(*next->board);
            next->emptyRow = emptyIndex / static_cast<int>(std::sqrt(N));
            next->emptyCol = emptyIndex % static_cast<int>(std::sqrt(N));
            next->g = current->g + 1;
            next->parent = current; // Ustawienie rodzica
            next->movedTile = (*current->board)[emptyIndex];

            if (heuristic == "manhattan") {
                next->h = calculateManhattanDistance<N>(*next->board);
            } else if (heuristic == "misplaced") {
                next->h = calculateMisplacedTiles<N>(*next->board);
            }
            else if (heuristic == "euclidean") {
                next->h = calculateEuclideanDistance<N>(*next->board);
            }

            neighbors.push_back(next);
        }
    }
    return neighbors;
}
template <size_t N>
struct CompareStates {
    bool operator()(const std::shared_ptr<State<N>>& a, const std::shared_ptr<State<N>>& b) const {
        return a->f() > b->f(); // Kolejka priorytetowa w porządku rosnącym
    }
};
// Algorytm A*
template <size_t N>
std::string solvePuzzle(const std::array<int, N>& startBoard, const std::string& heuristic) {
    std::priority_queue<std::shared_ptr<State<N>>, std::vector<std::shared_ptr<State<N>>>, CompareStates<N>> openSet;
    std::unordered_map<std::string, int> visited;
    visitedCount = 0;

    auto start = std::make_shared<State<N>>();
    *start->board = startBoard; // Kopiowanie planszy do wskaźnika
    int emptyIndex = findEmptyTile<N>(*start->board);
    start->emptyRow = emptyIndex / static_cast<int>(std::sqrt(N));
    start->emptyCol = emptyIndex % static_cast<int>(std::sqrt(N));
    start->g = 0;

    if (heuristic == "manhattan") {
        start->h = calculateManhattanDistance<N>(*start->board);
    } else if (heuristic == "misplaced") {
        start->h = calculateMisplacedTiles<N>(*start->board);
    } else if (heuristic == "euclidean") {
        start->h = calculateEuclideanDistance<N>(*start->board);
    } 

    // std::cout << "Początkowa Manhattan Distance: " << start->h << "\n";
    openSet.push(start);

    while (!openSet.empty()) {
        auto current = openSet.top();
        openSet.pop();

        if (isSolved<N>(*current->board)) {
            std::string moves;
            for (auto p = current; p->parent != nullptr; p = p->parent) {
                moves = std::to_string(p->movedTile) + " " + moves;
            }
            return moves;
        }

        std::string serializedBoard = serializeBoard<N>(*current->board);
        if (visited.count(serializedBoard) && visited[serializedBoard] <= current->g) {
            continue; // Pomijamy stany, które zostały odwiedzone z mniejszym kosztem
        }
        visited[serializedBoard] = current->g; // Aktualizujemy koszt dojścia
        ++visitedCount;

        for (const auto& neighbor : generateNeighbors<N>(current, heuristic)) {
            openSet.push(neighbor);
        }
    }

    return "Brak rozwiązania";
}


template <size_t N>
std::string reconstructPath(const std::shared_ptr<State<N>>& startNode, const std::shared_ptr<State<N>>& goalNode) {
    std::string forwardMoves, backwardMoves;

    // Rekonstrukcja ścieżki od początku
    for (auto p = startNode; p != nullptr && p->parent != nullptr; p = p->parent) {
        forwardMoves = std::to_string(p->movedTile) + " " + forwardMoves;
    }
    
    for (auto p = goalNode; p != nullptr && p->parent != nullptr; p = p->parent) {
        backwardMoves += std::to_string(p->movedTile) + " ";
    }

    return forwardMoves + backwardMoves;
}

template <size_t N>
void search(
    bool forward,
    std::priority_queue<std::shared_ptr<State<N>>, std::vector<std::shared_ptr<State<N>>>, CompareStates<N>>& openSet,
    std::unordered_map<std::string, std::shared_ptr<State<N>>>& seen,
    std::unordered_map<std::string, std::shared_ptr<State<N>>>& otherSeen,
    std::mutex& mtx,
    std::condition_variable& cv,
    bool& solutionFound,
    std::string& solution,
    const std::string& heuristic
) {
    while (!openSet.empty() && !solutionFound) {
        auto current = openSet.top();
        openSet.pop();
        if (visitedCount > 30000000) {
            std::cout << "Liczba odwiedzonych stanów: " << visitedCount << ", Rozmiar openSet: " << openSet.size() << "\n";
            std::cout << "g: " << current->g << ", h: " << current->h << ", f: " << current->f() << "\n";
        }
        if (!current) {
            continue; // Pomijamy nieprawidłowy wskaźnik
        }

        std::string key = serializeBoard<N>(*current->board);

        {
            std::lock_guard<std::mutex> lock(mtx);
            if (otherSeen.count(key)) {
                solutionFound = true;
                solution = forward
                    ? reconstructPath(current, otherSeen[key])
                    : reconstructPath(otherSeen[key], current);
                cv.notify_all();
                return;
            }
        }

        for (const auto& neighbor : generateNeighbors<N>(current, heuristic)) {
            if (!neighbor) {
                continue; // Pomijamy nieprawidłowy wskaźnik
            }

            std::string neighborKey = serializeBoard<N>(*neighbor->board);
            {
                std::lock_guard<std::mutex> lock(mtx);
                if (!seen.count(neighborKey) || seen[neighborKey]->g > neighbor->g) {
                    openSet.push(neighbor);
                    seen[neighborKey] = neighbor;
                    ++visitedCount;
                }
            }
        }
    }
}

template <size_t N>
std::string solveBidirectional(const std::array<int, N>& startBoard, const std::string& heuristic) {
    std::priority_queue<std::shared_ptr<State<N>>, std::vector<std::shared_ptr<State<N>>>, CompareStates<N>> openStart, openGoal;
    std::unordered_map<std::string, std::shared_ptr<State<N>>> seenStart, seenGoal;
    std::mutex mtx;
    std::condition_variable cv;
    bool solutionFound = false;
    std::string solution;
    visitedCount = 0;

    // Stan początkowy
    auto start = std::make_shared<State<N>>();
    *start->board = startBoard;
    start->g = 0;
    if (heuristic == "manhattan") {
        start->h = calculateManhattanDistance<N>(*start->board);
    } else if (heuristic == "misplaced") {
        start->h = calculateMisplacedTiles<N>(*start->board);
    } else if (heuristic == "euclidean") {
        start->h = calculateEuclideanDistance<N>(*start->board);
    }
    // std::cout << "Początkowa Manhattan Distance: " << start->h << "\n";
    start->emptyRow = findEmptyTile<N>(*start->board) / static_cast<int>(std::sqrt(N));
    start->emptyCol = findEmptyTile<N>(*start->board) % static_cast<int>(std::sqrt(N));
    openStart.push(start);
    seenStart[serializeBoard<N>(*start->board)] = start;

    // Stan końcowy
    auto goal = std::make_shared<State<N>>();
    for (size_t i = 0; i < N - 1; ++i) (*goal->board)[i] = static_cast<int>(i + 1);
    (*goal->board)[N - 1] = 0;
    goal->g = 0;
    goal->h = 0;
    goal->emptyRow = static_cast<int>(std::sqrt(N)) - 1;
    goal->emptyCol = static_cast<int>(std::sqrt(N)) - 1;
    openGoal.push(goal);
    seenGoal[serializeBoard<N>(*goal->board)] = goal;

    // Uruchomienie wątków
    std::thread forwardThread(
        search<N>,
        true, // forward
        std::ref(openStart),
        std::ref(seenStart),
        std::ref(seenGoal),
        std::ref(mtx),
        std::ref(cv),
        std::ref(solutionFound),
        std::ref(solution),
        heuristic
    );

    std::thread backwardThread(
        search<N>,
        false, // backward
        std::ref(openGoal),
        std::ref(seenGoal),
        std::ref(seenStart),
        std::ref(mtx),
        std::ref(cv),
        std::ref(solutionFound),
        std::ref(solution),
        heuristic
    );

    // Czekanie na zakończenie
    {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [&]() { return solutionFound; });
    }

    forwardThread.join();
    backwardThread.join();

    return solutionFound ? solution : "Brak rozwiązania";
}

template <size_t N>
void displayFinalState(const std::array<int, N>& startBoard, const std::string& solution) {
    std::array<int, N> board = startBoard;
    std::istringstream movesStream(solution);
    int movedTile;

    while (movesStream >> movedTile) {
        int emptyIndex = findEmptyTile<N>(board);
        int size = static_cast<int>(std::sqrt(N));
        int emptyRow = emptyIndex / size;
        int emptyCol = emptyIndex % size;

        // Znajdź kafelek, który został przesunięty
        int tileIndex = -1;
        for (size_t i = 0; i < N; ++i) {
            if (board[i] == movedTile) {
                tileIndex = static_cast<int>(i);
                break;
            }
        }

        int tileRow = tileIndex / size;
        int tileCol = tileIndex % size;

        // Przesuń kafelek
        if (std::abs(tileRow - emptyRow) + std::abs(tileCol - emptyCol) == 1) {
            std::swap(board[emptyIndex], board[tileIndex]);
        }
    }

    std::cout << "Końcowy stan planszy:\n";
    printBoard<N>(board);
}
#endif // FUNCTIONS_HPP