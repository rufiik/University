Biblioteki użyte w programie:
- Biblioteki standardowe języka C++ oraz systemowe (unistd.h, arpa/inet.h)
- Brak zewnętrznych bibliotek (program nie wymaga dodatkowych bibliotek poza standardowymi)
- board.h wykonane przez Macieja Gębale
Aby skompilować program należy użyć polecenia:
g++ my_game_client.cpp -std=c++20 -W -pedantic -O3 -o my_game_client

# lub jeśli używany jest starszy kompilator:
# g++ my_game_client.cpp -std=c++17 -W -pedantic -O3 -o my_game_client

Wymagania:
- Kompilator g++ w wersji co najmniej 9 (zalecane wsparcie dla C++17 lub C++20)