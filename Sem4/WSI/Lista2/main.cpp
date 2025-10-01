#include "functions.hpp"
long long visitedCount = 0;
int main() {
    //4x4 plansza
    // std::array<int,16> board = generateRandomBoardWithMoves<16>(30);
    std::array<int, 16> board = {0,12,9,13,15,11,10,14,3,7,2,5,4,8,6,1};
    std::cout << "Początkowa plansza:\n";
    printBoard<16>(board);
    // std::cout << "Liczba inwersji: " << countInversions<16>(board) << "\n";



    std::string solution = solveBidirectional<16>(board, "manhattan");
    int manhattanSteps = std::count(solution.begin(), solution.end(), ' '); 
    std::cout << "Liczba odwiedzonych stanów: " << visitedCount << "\n";
    // std::cout << "Manhattan: " << solution << "\n";
    std::cout << "Liczba kroków (Manhattan): " << manhattanSteps << "\n";



    // std::string solution2 = solveBidirectional<16>(board, "euclidean");
    // int euclideanSteps = std::count(solution2.begin(), solution2.end(), ' ');
    // std::cout << "Euclidean: " << solution2 << "\n";
    // std::cout << "Liczba odwiedzonych stanów: " << visitedCount << "\n";
    // std::cout << "Liczba kroków (Euclidean): " << euclideanSteps << "\n";

    // std::string solution3 = solveBidirectional<16>(board, "misplaced");
    // int misplacedSteps = std::count(solution3.begin(), solution3.end(), ' ');
    // std::cout << "Misplaced: " << solution3 << "\n";
        // std::cout << "Liczba odwiedzonych stanów: " << visitedCount << "\n";
    // std::cout << "Liczba kroków (Euclidean): " << euclideanSteps << "\n";



    // displayFinalState<16>(board, solution2);

    //3x3 plansza

    // std::array<int,9> board = generateRandomBoard<9>();
    // std::cout << "Początkowa plansza:\n";
    // printBoard<9>(board);
    // std::string solution = solvePuzzle<9>(board, "manhattan");
    // int manhattanSteps = std::count(solution.begin(), solution.end(), ' '); 
    // std::cout << "Manhattan: " << solution << "\n";
    // std::cout << "Liczba kroków (Manhattan): " << manhattanSteps << "\n";
    // std::cout << "Liczba odwiedzonych stanów: " << visitedCount << "\n";


    // std::string solution2 = solvePuzzle<9>(board, "misplaced");
    // int misplacedSteps = std::count(solution2.begin(), solution2.end(), ' ');
    // std::cout << "Misplaced: " << solution2 << "\n";
    // std::cout << "Liczba kroków (Misplaced): " << misplacedSteps << "\n";
    // std::cout << "Liczba odwiedzonych stanów: " << visitedCount << "\n";

    // std::string solution3 = solvePuzzle<9>(board, "euclidean");
    // int euclideanSteps = std::count(solution3.begin(), solution3.end(), ' ');
    // std::cout << "Euclidean: " << solution3 << "\n";
    // std::cout << "Liczba kroków (Euclidean): " << euclideanSteps << "\n";
    // std::cout << "Liczba odwiedzonych stanów: " << visitedCount << "\n";



//pliki
// std::ofstream outFile("output6.txt", std::ios::app);
//     if (!outFile) {
//         std::cerr << "Nie można otworzyć pliku do zapisu!\n";
//         return 1;
//     }


//     outFile <<"Manhattan " << "\n";
//     outFile <<"Liczba stanów, Liczba kroków, k\n";
//     for(int j=0;j<100;j++){
//         std::array<int, 16> board = generateRandomBoard<16>();
//         std::string solution = solveBidirectional<16>(board, "manhattan");
//         int manhattanSteps = std::count(solution.begin(), solution.end(), ' ');
//         outFile << visitedCount;
//         outFile << " " << manhattanSteps << "\n";

//     }
// outFile <<"Misplaced " << "\n";
// outFile <<"Liczba stanów, Liczba kroków, k\n";

//     for(int k=20; k<100;k++ ){
//         for(int j=0;j<5000;j++){
//         std::array<int,16> board = generateRandomBoardWithMoves<16>(k);
//         std::string solution2 = solvePuzzle<16>(board, "misplaced");
//         int misplacedSteps = std::count(solution2.begin(), solution2.end(), ' ');
//         outFile << visitedCount;
//         outFile << " " << misplacedSteps << " ";
//         outFile << k << "\n";
//     }
// }

// outFile <<"Euclidean " << "\n";
// outFile <<"Liczba stanów, Liczba kroków, k\n";

//     for(int k=20; k<100;k++ ){
//         for(int j=0;j<5000;j++){
//         std::array<int,16> board = generateRandomBoardWithMoves<16>(k);
//         std::string solution2 = solveBidirectional<16>(board, "euclidean");
//         int euclideanSteps = std::count(solution2.begin(), solution2.end(), ' ');
//         outFile << visitedCount;
//         outFile << " " << euclideanSteps << " ";
//         outFile << k << "\n";
//     }
// }


//         for(int j=0;j<5000;j++){
//         std::array<int,16> board = generateRandomBoard<9>();
//         std::string solution2 = solveBidirectional<9>(board, "euclidean");
//         int euclideanSteps = std::count(solution2.begin(), solution2.end(), ' ');
//         outFile << visitedCount;
//         outFile << " " << euclideanSteps << " ";
//         outFile << k << "\n";
//     }
    // outFile.close();

    return 0;
}