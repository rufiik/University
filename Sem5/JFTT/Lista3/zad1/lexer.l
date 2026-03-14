%{
#include "parser.tab.h"
#include <stdlib.h>
#include <ctype.h>
%}

%%
[0-9]+   {
    yylval = atoi(yytext); 
    return NUMBER;
}

"+"      { return PLUS; }
"*"      { return MULT; }
"/"      { return DIV; }
"^"      { return POW; }
"-"      { return MINUS; }
"("      { return LPAREN; }
")"      { return RPAREN; }
^#(.|\\\n)*\n   { }
"\\\n"          { }
[ \t]+          { }
\n              { return '\n'; }
.               { printf("Nieznany znak: %s\n", yytext); }

%%

int yywrap() { return 1; }