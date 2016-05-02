/** Файл содержит основанный на Bison LALR-парсер,
    способный проверить принадлежность входных данных грамматике языка.

    Основано на https://github.com/bingmann/flex-bison-cpp-example/blob/master/src/parser.yy
    */

%start program

%{
#include <stdio.h>
#include "globals.h"

/*
    Функция для вывода сообщения об ошибке должна быть определена вручную.
    При этом мы определяем checker_error вместо yyerror,
    так как ранее применили директиву %name-prefix
*/
void checker_error (char const *s) {
    ++g_errorsCount;
    fprintf (stderr, "Error: %s\n", s);
}
%}

/* Директива вызовет генерацию файла с объявлениями токенов.
   Генератор парсеров должен знать целочисленные коды токенов,
   чтобы сгенерировать таблицу разбора. */
%defines

/* Префикс будет добавлен к идентификаторам генерируемых функций */
%name-prefix "checker_"

/* Подробные сообщения об ошибках разбора по грамматике. */
%error-verbose

%token END 0    "end of file"
%token NEWLINE  "end of line"
%token NUMBER   "Number constant"
%token STRING   "String constant"
%token BOOL     "Bool constant"
%token ID       "Identifier"
%token PRINT    "print"
%token WHILE    "while"
%token DO       "do"
%token IF       "if"
%token ELSE     "else"
%token RETURN   "return"
%token BLOCK_END "end"
%token FUNCTION "def"
%token EQUALS   "operator =="
%token AND      "operator &&"
%token OR       "operator ||"

/* %left, %right, %nonassoc и %precedence управляют разрешением
   приоритета операторов и правил ассоциативности

   Документация Bison: http://www.gnu.org/software/bison/manual/bison.html#Precedence-Decl
*/
%left '<' EQUALS
%left AND OR NOT
%left '+' '-'
%left '*' '/' '%'

%% /* Грамматические правила */

epsilon : /*empty*/

constant : BOOL | NUMBER | STRING

variable : ID

function_call : ID '(' expression_list ')'

expression : constant | variable | '(' expression ')'
        | '+' expression | '-' expression | NOT expression
        | expression '<' expression | expression EQUALS expression
        | expression AND expression | expression OR expression
        | expression '+' expression | expression '-' expression
        | expression '*' expression | expression '/' expression
        | expression '%' expression
        | function_call

expression_list : epsilon | expression | expression_list ',' expression

statement : PRINT expression_list
          | variable '=' expression
          | RETURN expression
          | IF expression block
          | IF expression NEWLINE statement_list ELSE block
          | IF expression NEWLINE ELSE block
          | WHILE expression block
          | DO NEWLINE statement_list WHILE expression BLOCK_END
          | DO NEWLINE WHILE expression BLOCK_END

statement_line : error NEWLINE | statement NEWLINE

statement_list : statement_line | statement_list statement_line

block : NEWLINE statement_list BLOCK_END

block : NEWLINE BLOCK_END

parameter_list : ID | parameter_list ID

function_declaration : FUNCTION ID '(' parameter_list ')' block
                     | FUNCTION ID '(' ')' block

toplevel_statement : function_declaration | statement

toplevel_line : NEWLINE | toplevel_statement NEWLINE | error NEWLINE

toplevel_list : toplevel_line | toplevel_list toplevel_line

program : toplevel_list
