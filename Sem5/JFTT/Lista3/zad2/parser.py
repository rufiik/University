import ply.lex as lex
import ply.yacc as yacc
import math
from math import gcd

P = 1234577
def mod_inverse(a, p):
    if gcd(a, p) != 1:
        return -1
    if p == 1:
        return 0
        
    m0, x0, x1 = p, 0, 1
    a_orig = a
    
    while a > 1:
        q = a // p
        t = p
        p = a % p
        a = t
        t = x0
        x0 = x1 - q * x0
        x1 = t
        
    if x1 < 0:
        x1 += m0
        
    return x1

def toGF_exp(a):
    return ((a % (P - 1)) + (P - 1)) % (P - 1)

class Parser:
    tokens = (
        'NUMBER', 'PLUS', 'MINUS', 'MULT', 'DIV', 'POW', 
        'LPAREN', 'RPAREN', 'NEWLINE'
    )
    precedence = (
            ('left', 'PLUS', 'MINUS'),
            ('left', 'MULT', 'DIV'),
            ('right', 'POW')
        )
    t_PLUS = r'\+'
    t_MINUS = r'-'
    t_MULT = r'\*'
    t_DIV = r'/'
    t_POW = r'\^'
    t_LPAREN = r'\('
    t_RPAREN = r'\)'

    def t_NUMBER(self, t):
        r'\d+'
        t.value = int(t.value)
        return t

    def t_NEWLINE(self, t):
        r'\n'
        t.lexer.lineno += 1
        return t

    t_ignore = ' \t'
    t_ignore_COMMENT = r'\#.*'

    def t_error(self, t):
        print(f"Nieznany znak: '{t.value[0]}'")
        t.lexer.skip(1)

    def __init__(self):
        self.ONP = ""
        self.error_msg = ""
        self.lexer = lex.lex(module=self)  
        self.parser = yacc.yacc(module=self)
        
    def parse(self, input_string):
        self.ONP = ""
        self.error_msg = ""
        return self.parser.parse(input_string, lexer=self.lexer)
    
    # --- GRAMATYKA ---
    
    def p_input(self, p):
        '''input : line
                 | input line'''
        pass
    
    def p_line_expr(self, p):
        'line : expr NEWLINE'
        print(f"ONP: {self.ONP}")
        print(f"Wynik: {p[1]}")
        self.ONP = ""
    
    def p_line_error(self, p):
        'line : error NEWLINE'
        if self.error_msg:
            print(f"Błąd: {self.error_msg}")
            self.error_msg = ""
        else:
            print("Błąd: Zła składnia")
        self.ONP = ""
    
    def p_expr_simple(self, p):
        'expr : simple_expr'
        p[0] = p[1]
    
    def p_expr_plus(self, p):
        'expr : expr PLUS expr'
        p[0] = (p[1] + p[3]) % P
        self.ONP += "+ "
    
    def p_expr_minus(self, p):
        'expr : expr MINUS expr'
        p[0] = (p[1] - p[3] + P) % P
        self.ONP += "- "
    
    def p_expr_mult(self, p):
        'expr : expr MULT expr'
        p[0] = (p[1] * p[3]) % P
        self.ONP += "* "
    
    def p_expr_div(self, p):
        'expr : expr DIV expr'
        if p[3] == 0:
            self.error_msg = "Dzielenie przez 0"
            raise SyntaxError
        inv = mod_inverse(p[3], P)
        if inv == -1:
            self.error_msg = "Dzielenie przez liczbę nieodwracalną w GF(P)"
            raise SyntaxError
        p[0] = (p[1] * inv) % P
        self.ONP += "/ "
    
    def p_simple_expr(self, p):
        'simple_expr : power_expr'
        p[0] = p[1]
    
    def p_power_expr_atom(self, p):
        'power_expr : atom'
        p[0] = p[1]
    
    def p_power_expr_pow(self, p):
        'power_expr : atom POW exponent'
        base_val = p[1] % P
        exp_val = p[3]
        result = 1
        
        while exp_val > 0:
            if exp_val % 2 == 1:
                result = (result * base_val) % P
            base_val = (base_val * base_val) % P
            exp_val //= 2
        
        p[0] = result
        self.ONP += "^ "
    
    def p_atom_number(self, p):
        'atom : NUMBER_CHANGED'
        p[0] = p[1]
        self.ONP += f"{p[1]} "
    
    def p_atom_paren(self, p):
        'atom : LPAREN expr RPAREN'
        p[0] = p[2]
    
    def p_exponent_simple(self, p):
        'exponent : EXP_SIMPLE'
        p[0] = p[1]
    
    def p_exponent_complex(self, p):
        'exponent : LPAREN EXP_COMPLEX RPAREN'
        p[0] = p[2]
    
    def p_NUMBER_CHANGED_number(self, p):
        'NUMBER_CHANGED : NUMBER'
        p[0] = (p[1] % P + P) % P
    
    def p_NUMBER_CHANGED_uminus(self, p):
        'NUMBER_CHANGED : MINUS NUMBER'
        p[0] = (-p[2] % P + P) % P
    
    def p_EXP_SIMPLE_number(self, p):
        'EXP_SIMPLE : NUMBER'
        p[0] = toGF_exp(p[1])
        self.ONP += f"{p[1]} "
    
    def p_EXP_SIMPLE_uminus(self, p):
        'EXP_SIMPLE : MINUS NUMBER'
        p[0] = toGF_exp(-p[2])
        self.ONP += f"{p[0]} "
    
    def p_EXP_COMPLEX_simple(self, p):
        'EXP_COMPLEX : EXP_SIMPLE'
        p[0] = p[1]
    
    def p_EXP_COMPLEX_plus(self, p):
        'EXP_COMPLEX : EXP_COMPLEX PLUS EXP_SIMPLE'
        p[0] = toGF_exp(p[1] + p[3])
        self.ONP += "+ "
    
    def p_EXP_COMPLEX_minus(self, p):
        'EXP_COMPLEX : EXP_COMPLEX MINUS EXP_SIMPLE'
        p[0] = toGF_exp(p[1] - p[3])
        self.ONP += "- "
    
    def p_EXP_COMPLEX_mult(self, p):
        'EXP_COMPLEX : EXP_COMPLEX MULT EXP_SIMPLE'
        p[0] = toGF_exp(p[1] * p[3])
        self.ONP += "* "
    
    def p_EXP_COMPLEX_div(self, p):
        'EXP_COMPLEX : EXP_COMPLEX DIV EXP_SIMPLE'
        if p[3] == 0:
            self.error_msg = "Dzielenie przez 0 w wykładniku"
            raise SyntaxError
        
        if p[1] % p[3] == 0:
            p[0] = p[1] // p[3]
            self.ONP += "/ "
        else:
            inv = mod_inverse(p[3], P - 1)
            if inv == -1:
                self.error_msg = "Dzielenie przez liczbę nieodwracalną w GF(P-1)"
                raise SyntaxError
            p[0] = toGF_exp(p[1] * inv)
            self.ONP += "/ "
    
    def p_error(self, p):
        if not self.error_msg:
            self.error_msg = "Zła składnia"

# --- MAIN ---
if __name__ == "__main__":
    parser = Parser()
    print("Podaj wyrażenie:")
    
    while True:
        try:
            line = input()
            if not line:
                continue
            result = parser.parse(line + '\n')
        except EOFError:
            break
        except Exception as e:
            print(f"Błąd: {e}")