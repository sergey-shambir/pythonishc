%{
#include <stdio.h>
#include <math.h>
#include <string.h>

int g_line = 0;
int g_column = 0;
%}

NON_EOLN .
EOLN \n

DIGIT [0-9]
ID    [a-zA-Z][a-zA-Z0-9]*
OPERATOR [\+\-\*\/\%]
COMMENT "//".*
BRANCHING if|else|while|for|break|continue|goto|case|switch|try|throw|catch
TYPES     int|long|short|unsigned|signed|float|double|char|struct|union
OTHER_KEYWORDS inline|static|const|volatile|typedef
PREPROCESSOR "#".*
STRING \".*\"

%s COMMENT

%%

<INITIAL>"/*"   {
        BEGIN COMMENT;
        g_column += strlen(yytext);
    }

<COMMENT>"*/"   {
        BEGIN INITIAL;
        g_column += strlen(yytext);
    }

<COMMENT>. {
        g_column += strlen(yytext);
    }

<INITIAL>{COMMENT}         /* eat up one-line comments */

<INITIAL>{STRING} {
        printf("String literal: %s \n", yytext);
    }

<INITIAL>{DIGIT}+ {
        printf("An integer: %s \n", yytext);
        g_column += strlen(yytext);
    }
    
<INITIAL>{DIGIT}+"."{DIGIT}* {
        printf("A float: %s \n", yytext);
        g_column += strlen(yytext);
    }

<INITIAL>{OPERATOR} {
        printf( "An operator %s\n", yytext );
        g_column += strlen(yytext);
    }
                
<INITIAL>{BRANCHING} {
        printf("A flow controlling instruction: %s \n", yytext);
        g_column += strlen(yytext);
    }

<INITIAL>{TYPES} {
        printf("A builtin type: %s \n", yytext);
        g_column += strlen(yytext);
    }

<INITIAL>{OTHER_KEYWORDS} {
        printf("Special keyword: %s \n", yytext);
        g_column += strlen(yytext);
    }

<INITIAL>{PREPROCESSOR} {
        printf("Preprocessor directive: %s \n", yytext);
        g_column += strlen(yytext);
    }

<INITIAL>{ID} {
        printf("An identifier: %s \n", yytext );
        g_column += strlen(yytext);
    }

<INITIAL>\; {
        printf("Semicolon\n");
    }

<INITIAL>\, {
        printf("Comma\n");
    }

<INITIAL>\( {
        printf("Left bracket\n");
    }

<INITIAL>\) {
        printf("Right bracket\n");
    }

<INITIAL>. {
        g_column += strlen(yytext);
    }

[ \t]+  {
        g_column += strlen(yytext);
    }

{EOLN} {
        ++g_line;
        g_column = 0;
    }
%%

int main()
{
    yylex();
    printf( "# of lines = %d, # of chars = %d\n", g_line, g_column );
    return 0;
}
