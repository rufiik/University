%{
#include <stdio.h>
#include <string.h>

%}


%x IN_TRIPLE_SINGLE IN_TRIPLE_DOUBLE COMMENT_TRIPLE_SINGLE COMMENT_TRIPLE_DOUBLE

%%

^[ \t]*"'''" {
    BEGIN(COMMENT_TRIPLE_SINGLE);
}
^[ \t]*"\"\"\"" {
    BEGIN(COMMENT_TRIPLE_DOUBLE);
}

"'''" {
    BEGIN(IN_TRIPLE_SINGLE);
    fputs(yytext, stdout);
}
<IN_TRIPLE_SINGLE>"'''" {
    fputs(yytext, stdout);
    BEGIN(INITIAL);
}
<IN_TRIPLE_SINGLE>.|\n {
    fputs(yytext, stdout);
}

"\"\"\"" {
    BEGIN(IN_TRIPLE_DOUBLE);
    fputs(yytext, stdout);
}
<IN_TRIPLE_DOUBLE>"\"\"\"" {
    fputs(yytext, stdout);
    BEGIN(INITIAL);
}
<IN_TRIPLE_DOUBLE>.|\n {
    fputs(yytext, stdout);
}


<COMMENT_TRIPLE_SINGLE>"'''" { BEGIN(INITIAL); }
<COMMENT_TRIPLE_SINGLE>.|\n   { /* pomiń */ }

<COMMENT_TRIPLE_DOUBLE>"\"\"\"" { BEGIN(INITIAL); }
<COMMENT_TRIPLE_DOUBLE>.|\n     { /* pomiń */ }


"#"[^\n]*  {  }


\"(\\.|[^"\\\n])*\"  { fputs(yytext, stdout); }
\'(\\.|[^'\\\n])*\'  { fputs(yytext, stdout); }


\n  { fputs(yytext, stdout); }


.   { fputs(yytext, stdout); }

%%
int yywrap() { 
    printf("\n");
    return 1; 
    }

int main(void) {
    yylex();
    return 0;
}
