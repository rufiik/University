#include <bits/stdc++.h>
using namespace std;

set<uint32_t> buildAlphabet(const string &pattern, const string &text) {
    set<uint32_t> alphabet;
    for (char c : pattern) alphabet.insert((unsigned char)c);
    for (char c : text) alphabet.insert((unsigned char)c);
    return alphabet;
}

vector<unordered_map<uint32_t, int>> buildTransition(const string &p, const set<uint32_t> &alphabet) {
    int m = (int)p.size();
    vector<unordered_map<uint32_t, int>> TF(m + 1);

    for (int q = 0; q <= m; ++q) {
        for (uint32_t c : alphabet) { 
            string s = p.substr(0, q) + char(c);
            int k = min(m, q + 1);
            while (k > 0) {
                if (p.substr(0, k) == s.substr((int)s.size() - k)) break;
                --k;
            }
            TF[q][c] = k;
        }
    }
    return TF;
}

int main(int argc, char **argv) {
    if (argc != 3) {
        cerr << "Uzycie: FA <wzorzec> <nazwa pliku>\n";
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
    auto alphabet = buildAlphabet(pattern, filename);
    auto TF = buildTransition(pattern, alphabet);
    int m = (int)pattern.size();
    int state = 0;
    long long pos = 0;
    int ch;
    while ((ch = ifs.get()) != EOF) {
        unsigned char uc = static_cast<unsigned char>(ch);
        state = TF[state][uc];
        if (state == m) {
            cout << (pos - m + 1) << "\n";
        }
        ++pos;
    }
    return 0;
}