%{
#include <string>
#include <stack>
#include <cmath>
#include <iostream>
#include <numeric>

#define P 1234577
void yyerror(const char *s);
int yylex();
long long mod_inverse(long long a, long long p);
int toGF_exp(int a);
std::string error_msg;

std::string ONP; 
%}

%token NUMBER PLUS MINUS MULT DIV POW LPAREN RPAREN

%left PLUS MINUS
%left MULT DIV
%right POW
%right UMINUS

%%

input:
    %empty
    | input line
;

line:
    expr '\n' {
        printf("ONP: %s\n", ONP.c_str()); 
        printf("Wynik: %d\n", $1);
        ONP.clear();
    }
    | error '\n' { 
        if (error_msg != "") {
            printf("Błąd: %s\n", error_msg.c_str());
            error_msg = "";
        } else {
            printf("Błąd: Zła składnia\n");
        }
        ONP.clear();
    }
;

expr:
    simple_expr
    | expr PLUS expr {
        $$ = ($1 + $3) % P;
        ONP += "+ ";
    }
    | expr MINUS expr {
        $$ = ($1 - $3 + P) % P;
        ONP += "- ";
    }
    | expr MULT expr {
        $$ = ($1 * $3) % P;
        ONP += "* ";
    }
    | expr DIV expr {
        if ($3 == 0) {
            error_msg = "Dzielenie przez 0";
            YYERROR;
        }
        long long inv = mod_inverse($3, P);
        if (inv == -1) {
            error_msg = "Dzielenie przez liczbę nieodwracalną w GF(P)";
            YYERROR;
        }
        $$ = ($1 * inv) % P;
        ONP += "/ ";
    }
;

simple_expr:
    power_expr
;

power_expr:
    atom
    | atom POW exponent {
        long long result = 1;
        long long base_val = $1 % P;
        long long exp_val = $3;

        while (exp_val > 0) {
            if (exp_val % 2 == 1) {
                result = (result * base_val) % P;
            }
            base_val = (base_val * base_val) % P;
            exp_val /= 2;
        }
        $$ = result;
        ONP += "^ ";
    }
;

atom:
    NUMBER_CHANGED {
        $$ = $1;
        ONP += std::to_string($$) + " ";
    }
    | LPAREN expr RPAREN {
        $$ = $2;
    }
;

exponent:
    EXP_SIMPLE
    | LPAREN EXP_COMPLEX RPAREN {
        $$ = $2;
    }
;

EXP_SIMPLE:
    NUMBER {
        $$ = toGF_exp($1);
        ONP += std::to_string($1) + " "; 
    }
    | MINUS NUMBER %prec UMINUS {
        $$ = toGF_exp(-$2);
        ONP += std::to_string($$) + " ";
    }
;

EXP_COMPLEX:
    EXP_SIMPLE
    | EXP_COMPLEX PLUS EXP_SIMPLE {
        $$ = toGF_exp($1 + $3);
        ONP += "+ ";
    }
    | EXP_COMPLEX MINUS EXP_SIMPLE {
        $$ = toGF_exp($1 - $3);
        ONP += "- ";
    }
    | EXP_COMPLEX MULT EXP_SIMPLE {
        $$ = toGF_exp($1 * $3);
        ONP += "* ";
    }
    | EXP_COMPLEX DIV EXP_SIMPLE {
        if ($3 == 0) {
            error_msg = "Dzielenie przez 0 w wykładniku";
            YYERROR;
        }
        if ($1 % $3 == 0) {
            $$ = $1 / $3;
            ONP += "/ "; 
        } else {
            long long inv = mod_inverse($3, P - 1);
            if (inv == -1) {
                error_msg = "Dzielenie przez liczbę nieodwracalną w GF(P-1)";
                YYERROR;
            }
            $$ = toGF_exp($1 * inv);
            ONP += "/ "; 
        }
    }
;

NUMBER_CHANGED:
    NUMBER {
        $$ = ($1 % P + P) % P; 
    }
    | MINUS NUMBER %prec UMINUS {
        $$ = (-$2 % P + P) % P;
    }
;

%%

int toGF_exp(int a) {
    return ((a % (P - 1)) + (P - 1)) % (P - 1);
}

long long mod_inverse(long long a, long long p) {
    long long m0 = p, t, q;
    long long x0 = 0, x1 = 1;

    if (std::gcd(a, p) != 1) {
        return -1;
    }

    if (p == 1) return 0;

    while (a > 1) {
        q = a / p;
        t = p;
        p = a % p, a = t;
        t = x0;
        x0 = x1 - q * x0;
        x1 = t;
    }

    if (x1 < 0) x1 += m0;

    return x1;
}

void yyerror(const char *s) {
    error_msg = "Zła składnia";
}

int main() {
    printf("Podaj wyrażenie:\n");
    return yyparse();
}