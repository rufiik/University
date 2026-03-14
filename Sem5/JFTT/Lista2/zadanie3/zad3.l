%{
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

bool keep_doc_comments = false;   
    

void printDoc(const char *text) {
    fputs(text, stdout);
}
%}

%x C_COMMENT STR CHAR DOC_COMMENT

%%

\" {
    printDoc(yytext);
    BEGIN(STR);
}
<STR>\\.|[^"\\\n] { printDoc(yytext); }     
<STR>\"            { printDoc(yytext); BEGIN(INITIAL); } 
\' {
    printDoc(yytext);
    BEGIN(CHAR);
}
<CHAR>\\.|[^'\\\n] { printDoc(yytext); }
<CHAR>\'            { printDoc(yytext); BEGIN(INITIAL); }

"//!"[^\n]*        { if (keep_doc_comments) printDoc(yytext); }
"///"[^\n]*        { if (keep_doc_comments) printDoc(yytext); }

"/*!"              { 
                    if (keep_doc_comments) {
                        printDoc(yytext); 
                        BEGIN(DOC_COMMENT);
                    } else {
                        BEGIN(C_COMMENT);
                    }
                   }
"/**"              { 
                    if (keep_doc_comments) {
                        printDoc(yytext); 
                        BEGIN(DOC_COMMENT);
                    } else {
                        BEGIN(C_COMMENT);
                    }
                   }

"/*"               { BEGIN(C_COMMENT); }
"//"[^\n]*         {  }

<C_COMMENT>[^*]+         { }
<C_COMMENT>"*"+[^*/]     { }
<C_COMMENT>"*"+"/"       { BEGIN(INITIAL); }

<DOC_COMMENT>[^*]+         { if (keep_doc_comments) printDoc(yytext); }
<DOC_COMMENT>"*"+[^*/]     { if (keep_doc_comments) printDoc(yytext); }
<DOC_COMMENT>"*"+"/"       { if (keep_doc_comments) printDoc(yytext); BEGIN(INITIAL); }

.|\n { printDoc(yytext); }

%%

int yywrap() { return 1; }

int main(int argc, char **argv) {
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-d") == 0)
            keep_doc_comments = true;
    }

    yylex();
    return 0;
}