%{
#include <cstdio>
#include <cstring>
#include <string>
#include <stack>
#include <cmath>
#include <cstdlib>
#include <climits>

using std::string;
using std::stack;

string error_msg;
stack<int> numbers;
bool bad = false;
string current_line;

void clear_stack() {
    while (!numbers.empty()) numbers.pop();
    bad = false;
    error_msg.clear();
    current_line.clear();
}

void finish_line() {
    // Usuń końcowe białe znaki z aktualnej linii
    while (!current_line.empty() && (current_line.back() == ' ' || current_line.back() == '\t' || current_line.back() == '\r')) {
        current_line.pop_back();
    }
    
    printf("%s\n", current_line.c_str());
    
    if (bad) {
        printf("Błąd: %s\n", error_msg.c_str());
    } else if (numbers.empty()) {
        printf("Błąd: puste wyrażenie\n");
    } else if (numbers.size() > 1) {
        printf("Błąd: za mała liczba operatorów\n");
    } else {
        printf("= %d\n", numbers.top());
    }
    clear_stack();
}

bool do_op(char op) {
    if (bad) return false;
    if (numbers.size() < 2) { error_msg = "za mała liczba argumentów"; bad = true; return false; }
    int b = numbers.top(); numbers.pop();
    int a = numbers.top(); numbers.pop();
    long long r = 0;
    switch (op) {
        case '+': r = (long long)a + b; break;
        case '-': r = (long long)a - b; break;
        case '*': r = (long long)a * b; break;
        case '/':
            if (b == 0) { error_msg = "dzielenie przez zero"; bad = true; return false; }
            r = a / b; break;
        case '%':
            if (b == 0) { error_msg = "modulo przez zero"; bad = true; return false; }
            r = a % b; break;
        case '^':
            if (b < 0) { error_msg = "ujemny wykładnik"; bad = true; return false; }
            r = (long long)pow(a, b);
            break;
        default:
            error_msg = "nieznany operator";
            bad = true;
            return false;
    }
    
    if (r < INT_MIN || r > INT_MAX) {
        error_msg = "przekroczenie zakresu";
        bad = true;
        return false;
    }
    
    numbers.push((int)r);
    return true;
}
%}

WHITESPACE     [ \t]+
DIGIT         -?[0-9]+
OPERATOR     [+\-*/%^]
INVALID        ([0-9]*\.[0-9]+)|[^0-9+\-*/%^ \t\r\n]+

%x ERRORS

%%

{DIGIT}        { 
    if (!bad) numbers.push(atoi(yytext)); 
    current_line += yytext;
}

{OPERATOR}     { 
    if (!bad) do_op(yytext[0]); 
    current_line += yytext;
}

{WHITESPACE}   { 
    current_line += yytext; 
}

\n             { 
    finish_line(); 
}

{INVALID}      { 
    error_msg = std::string("zły symbol '") + yytext + "'"; 
    bad = true; 
    current_line += yytext;
    BEGIN(ERRORS); 
}

<ERRORS>[^\n]*\n  { 
    current_line += yytext;
    finish_line(); 
    BEGIN(INITIAL); 
}

<ERRORS>.      { 
    current_line += yytext;
}

%%

int yywrap() { 
    if (!current_line.empty()) {
        finish_line();
    }
    return 1; 
}

int main() {
    yylex();
    return 0;
}