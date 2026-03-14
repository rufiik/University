%{
#include <stdio.h>

int in_line = 0;       
int pending_space = 0; 
int lines = 1;
int words = 0;
%}

WHITESPACE     [ \t]+
LEFTWS         ^[ \t]+
RIGHTWS        [ \t]+$
WORD           [^ \t\r\n]+
EOL            (\r\n|\n)

%%

{LEFTWS}       { }

{RIGHTWS}      { pending_space = 0; }

{WHITESPACE}   {
                  if (in_line) pending_space = 1;
                }

{WORD}         {
                 if (pending_space && in_line) {
                      putchar(' ');
                      pending_space = 0;
                  }
                  fputs(yytext, stdout);
                  in_line = 1;
                  words++;          
                }

{EOL}          {
                if(in_line) {
                  putchar('\n');
                  lines++;
                  
                }
                  pending_space = 0;
                  in_line = 0;
                }

%%

int yywrap() { return 1; }

int main(void) {
    yylex();
    printf("\nLiczba linii: %d\nLiczba słów: %d\n", lines, words);
    return 0;
}
