%{

#include "grammar.gen.h"
#define yylex checker_lex

%}

NUMBER [0-9].?[0-9]*
BOOL true|false
STRING \".*\"
ID [a-zA-Z][a-zA-Z0-9_]*

/* Опция, необходимая для языков с препроцессорами и #include,
   в нашем случае не нужна */
%option noyywrap

%%

"("     return '(';
")"     return ')';
"="     return '=';
"+"     return '+';
"-"     return '-';
"*"     return '*';
"/"     return '/';
"%"     return '%';
"<"     return '<';
"!"     return '!';
"=="    return EQUALS;
"&&"    return AND;
"||"    return OR;
{BOOL}    return BOOL;
{NUMBER}  return NUMBER;
{STRING}  return STRING;
return  return RETURN;
while   return WHILE;
if      return IF;
else    return ELSE;
print   return PRINT;
end     return BLOCK_END;
def     return FUNCTION;
{ID}    return ID;
\n      return NEWLINE;
[ \t]+  /* skip */
