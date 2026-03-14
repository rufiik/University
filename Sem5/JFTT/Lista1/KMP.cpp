#include <bits/stdc++.h>
using namespace std;

vector<int> computePrefixFunction(const string &pattern) {
    int m = pattern.size();
    vector<int> pi(m, 0); 
    int k = 0; 

    for (int q = 1; q < m; ++q) {
        while (k > 0 && pattern[k] != pattern[q]) {
            k = pi[k - 1]; 
        }
        if (pattern[k] == pattern[q]) {
            ++k; 
        }
        pi[q] = k; 
    }
    return pi;
}

vector<int> KMP(const string &pattern, const string &text) {
    vector<int> pi = computePrefixFunction(pattern); 
    vector<int> occurrences; 
    int m = pattern.size();
    int n = text.size();
    int q = 0; 

    for (int i = 0; i < n; ++i) {
        while (q > 0 && pattern[q] != text[i]) {
            q = pi[q - 1]; 
        }
        if (pattern[q] == text[i]) {
            ++q; 
        }
        if (q == m) {
            occurrences.push_back(i - m + 1);
            q = pi[q - 1];
        }
    }
    return occurrences;
}

int main(int argc, char **argv) {
    if (argc != 3) {
        cerr << "Uzycie: KMP <wzorzec> <nazwa pliku>\n";
        return 1;
    }
    string pattern = argv[1];
    if (pattern.empty()) {
        cerr << "Wzorzec nie moze byc pusty\n";
        return 1;
    }
    const char *filename = argv[2];
    ifstream ifs(filename, ios::binary);
    if (!ifs) {
        cerr << "Nie mozna otworzyc pliku: " << filename << "\n";
        return 1;
    }
    string text((istreambuf_iterator<char>(ifs)), istreambuf_iterator<char>());
    vector<int> occurrences = KMP(pattern, text);

    for (int pos : occurrences) {
        cout << pos << "\n";
    }
    return 0;
}