#include <iostream>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <arpa/inet.h>
#include <vector>
#include "board.h"

using namespace std;
struct Opening {
    int boardState[5][5]; // Stan planszy
    int move;             // Zalecany ruch
};

// Przykładowa biblioteka otwarć
Opening openingLibrary[] = {
    // Center response - take corner
    {{{0,0,0,0,0}, {0,0,0,0,0}, {0,0,1,0,0}, {0,0,0,0,0}, {0,0,0,0,0}}, 11},  // (3,3) → (1,1)

};

bool compareBoards(const int board1[5][5], const int board2[5][5]) {
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            if (board1[i][j] != board2[i][j]) {
                return false;
            }
        }
    }
    return true;
}
int countMoves() {
    int moves = 0;
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            if (board[i][j] != 0) {
                moves++;
            }
        }
    }
    return moves;
}
class GameClient {
private:
    int server_socket;
    struct sockaddr_in server_addr;
    int player;
    string player_name;
    int depth;

public:
    GameClient(const string& ip, int port, int player, const string& name, int depth)
        : player(player), player_name(name), depth(depth) {
        server_socket = socket(AF_INET, SOCK_STREAM, 0);
        if (server_socket < 0) {
            cerr << "Unable to create socket" << endl;
            exit(EXIT_FAILURE);
        }
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(port);
        server_addr.sin_addr.s_addr = inet_addr(ip.c_str());
    }

    void connectToServer() {
        if (connect(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
            cerr << "Unable to connect to server" << endl;
            exit(EXIT_FAILURE);
        }
        cout << "Connected to server successfully" << endl;
    }

    void playGame() {
        char server_message[16], player_message[16];
        bool end_game = false;

        // Initial handshake
        memset(server_message, '\0', sizeof(server_message));
        if (recv(server_socket, server_message, sizeof(server_message), 0) < 0) {
            cerr << "Error while receiving server's message" << endl;
            exit(EXIT_FAILURE);
        }
        cout << "Server message: " << server_message << endl;

        snprintf(player_message, sizeof(player_message), "%d %s", player, player_name.c_str());
        if (send(server_socket, player_message, strlen(player_message), 0) < 0) {
            cerr << "Unable to send message" << endl;
            exit(EXIT_FAILURE);
        }

        // Main game loop
        while (!end_game) {
            memset(server_message, '\0', sizeof(server_message));
            if (recv(server_socket, server_message, sizeof(server_message), 0) < 0) {
                cerr << "Error while receiving server's message" << endl;
                exit(EXIT_FAILURE);
            }
            cout << "Server message: " << server_message << endl;

            int msg, move;
            sscanf(server_message, "%d", &msg);
            move = msg % 100;
            msg = msg / 100;

            if (move != 0) {
                setMove(move, 3 - player); // Ustaw ruch przeciwnika
                printBoard(); // Wyświetl planszę
            }

            if (msg == 0 || msg == 6) {
                move = calculateBestMove(); // Oblicz najlepszy ruch
                if (setMove(move, player)) { // Ustaw swój ruch
                    printBoard(); // Wyświetl planszę
                } else {
                    cout << "Invalid move!" << endl;
                }
                snprintf(player_message, sizeof(player_message), "%d", move);
                if (send(server_socket, player_message, strlen(player_message), 0) < 0) {
                    cerr << "Unable to send message" << endl;
                    exit(EXIT_FAILURE);
                }
                cout << "Your move: " << move << endl;
            } else {
                end_game = true;
                handleEndGame(msg);
            }
        }

        close(server_socket);
    }
    int evaluateBoard(int player) {
        int score = 0;
        // Sprawdź, czy gracz wygrał
        if (winCheck(player)) {
            return 10000; 
        }
        // Sprawdź, czy przeciwnik wygrał
        if (winCheck(3 - player)) {
            return -10000; 
        }
for (int i = 0; i < 48; i++) {
    int playerCount = 0;
    int emptyCount = 0;

    for (int j = 0; j < 3; j++) {
        int x = lose[i][j][0];
        int y = lose[i][j][1];
        if (board[x][y] == player) {
            playerCount++;
        } else if (board[x][y] == 0) {
            emptyCount++;
        }
    }

    // Kara za tworzenie trzech symboli w linii
    if (playerCount == 3 && emptyCount == 0) {
        score -= 500; // Zwiększona kara
    }
}

// Sprawdź linie, w których przeciwnik ma trzy symbole
for (int i = 0; i < 48; i++) {
    int opponentCount = 0;
    int emptyCount = 0;

    for (int j = 0; j < 3; j++) {
        int x = lose[i][j][0];
        int y = lose[i][j][1];
        if (board[x][y] == (3 - player)) {
            opponentCount++;
        } else if (board[x][y] == 0) {
            emptyCount++;
        }
    }

    // Nagroda za groźby przeciwnika
    if (opponentCount == 3 && emptyCount == 0) {
        score += 500; // Zwiększona nagroda
    }
}
    
        // logika do wykrywania groźby wygranej przeciwnika
        for (int i = 0; i < 28; i++) { // Sprawdzanie linii wygrywających
            int opponentCount = 0;
            int emptyCount = 0;
        
            for (int j = 0; j < 4; j++) {
                int x = win[i][j][0];
                int y = win[i][j][1];
                if (board[x][y] == (3 - player)) {
                    opponentCount++;
                } else if (board[x][y] == 0) {
                    emptyCount++;
                }
            }
    
            if (opponentCount == 3 && emptyCount == 1) {
                return -10000; // Natychmiastowa kara za groźbę wygranej przeciwnika
            }
        }
    
        return score;
    }
    int minimax(int depth, bool isMaximizing, int currentPlayer, int alpha, int beta) {
        if (depth == 0 || winCheck(player) || winCheck(3 - player)) {
            return evaluateBoard(player);
        }
    
        if (isMaximizing) {
            int maxEval = -10000;
            for (int i = 1; i <= 5; i++) {
                for (int j = 1; j <= 5; j++) {
                    int move = i * 10 + j;
                    if (setMove(move, currentPlayer)) {
                        int eval = minimax(depth - 1, false, 3 - currentPlayer, alpha, beta);
                        maxEval = max(maxEval, eval);
                        board[i - 1][j - 1] = 0; 
                        alpha = max(alpha, maxEval);
                        if (alpha >= beta) {
                            return maxEval; // Cięcie
                        }
                    }
                }
            }
            return maxEval;
        } else {
            int minEval = 10000;
            for (int i = 1; i <= 5; i++) {
                for (int j = 1; j <= 5; j++) {
                    int move = i * 10 + j;
                    if (setMove(move, currentPlayer)) {
                        int eval = minimax(depth - 1, true, 3 - currentPlayer, alpha, beta);
                        minEval = min(minEval, eval);
                        board[i - 1][j - 1] = 0;
                        beta = min(beta, minEval);
                        if (alpha >= beta) {
                            return minEval; // Cięcie
                        }
                    }
                }
            }
            return minEval;
        }
    }
   int calculateBestMove() {
    int moves = countMoves();
    if (moves < depth) { 
        if (board[2][2] == 0) { 
            return 33; // Ruch do centrum
        }
        for (const auto& opening : openingLibrary) {
            if (compareBoards(board, opening.boardState)) {
                return opening.move; // Zwróć ruch z biblioteki otwarć
            }
        }
    }
        int bestMove = -1;
        int bestValue = -10000;
        int alpha = -10000;
        int beta = 10000;
    
        for (int i = 1; i <= 5; i++) {
            for (int j = 1; j <= 5; j++) {
                int move = i * 10 + j;
                if (setMove(move, player)) {
                    int moveValue = minimax(depth - 1, false, player, alpha, beta);
                    board[i - 1][j - 1] = 0; // Cofnij ruch
    
                    if (moveValue > bestValue) {
                        bestValue = moveValue;
                        bestMove = move;
                    }
                }
            }
        }
    
        return bestMove;
    }
    void handleEndGame(int msg) {
        switch (msg) {
            case 1: cout << "You won." << endl; break;
            case 2: cout << "You lost." << endl; break;
            case 3: cout << "Draw." << endl; break;
            case 4: cout << "You won. Opponent error." << endl; break;
            case 5: cout << "You lost. Your error." << endl; break;
        }
    }
};

int main(int argc, char* argv[]) {
    if (argc != 6) {
        cerr << "Usage: " << argv[0] << " <server_ip> <server_port> <player_number> <player_name> <depth>" << endl;
        return EXIT_FAILURE;
    }

    string ip = argv[1];
    int port = atoi(argv[2]);
    int player = atoi(argv[3]);
    string name = argv[4];
    int depth = atoi(argv[5]);

    GameClient client(ip, port, player, name, depth);
    client.connectToServer();
    client.playGame();

    return 0;
}