// 
///123
/**
    * @file test.cpp
 */
 /*!
 
 */

 //! @mainpage Dokumentacja projektu

#include <iostream>
#include <string>

using namespace std;

int main() {
    // Zwykły komentarz
    string message1 = "Hello // world /* komentarz */ w stringu";
    string message2 = "To nie jest /* komentarz */ tylko string";
    
    /*
     * Komentarz wielolinijkowy
     * z różnymi treściami
     */
    string message3 = "Koniec /* komentarza */ w stringu";
    
    cout << message1 << endl;  // Komentarz po kodzie
    cout << "Tekst z // i /* */ w środku" << endl;
    
    /*
    string fake_code = "To wygląda jak kod ale jest w komentarzu";
    cout << fake_code << endl;
    */
    
    string path = "C:\\files\\project//src";  // Ścieżka z podwójnym ukośnikiem
    string regex = "a//b/*c*/d";  // Wyrażenie regularne
        cout << "Pulapka \" \\
        // a \
        /* a */ \
        " << endl;
    return 0;  // Koniec programu


}