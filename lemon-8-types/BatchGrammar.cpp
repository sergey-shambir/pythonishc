/* Driver template for the LEMON parser generator.
** The author disclaims copyright to this source code.
*/
/* First off, code is included that follows the "include" declaration
** in the input grammar file. */
#include <stdio.h>

// Generated function: void ParseBatchGrammar(void*, int, SToken, CBatchParser*);

#include "BatchParser_private.h"

using namespace parser_private;

// Indicate that ParseBatchGrammarFree() will never be called with a nullptr.
#define YYPARSEFREENEVERNULL 1

/* Next is all token values, in a form suitable for use by makeheaders.
** This section will be null unless lemon is run with the -m switch.
*/
/* 
** These constants (all generated automatically by the parser generator)
** specify the various kinds of tokens (terminals) that the parser
** understands. 
**
** Each symbol here is a terminal symbol in the grammar.
*/
/* Make sure the INTERFACE macro is defined.
*/
#ifndef INTERFACE
# define INTERFACE 1
#endif
/* The next thing included is series of defines which control
** various aspects of the generated parser.
**    YYCODETYPE         is the data type used for storing terminal
**                       and nonterminal numbers.  "unsigned char" is
**                       used if there are fewer than 250 terminals
**                       and nonterminals.  "int" is used otherwise.
**    YYNOCODE           is a number of type YYCODETYPE which corresponds
**                       to no legal terminal or nonterminal number.  This
**                       number is used to fill in empty slots of the hash 
**                       table.
**    YYFALLBACK         If defined, this indicates that one or more tokens
**                       have fall-back values which should be used if the
**                       original value of the token will not parse.
**    YYACTIONTYPE       is the data type used for storing terminal
**                       and nonterminal numbers.  "unsigned char" is
**                       used if there are fewer than 250 rules and
**                       states combined.  "int" is used otherwise.
**    ParseBatchGrammarTOKENTYPE     is the data type used for minor tokens given 
**                       directly to the parser from the tokenizer.
**    YYMINORTYPE        is the data type used for all minor tokens.
**                       This is typically a union of many types, one of
**                       which is ParseBatchGrammarTOKENTYPE.  The entry in the union
**                       for base tokens is called "yy0".
**    YYSTACKDEPTH       is the maximum depth of the parser's stack.  If
**                       zero the stack is dynamically sized using realloc()
**    ParseBatchGrammarARG_SDECL     A static variable declaration for the %extra_argument
**    ParseBatchGrammarARG_PDECL     A parameter declaration for the %extra_argument
**    ParseBatchGrammarARG_STORE     Code to store %extra_argument into yypParser
**    ParseBatchGrammarARG_FETCH     Code to extract %extra_argument from yypParser
**    YYNSTATE           the combined number of states.
**    YYNRULE            the number of rules in the grammar
**    YYERRORSYMBOL      is the code number of the error symbol.  If not
**                       defined, then do no error processing.
*/
#define YYCODETYPE unsigned char
#define YYNOCODE 38
#define YYACTIONTYPE unsigned char
#define ParseBatchGrammarTOKENTYPE SToken
typedef union {
  int yyinit;
  ParseBatchGrammarTOKENTYPE yy0;
  ExpressionPtr yy35;
  StatementPtr yy40;
  StatementListPtr yy46;
  FunctionPtr yy61;
  NamesListPtr yy65;
  ExpressionListPtr yy69;
  int yy75;
} YYMINORTYPE;
#ifndef YYSTACKDEPTH
#define YYSTACKDEPTH 100
#endif
#define ParseBatchGrammarARG_SDECL CBatchParser *pParse;
#define ParseBatchGrammarARG_PDECL ,CBatchParser *pParse
#define ParseBatchGrammarARG_FETCH CBatchParser *pParse = yypParser->pParse
#define ParseBatchGrammarARG_STORE yypParser->pParse = pParse
#define YYNSTATE 95
#define YYNRULE 43
#define YYERRORSYMBOL 25
#define YYERRSYMDT yy75
#define YY_NO_ACTION      (YYNSTATE+YYNRULE+2)
#define YY_ACCEPT_ACTION  (YYNSTATE+YYNRULE+1)
#define YY_ERROR_ACTION   (YYNSTATE+YYNRULE)

/* The yyzerominor constant is used to initialize instances of
** YYMINORTYPE objects to zero. */
static const YYMINORTYPE yyzerominor = { 0 };

/* Define the yytestcase() macro to be a no-op if is not already defined
** otherwise.
**
** Applications can choose to define yytestcase() in the %include section
** to a macro that can assist in verifying code coverage.  For production
** code the yytestcase() macro should be turned off.  But it is useful
** for testing.
*/
#ifndef yytestcase
# define yytestcase(X)
#endif


/* Next are the tables used to determine what action to take based on the
** current state and lookahead token.  These tables are used to implement
** functions that take a state number and lookahead value and return an
** action integer.  
**
** Suppose the action integer is N.  Then the action is determined as
** follows
**
**   0 <= N < YYNSTATE                  Shift N.  That is, push the lookahead
**                                      token onto the stack and goto state N.
**
**   YYNSTATE <= N < YYNSTATE+YYNRULE   Reduce by rule N-YYNSTATE.
**
**   N == YYNSTATE+YYNRULE              A syntax error has occurred.
**
**   N == YYNSTATE+YYNRULE+1            The parser accepts its input.
**
**   N == YYNSTATE+YYNRULE+2            No such action.  Denotes unused
**                                      slots in the yy_action[] table.
**
** The action table is constructed as a single large table named yy_action[].
** Given state S and lookahead X, the action is computed as
**
**      yy_action[ yy_shift_ofst[S] + X ]
**
** If the index value yy_shift_ofst[S]+X is out of range or if the value
** yy_lookahead[yy_shift_ofst[S]+X] is not equal to X or if yy_shift_ofst[S]
** is equal to YY_SHIFT_USE_DFLT, it means that the action is not in the table
** and that yy_default[S] should be used instead.  
**
** The formula above is for computing the action when the lookahead is
** a terminal symbol.  If the lookahead is a non-terminal (as occurs after
** a reduce action) then the yy_reduce_ofst[] array is used in place of
** the yy_shift_ofst[] array and YY_REDUCE_USE_DFLT is used in place of
** YY_SHIFT_USE_DFLT.
**
** The following are the tables generated in this section:
**
**  yy_action[]        A single table containing all actions.
**  yy_lookahead[]     A table containing the lookahead for each entry in
**                     yy_action.  Used to detect hash collisions.
**  yy_shift_ofst[]    For each state, the offset into yy_action for
**                     shifting terminals.
**  yy_reduce_ofst[]   For each state, the offset into yy_action for
**                     shifting non-terminals after a reduce.
**  yy_default[]       Default action for each state.
*/
#define YY_ACTTAB_COUNT (261)
static const YYACTIONTYPE yy_action[] = {
 /*     0 */    31,   30,   28,   27,   26,   25,   24,    2,   26,   25,
 /*    10 */    24,   66,   72,   60,   31,   30,   28,   27,   26,   25,
 /*    20 */    24,    2,   58,   93,   59,   88,   71,   92,   31,   30,
 /*    30 */    28,   27,   26,   25,   24,   78,   33,   21,   53,   85,
 /*    40 */    52,   31,   30,   28,   27,   26,   25,   24,    3,   31,
 /*    50 */    30,   28,   27,   26,   25,   24,    2,   23,   22,   28,
 /*    60 */    27,   26,   25,   24,   56,   29,   77,   31,   30,   28,
 /*    70 */    27,   26,   25,   24,   62,   57,   81,   80,   79,   61,
 /*    80 */     7,   20,   19,   18,   42,   17,   54,   57,   51,   87,
 /*    90 */    75,   95,   86,   20,   19,   18,   55,   17,   54,   14,
 /*   100 */    62,   57,   32,    6,    5,    4,   50,   20,   19,   18,
 /*   110 */    67,   17,   54,   23,   22,   43,   44,  140,   45,   57,
 /*   120 */    56,   29,   89,  140,   38,   20,   19,   18,   49,   17,
 /*   130 */    54,   48,   81,   80,   79,  140,   57,   84,   83,   74,
 /*   140 */    82,   47,   20,   19,   18,   57,   17,   54,   73,   46,
 /*   150 */   140,   20,   19,   18,   57,   17,   54,   68,   41,   40,
 /*   160 */    20,   19,   18,   57,   17,   54,   69,   39,   37,   20,
 /*   170 */    19,   18,   57,   17,   54,   70,   36,  140,   20,   19,
 /*   180 */    18,   35,   17,   54,   34,  140,   63,  140,   91,  140,
 /*   190 */   140,  140,   90,   57,  139,    1,   65,   64,  140,   20,
 /*   200 */    19,   18,  140,   16,   54,   57,   58,  140,   59,   76,
 /*   210 */    12,   20,   19,   18,   57,   17,   54,  140,  140,  140,
 /*   220 */    20,   19,   18,  140,   15,   54,   63,   58,   91,   59,
 /*   230 */    76,   11,   90,  140,  140,  140,   94,   64,   58,  140,
 /*   240 */    59,   76,   13,  140,  140,  140,   58,  140,   59,   76,
 /*   250 */     9,   58,  140,   59,   76,    8,   58,  140,   59,   76,
 /*   260 */    10,
};
static const YYCODETYPE yy_lookahead[] = {
 /*     0 */     1,    2,    3,    4,    5,    6,    7,    8,    5,    6,
 /*    10 */     7,   10,   13,   12,    1,    2,    3,    4,    5,    6,
 /*    20 */     7,    8,   25,    8,   27,   28,   13,    8,    1,    2,
 /*    30 */     3,    4,    5,    6,    7,   12,   11,   14,   12,   12,
 /*    40 */    14,    1,    2,    3,    4,    5,    6,    7,    8,    1,
 /*    50 */     2,    3,    4,    5,    6,    7,    8,    3,    4,    3,
 /*    60 */     4,    5,    6,    7,   10,   11,   12,    1,    2,    3,
 /*    70 */     4,    5,    6,    7,    9,   10,   22,   23,   24,   10,
 /*    80 */     8,   16,   17,   18,   26,   20,   21,   10,   30,    8,
 /*    90 */    13,    0,    8,   16,   17,   18,   19,   20,   21,   11,
 /*   100 */     9,   10,   15,    8,    8,    8,   32,   16,   17,   18,
 /*   110 */    10,   20,   21,    3,    4,   26,   26,   37,   26,   10,
 /*   120 */    10,   11,   13,   37,   26,   16,   17,   18,   26,   20,
 /*   130 */    21,   26,   22,   23,   24,   37,   10,   26,   26,   13,
 /*   140 */    26,   26,   16,   17,   18,   10,   20,   21,   13,   26,
 /*   150 */    37,   16,   17,   18,   10,   20,   21,   13,   26,   26,
 /*   160 */    16,   17,   18,   10,   20,   21,   13,   26,   26,   16,
 /*   170 */    17,   18,   10,   20,   21,   13,   26,   37,   16,   17,
 /*   180 */    18,   26,   20,   21,   26,   37,   25,   37,   27,   37,
 /*   190 */    37,   37,   31,   10,   33,   34,   35,   36,   37,   16,
 /*   200 */    17,   18,   37,   20,   21,   10,   25,   37,   27,   28,
 /*   210 */    29,   16,   17,   18,   10,   20,   21,   37,   37,   37,
 /*   220 */    16,   17,   18,   37,   20,   21,   25,   25,   27,   27,
 /*   230 */    28,   29,   31,   37,   37,   37,   35,   36,   25,   37,
 /*   240 */    27,   28,   29,   37,   37,   37,   25,   37,   27,   28,
 /*   250 */    29,   25,   37,   27,   28,   29,   25,   37,   27,   28,
 /*   260 */    29,
};
#define YY_SHIFT_USE_DFLT (-2)
#define YY_SHIFT_COUNT (64)
#define YY_SHIFT_MIN   (-1)
#define YY_SHIFT_MAX   (204)
static const short yy_shift_ofst[] = {
 /*     0 */    65,   91,  162,  153,  195,  204,  195,  195,   77,  144,
 /*    10 */   135,  126,  109,  183,   54,  110,  110,  110,  110,  110,
 /*    20 */   110,  110,  110,  110,  110,  110,  110,  110,  110,  110,
 /*    30 */   110,  110,  110,    1,   13,   -1,   48,   40,   27,   66,
 /*    40 */    66,   66,   66,   66,   56,   56,    3,    3,    3,    3,
 /*    50 */    26,   23,  100,   97,   96,   95,   88,   87,   84,   81,
 /*    60 */    72,   25,   69,   19,   15,
};
#define YY_REDUCE_USE_DFLT (-4)
#define YY_REDUCE_COUNT (33)
#define YY_REDUCE_MIN   (-3)
#define YY_REDUCE_MAX   (231)
static const short yy_reduce_ofst[] = {
 /*     0 */   161,  201,  231,  226,  221,  213,  202,  181,   -3,   -3,
 /*    10 */    -3,   -3,   -3,   -3,   58,  158,  155,  150,  142,  141,
 /*    20 */   133,  132,  123,  115,  114,  112,  111,  105,  102,   98,
 /*    30 */    92,   90,   89,   74,
};
static const YYACTIONTYPE yy_default[] = {
 /*     0 */   138,  138,  138,  138,  138,  138,  138,  138,  138,  138,
 /*    10 */   138,  138,  138,  138,  138,  138,  138,  138,  138,  138,
 /*    20 */   138,  138,  138,  138,  138,  138,  138,  138,  138,  138,
 /*    30 */   138,  138,  138,  138,  138,  138,  138,  138,  138,  112,
 /*    40 */   111,  121,  120,  110,  125,  126,  133,  132,  128,  127,
 /*    50 */   138,  138,  138,  138,  138,  138,  137,  138,  138,  138,
 /*    60 */   138,  138,  138,  138,  138,   96,  104,  105,  103,  113,
 /*    70 */   116,  118,  119,  117,  115,  114,  106,  122,  123,  136,
 /*    80 */   135,  134,  131,  130,  129,  124,  109,  108,  107,  102,
 /*    90 */   101,  100,   99,   98,   97,
};

/* The next table maps tokens into fallback tokens.  If a construct
** like the following:
** 
**      %fallback ID X Y Z.
**
** appears in the grammar, then ID becomes a fallback token for X, Y,
** and Z.  Whenever one of the tokens X, Y, or Z is input to the parser
** but it does not parse, the type of the token is changed to ID and
** the parse is retried before an error is thrown.
*/
#ifdef YYFALLBACK
static const YYCODETYPE yyFallback[] = {
};
#endif /* YYFALLBACK */

/* The following structure represents a single element of the
** parser's stack.  Information stored includes:
**
**   +  The state number for the parser at this level of the stack.
**
**   +  The value of the token stored at this level of the stack.
**      (In other words, the "major" token.)
**
**   +  The semantic value stored at this level of the stack.  This is
**      the information used by the action routines in the grammar.
**      It is sometimes called the "minor" token.
*/
struct yyStackEntry {
  YYACTIONTYPE stateno;  /* The state-number */
  YYCODETYPE major;      /* The major token value.  This is the code
                         ** number for the token at this stack level */
  YYMINORTYPE minor;     /* The user-supplied minor token value.  This
                         ** is the value of the token  */
};
typedef struct yyStackEntry yyStackEntry;

/* The state of the parser is completely contained in an instance of
** the following structure */
struct yyParser {
  int yyidx;                    /* Index of top element in stack */
#ifdef YYTRACKMAXSTACKDEPTH
  int yyidxMax;                 /* Maximum value of yyidx */
#endif
  int yyerrcnt;                 /* Shifts left before out of the error */
  ParseBatchGrammarARG_SDECL                /* A place to hold %extra_argument */
#if YYSTACKDEPTH<=0
  int yystksz;                  /* Current side of the stack */
  yyStackEntry *yystack;        /* The parser's stack */
#else
  yyStackEntry yystack[YYSTACKDEPTH];  /* The parser's stack */
#endif
};
typedef struct yyParser yyParser;

#ifndef NDEBUG
#include <stdio.h>
static FILE *yyTraceFILE = 0;
static char *yyTracePrompt = 0;
#endif /* NDEBUG */

#ifndef NDEBUG
/* 
** Turn parser tracing on by giving a stream to which to write the trace
** and a prompt to preface each trace message.  Tracing is turned off
** by making either argument NULL 
**
** Inputs:
** <ul>
** <li> A FILE* to which trace output should be written.
**      If NULL, then tracing is turned off.
** <li> A prefix string written at the beginning of every
**      line of trace output.  If NULL, then tracing is
**      turned off.
** </ul>
**
** Outputs:
** None.
*/
void ParseBatchGrammarTrace(FILE *TraceFILE, char *zTracePrompt){
  yyTraceFILE = TraceFILE;
  yyTracePrompt = zTracePrompt;
  if( yyTraceFILE==0 ) yyTracePrompt = 0;
  else if( yyTracePrompt==0 ) yyTraceFILE = 0;
}
#endif /* NDEBUG */

#ifndef NDEBUG
/* For tracing shifts, the names of all terminals and nonterminals
** are required.  The following table supplies these names */
static const char *const yyTokenName[] = { 
  "$",             "LESS",          "EQUALS",        "PLUS",        
  "MINUS",         "STAR",          "SLASH",         "PERCENT",     
  "NEWLINE",       "FUNCTION",      "ID",            "LPAREN",      
  "RPAREN",        "END",           "COMMA",         "ASSIGN",      
  "PRINT",         "RETURN",        "IF",            "ELSE",        
  "WHILE",         "DO",            "NUMBER",        "STRING",      
  "BOOL",          "error",         "expression",    "statement",   
  "statement_line",  "statement_list",  "expression_list",  "function_declaration",
  "parameter_list",  "translation_unit",  "toplevel_list",  "toplevel_line",
  "toplevel_statement",
};
#endif /* NDEBUG */

#ifndef NDEBUG
/* For tracing reduce actions, the names of all rules are required.
*/
static const char *const yyRuleName[] = {
 /*   0 */ "translation_unit ::= toplevel_list",
 /*   1 */ "toplevel_list ::= toplevel_line",
 /*   2 */ "toplevel_list ::= toplevel_list toplevel_line",
 /*   3 */ "toplevel_line ::= toplevel_statement NEWLINE",
 /*   4 */ "toplevel_line ::= error NEWLINE",
 /*   5 */ "toplevel_statement ::= statement",
 /*   6 */ "toplevel_statement ::= function_declaration",
 /*   7 */ "function_declaration ::= FUNCTION ID LPAREN RPAREN NEWLINE statement_list END",
 /*   8 */ "function_declaration ::= FUNCTION ID LPAREN parameter_list RPAREN NEWLINE statement_list END",
 /*   9 */ "parameter_list ::= ID",
 /*  10 */ "parameter_list ::= parameter_list COMMA ID",
 /*  11 */ "statement_list ::= statement_line",
 /*  12 */ "statement_list ::= statement_list statement_line",
 /*  13 */ "statement_line ::= statement NEWLINE",
 /*  14 */ "statement_line ::= error NEWLINE",
 /*  15 */ "statement ::= ID ASSIGN expression",
 /*  16 */ "statement ::= PRINT expression",
 /*  17 */ "statement ::= RETURN expression",
 /*  18 */ "statement ::= IF expression NEWLINE END",
 /*  19 */ "statement ::= IF expression NEWLINE statement_list END",
 /*  20 */ "statement ::= IF expression NEWLINE statement_list ELSE NEWLINE statement_list END",
 /*  21 */ "statement ::= WHILE expression NEWLINE END",
 /*  22 */ "statement ::= WHILE expression NEWLINE statement_list END",
 /*  23 */ "statement ::= DO NEWLINE WHILE expression END",
 /*  24 */ "statement ::= DO NEWLINE statement_list WHILE expression END",
 /*  25 */ "expression_list ::= expression",
 /*  26 */ "expression_list ::= expression_list COMMA expression",
 /*  27 */ "expression ::= ID LPAREN RPAREN",
 /*  28 */ "expression ::= ID LPAREN expression_list RPAREN",
 /*  29 */ "expression ::= LPAREN expression RPAREN",
 /*  30 */ "expression ::= expression LESS expression",
 /*  31 */ "expression ::= expression EQUALS expression",
 /*  32 */ "expression ::= expression PLUS expression",
 /*  33 */ "expression ::= expression MINUS expression",
 /*  34 */ "expression ::= expression STAR expression",
 /*  35 */ "expression ::= expression SLASH expression",
 /*  36 */ "expression ::= expression PERCENT expression",
 /*  37 */ "expression ::= PLUS expression",
 /*  38 */ "expression ::= MINUS expression",
 /*  39 */ "expression ::= NUMBER",
 /*  40 */ "expression ::= STRING",
 /*  41 */ "expression ::= BOOL",
 /*  42 */ "expression ::= ID",
};
#endif /* NDEBUG */


#if YYSTACKDEPTH<=0
/*
** Try to increase the size of the parser stack.
*/
static void yyGrowStack(yyParser *p){
  int newSize;
  yyStackEntry *pNew;

  newSize = p->yystksz*2 + 100;
  pNew = realloc(p->yystack, newSize*sizeof(pNew[0]));
  if( pNew ){
    p->yystack = pNew;
    p->yystksz = newSize;
#ifndef NDEBUG
    if( yyTraceFILE ){
      fprintf(yyTraceFILE,"%sStack grows to %d entries!\n",
              yyTracePrompt, p->yystksz);
    }
#endif
  }
}
#endif

/* 
** This function allocates a new parser.
** The only argument is a pointer to a function which works like
** malloc.
**
** Inputs:
** A pointer to the function used to allocate memory.
**
** Outputs:
** A pointer to a parser.  This pointer is used in subsequent calls
** to ParseBatchGrammar and ParseBatchGrammarFree.
*/
void *ParseBatchGrammarAlloc(void *(*mallocProc)(size_t)){
  yyParser *pParser;
  pParser = (yyParser*)(*mallocProc)( (size_t)sizeof(yyParser) );
  if( pParser ){
    pParser->yyidx = -1;
#ifdef YYTRACKMAXSTACKDEPTH
    pParser->yyidxMax = 0;
#endif
#if YYSTACKDEPTH<=0
    pParser->yystack = NULL;
    pParser->yystksz = 0;
    yyGrowStack(pParser);
#endif
  }
  return pParser;
}

/* The following function deletes the value associated with a
** symbol.  The symbol can be either a terminal or nonterminal.
** "yymajor" is the symbol code, and "yypminor" is a pointer to
** the value.
*/
static void yy_destructor(
  yyParser *yypParser,    /* The parser */
  YYCODETYPE yymajor,     /* Type code for object to destroy */
  YYMINORTYPE *yypminor   /* The object to be destroyed */
){
  ParseBatchGrammarARG_FETCH;
  switch( yymajor ){
    /* Here is inserted the actions which take place when a
    ** terminal or non-terminal is destroyed.  This can happen
    ** when the symbol is popped from the stack during a
    ** reduce or during error processing or when a parser is 
    ** being destroyed before it is finished parsing.
    **
    ** Note: during a reduce, the only symbols destroyed are those
    ** which appear on the RHS of the rule, but which are not used
    ** inside the C code.
    */
      /* TERMINAL Destructor */
    case 1: /* LESS */
    case 2: /* EQUALS */
    case 3: /* PLUS */
    case 4: /* MINUS */
    case 5: /* STAR */
    case 6: /* SLASH */
    case 7: /* PERCENT */
    case 8: /* NEWLINE */
    case 9: /* FUNCTION */
    case 10: /* ID */
    case 11: /* LPAREN */
    case 12: /* RPAREN */
    case 13: /* END */
    case 14: /* COMMA */
    case 15: /* ASSIGN */
    case 16: /* PRINT */
    case 17: /* RETURN */
    case 18: /* IF */
    case 19: /* ELSE */
    case 20: /* WHILE */
    case 21: /* DO */
    case 22: /* NUMBER */
    case 23: /* STRING */
    case 24: /* BOOL */
{

    (void)yypParser;
    (void)yypminor;
    (void)pParse;

}
      break;
    case 26: /* expression */
{
 Destroy((yypminor->yy35)); 
}
      break;
    case 27: /* statement */
    case 28: /* statement_line */
{
 Destroy((yypminor->yy40)); 
}
      break;
    case 29: /* statement_list */
{
 Destroy((yypminor->yy46)); 
}
      break;
    case 30: /* expression_list */
{
 Destroy((yypminor->yy69)); 
}
      break;
    case 31: /* function_declaration */
{
 Destroy((yypminor->yy61));  Destroy((yypminor->yy61)); 
}
      break;
    default:  break;   /* If no destructor action specified: do nothing */
  }
}

/*
** Pop the parser's stack once.
**
** If there is a destructor routine associated with the token which
** is popped from the stack, then call it.
**
** Return the major token number for the symbol popped.
*/
static int yy_pop_parser_stack(yyParser *pParser){
  YYCODETYPE yymajor;
  yyStackEntry *yytos = &pParser->yystack[pParser->yyidx];

  if( pParser->yyidx<0 ) return 0;
#ifndef NDEBUG
  if( yyTraceFILE && pParser->yyidx>=0 ){
    fprintf(yyTraceFILE,"%sPopping %s\n",
      yyTracePrompt,
      yyTokenName[yytos->major]);
  }
#endif
  yymajor = yytos->major;
  yy_destructor(pParser, yymajor, &yytos->minor);
  pParser->yyidx--;
  return yymajor;
}

/* 
** Deallocate and destroy a parser.  Destructors are all called for
** all stack elements before shutting the parser down.
**
** Inputs:
** <ul>
** <li>  A pointer to the parser.  This should be a pointer
**       obtained from ParseBatchGrammarAlloc.
** <li>  A pointer to a function used to reclaim memory obtained
**       from malloc.
** </ul>
*/
void ParseBatchGrammarFree(
  void *p,                    /* The parser to be deleted */
  void (*freeProc)(void*)     /* Function used to reclaim memory */
){
  yyParser *pParser = (yyParser*)p;
  if( pParser==0 ) return;
  while( pParser->yyidx>=0 ) yy_pop_parser_stack(pParser);
#if YYSTACKDEPTH<=0
  free(pParser->yystack);
#endif
  (*freeProc)((void*)pParser);
}

/*
** Return the peak depth of the stack for a parser.
*/
#ifdef YYTRACKMAXSTACKDEPTH
int ParseBatchGrammarStackPeak(void *p){
  yyParser *pParser = (yyParser*)p;
  return pParser->yyidxMax;
}
#endif

/*
** Find the appropriate action for a parser given the terminal
** look-ahead token iLookAhead.
**
** If the look-ahead token is YYNOCODE, then check to see if the action is
** independent of the look-ahead.  If it is, return the action, otherwise
** return YY_NO_ACTION.
*/
static int yy_find_shift_action(
  yyParser *pParser,        /* The parser */
  YYCODETYPE iLookAhead     /* The look-ahead token */
){
  int i;
  int stateno = pParser->yystack[pParser->yyidx].stateno;
 
  if( stateno>YY_SHIFT_COUNT
   || (i = yy_shift_ofst[stateno])==YY_SHIFT_USE_DFLT ){
    return yy_default[stateno];
  }
  assert( iLookAhead!=YYNOCODE );
  i += iLookAhead;
  if( i<0 || i>=YY_ACTTAB_COUNT || yy_lookahead[i]!=iLookAhead ){
    if( iLookAhead>0 ){
#ifdef YYFALLBACK
      YYCODETYPE iFallback;            /* Fallback token */
      if( iLookAhead<sizeof(yyFallback)/sizeof(yyFallback[0])
             && (iFallback = yyFallback[iLookAhead])!=0 ){
#ifndef NDEBUG
        if( yyTraceFILE ){
          fprintf(yyTraceFILE, "%sFALLBACK %s => %s\n",
             yyTracePrompt, yyTokenName[iLookAhead], yyTokenName[iFallback]);
        }
#endif
        return yy_find_shift_action(pParser, iFallback);
      }
#endif
#ifdef YYWILDCARD
      {
        int j = i - iLookAhead + YYWILDCARD;
        if( 
#if YY_SHIFT_MIN+YYWILDCARD<0
          j>=0 &&
#endif
#if YY_SHIFT_MAX+YYWILDCARD>=YY_ACTTAB_COUNT
          j<YY_ACTTAB_COUNT &&
#endif
          yy_lookahead[j]==YYWILDCARD
        ){
#ifndef NDEBUG
          if( yyTraceFILE ){
            fprintf(yyTraceFILE, "%sWILDCARD %s => %s\n",
               yyTracePrompt, yyTokenName[iLookAhead], yyTokenName[YYWILDCARD]);
          }
#endif /* NDEBUG */
          return yy_action[j];
        }
      }
#endif /* YYWILDCARD */
    }
    return yy_default[stateno];
  }else{
    return yy_action[i];
  }
}

/*
** Find the appropriate action for a parser given the non-terminal
** look-ahead token iLookAhead.
**
** If the look-ahead token is YYNOCODE, then check to see if the action is
** independent of the look-ahead.  If it is, return the action, otherwise
** return YY_NO_ACTION.
*/
static int yy_find_reduce_action(
  int stateno,              /* Current state number */
  YYCODETYPE iLookAhead     /* The look-ahead token */
){
  int i;
#ifdef YYERRORSYMBOL
  if( stateno>YY_REDUCE_COUNT ){
    return yy_default[stateno];
  }
#else
  assert( stateno<=YY_REDUCE_COUNT );
#endif
  i = yy_reduce_ofst[stateno];
  assert( i!=YY_REDUCE_USE_DFLT );
  assert( iLookAhead!=YYNOCODE );
  i += iLookAhead;
#ifdef YYERRORSYMBOL
  if( i<0 || i>=YY_ACTTAB_COUNT || yy_lookahead[i]!=iLookAhead ){
    return yy_default[stateno];
  }
#else
  assert( i>=0 && i<YY_ACTTAB_COUNT );
  assert( yy_lookahead[i]==iLookAhead );
#endif
  return yy_action[i];
}

/*
** The following routine is called if the stack overflows.
*/
static void yyStackOverflow(yyParser *yypParser, YYMINORTYPE *yypMinor){
   ParseBatchGrammarARG_FETCH;
   yypParser->yyidx--;
#ifndef NDEBUG
   if( yyTraceFILE ){
     fprintf(yyTraceFILE,"%sStack Overflow!\n",yyTracePrompt);
   }
#endif
   while( yypParser->yyidx>=0 ) yy_pop_parser_stack(yypParser);
   /* Here code is inserted which will execute if the parser
   ** stack every overflows */

    (void)yypMinor; // Silence compiler warnings.
    pParse->OnStackOverflow();
   ParseBatchGrammarARG_STORE; /* Suppress warning about unused %extra_argument var */
}

/*
** Perform a shift action.
*/
static void yy_shift(
  yyParser *yypParser,          /* The parser to be shifted */
  int yyNewState,               /* The new state to shift in */
  int yyMajor,                  /* The major token to shift in */
  YYMINORTYPE *yypMinor         /* Pointer to the minor token to shift in */
){
  yyStackEntry *yytos;
  yypParser->yyidx++;
#ifdef YYTRACKMAXSTACKDEPTH
  if( yypParser->yyidx>yypParser->yyidxMax ){
    yypParser->yyidxMax = yypParser->yyidx;
  }
#endif
#if YYSTACKDEPTH>0 
  if( yypParser->yyidx>=YYSTACKDEPTH ){
    yyStackOverflow(yypParser, yypMinor);
    return;
  }
#else
  if( yypParser->yyidx>=yypParser->yystksz ){
    yyGrowStack(yypParser);
    if( yypParser->yyidx>=yypParser->yystksz ){
      yyStackOverflow(yypParser, yypMinor);
      return;
    }
  }
#endif
  yytos = &yypParser->yystack[yypParser->yyidx];
  yytos->stateno = (YYACTIONTYPE)yyNewState;
  yytos->major = (YYCODETYPE)yyMajor;
  yytos->minor = *yypMinor;
#ifndef NDEBUG
  if( yyTraceFILE && yypParser->yyidx>0 ){
    int i;
    fprintf(yyTraceFILE,"%sShift %d\n",yyTracePrompt,yyNewState);
    fprintf(yyTraceFILE,"%sStack:",yyTracePrompt);
    for(i=1; i<=yypParser->yyidx; i++)
      fprintf(yyTraceFILE," %s",yyTokenName[yypParser->yystack[i].major]);
    fprintf(yyTraceFILE,"\n");
  }
#endif
}

/* The following table contains information about every rule that
** is used during the reduce.
*/
static const struct {
  YYCODETYPE lhs;         /* Symbol on the left-hand side of the rule */
  unsigned char nrhs;     /* Number of right-hand side symbols in the rule */
} yyRuleInfo[] = {
  { 33, 1 },
  { 34, 1 },
  { 34, 2 },
  { 35, 2 },
  { 35, 2 },
  { 36, 1 },
  { 36, 1 },
  { 31, 7 },
  { 31, 8 },
  { 32, 1 },
  { 32, 3 },
  { 29, 1 },
  { 29, 2 },
  { 28, 2 },
  { 28, 2 },
  { 27, 3 },
  { 27, 2 },
  { 27, 2 },
  { 27, 4 },
  { 27, 5 },
  { 27, 8 },
  { 27, 4 },
  { 27, 5 },
  { 27, 5 },
  { 27, 6 },
  { 30, 1 },
  { 30, 3 },
  { 26, 3 },
  { 26, 4 },
  { 26, 3 },
  { 26, 3 },
  { 26, 3 },
  { 26, 3 },
  { 26, 3 },
  { 26, 3 },
  { 26, 3 },
  { 26, 3 },
  { 26, 2 },
  { 26, 2 },
  { 26, 1 },
  { 26, 1 },
  { 26, 1 },
  { 26, 1 },
};

static void yy_accept(yyParser*);  /* Forward Declaration */

/*
** Perform a reduce action and the shift that must immediately
** follow the reduce.
*/
static void yy_reduce(
  yyParser *yypParser,         /* The parser */
  int yyruleno                 /* Number of the rule by which to reduce */
){
  int yygoto;                     /* The next state */
  int yyact;                      /* The next action */
  YYMINORTYPE yygotominor;        /* The LHS of the rule reduced */
  yyStackEntry *yymsp;            /* The top of the parser's stack */
  int yysize;                     /* Amount to pop the stack */
  ParseBatchGrammarARG_FETCH;
  yymsp = &yypParser->yystack[yypParser->yyidx];
#ifndef NDEBUG
  if( yyTraceFILE && yyruleno>=0 
        && yyruleno<(int)(sizeof(yyRuleName)/sizeof(yyRuleName[0])) ){
    fprintf(yyTraceFILE, "%sReduce [%s].\n", yyTracePrompt,
      yyRuleName[yyruleno]);
  }
#endif /* NDEBUG */

  /* Silence complaints from purify about yygotominor being uninitialized
  ** in some cases when it is copied into the stack after the following
  ** switch.  yygotominor is uninitialized when a rule reduces that does
  ** not set the value of its left-hand side nonterminal.  Leaving the
  ** value of the nonterminal uninitialized is utterly harmless as long
  ** as the value is never used.  So really the only thing this code
  ** accomplishes is to quieten purify.  
  **
  ** 2007-01-16:  The wireshark project (www.wireshark.org) reports that
  ** without this code, their parser segfaults.  I'm not sure what there
  ** parser is doing to make this happen.  This is the second bug report
  ** from wireshark this week.  Clearly they are stressing Lemon in ways
  ** that it has not been previously stressed...  (SQLite ticket #2172)
  */
  /*memset(&yygotominor, 0, sizeof(yygotominor));*/
  yygotominor = yyzerominor;


  switch( yyruleno ){
  /* Beginning here are the reduction cases.  A typical example
  ** follows:
  **   case 0:
  **  #line <lineno> <grammarfile>
  **     { ... }           // User supplied code
  **  #line <lineno> <thisfile>
  **     break;
  */
      case 3: /* toplevel_line ::= toplevel_statement NEWLINE */
      case 4: /* toplevel_line ::= error NEWLINE */ yytestcase(yyruleno==4);
{
  yy_destructor(yypParser,8,&yymsp[0].minor);
}
        break;
      case 5: /* toplevel_statement ::= statement */
{
    pParse->AddStatement(Take(yymsp[0].minor.yy40));
}
        break;
      case 6: /* toplevel_statement ::= function_declaration */
{
    pParse->AddFunction(Take(yymsp[0].minor.yy61));
}
        break;
      case 7: /* function_declaration ::= FUNCTION ID LPAREN RPAREN NEWLINE statement_list END */
{
    auto pBody = Take(yymsp[-1].minor.yy46);
    EmplaceAST<CFunctionAST>(yygotominor.yy61, yymsp[-5].minor.yy0.stringId, std::vector<unsigned>(), std::move(*pBody));
  yy_destructor(yypParser,9,&yymsp[-6].minor);
  yy_destructor(yypParser,11,&yymsp[-4].minor);
  yy_destructor(yypParser,12,&yymsp[-3].minor);
  yy_destructor(yypParser,8,&yymsp[-2].minor);
  yy_destructor(yypParser,13,&yymsp[0].minor);
}
        break;
      case 8: /* function_declaration ::= FUNCTION ID LPAREN parameter_list RPAREN NEWLINE statement_list END */
{
    auto pParameters = Take(yymsp[-4].minor.yy65);
    auto pBody = Take(yymsp[-1].minor.yy46);
    EmplaceAST<CFunctionAST>(yygotominor.yy61, yymsp[-6].minor.yy0.stringId, std::move(*pParameters), std::move(*pBody));
  yy_destructor(yypParser,9,&yymsp[-7].minor);
  yy_destructor(yypParser,11,&yymsp[-5].minor);
  yy_destructor(yypParser,12,&yymsp[-3].minor);
  yy_destructor(yypParser,8,&yymsp[-2].minor);
  yy_destructor(yypParser,13,&yymsp[0].minor);
}
        break;
      case 9: /* parameter_list ::= ID */
{
    auto list = Make<NamesList>();
    list->emplace_back(yymsp[0].minor.yy0.stringId);
    yygotominor.yy65 = list.release();
}
        break;
      case 10: /* parameter_list ::= parameter_list COMMA ID */
{
    auto pList = Take(yymsp[-2].minor.yy65);
    pList->emplace_back(yymsp[0].minor.yy0.stringId);
    yygotominor.yy65 = pList.release();
  yy_destructor(yypParser,14,&yymsp[-1].minor);
}
        break;
      case 11: /* statement_list ::= statement_line */
{
    CreateList(yygotominor.yy46, yymsp[0].minor.yy40);
}
        break;
      case 12: /* statement_list ::= statement_list statement_line */
{
    ConcatList(yygotominor.yy46, yymsp[-1].minor.yy46, yymsp[0].minor.yy40);
}
        break;
      case 13: /* statement_line ::= statement NEWLINE */
{
    MovePointer(yymsp[-1].minor.yy40, yygotominor.yy40);
  yy_destructor(yypParser,8,&yymsp[0].minor);
}
        break;
      case 14: /* statement_line ::= error NEWLINE */
{
    yygotominor.yy40 = nullptr;
  yy_destructor(yypParser,8,&yymsp[0].minor);
}
        break;
      case 15: /* statement ::= ID ASSIGN expression */
{
    EmplaceAST<CAssignAST>(yygotominor.yy40, yymsp[-2].minor.yy0.stringId, Take(yymsp[0].minor.yy35));
  yy_destructor(yypParser,15,&yymsp[-1].minor);
}
        break;
      case 16: /* statement ::= PRINT expression */
{
    EmplaceAST<CPrintAST>(yygotominor.yy40, Take(yymsp[0].minor.yy35));
  yy_destructor(yypParser,16,&yymsp[-1].minor);
}
        break;
      case 17: /* statement ::= RETURN expression */
{
    EmplaceAST<CReturnAST>(yygotominor.yy40, Take(yymsp[0].minor.yy35));
  yy_destructor(yypParser,17,&yymsp[-1].minor);
}
        break;
      case 18: /* statement ::= IF expression NEWLINE END */
{
    EmplaceAST<CIfAst>(yygotominor.yy40, Take(yymsp[-2].minor.yy35));
  yy_destructor(yypParser,18,&yymsp[-3].minor);
  yy_destructor(yypParser,8,&yymsp[-1].minor);
  yy_destructor(yypParser,13,&yymsp[0].minor);
}
        break;
      case 19: /* statement ::= IF expression NEWLINE statement_list END */
{
    auto pThenBody = Take(yymsp[-1].minor.yy46);
    EmplaceAST<CIfAst>(yygotominor.yy40, Take(yymsp[-3].minor.yy35), std::move(*pThenBody));
  yy_destructor(yypParser,18,&yymsp[-4].minor);
  yy_destructor(yypParser,8,&yymsp[-2].minor);
  yy_destructor(yypParser,13,&yymsp[0].minor);
}
        break;
      case 20: /* statement ::= IF expression NEWLINE statement_list ELSE NEWLINE statement_list END */
{
    auto pThenBody = Take(yymsp[-4].minor.yy46);
    auto pElseBody = Take(yymsp[-1].minor.yy46);
    EmplaceAST<CIfAst>(yygotominor.yy40, Take(yymsp[-6].minor.yy35), std::move(*pThenBody), std::move(*pElseBody));
  yy_destructor(yypParser,18,&yymsp[-7].minor);
  yy_destructor(yypParser,8,&yymsp[-5].minor);
  yy_destructor(yypParser,19,&yymsp[-3].minor);
  yy_destructor(yypParser,8,&yymsp[-2].minor);
  yy_destructor(yypParser,13,&yymsp[0].minor);
}
        break;
      case 21: /* statement ::= WHILE expression NEWLINE END */
{
    EmplaceAST<CWhileAst>(yygotominor.yy40, Take(yymsp[-2].minor.yy35));
  yy_destructor(yypParser,20,&yymsp[-3].minor);
  yy_destructor(yypParser,8,&yymsp[-1].minor);
  yy_destructor(yypParser,13,&yymsp[0].minor);
}
        break;
      case 22: /* statement ::= WHILE expression NEWLINE statement_list END */
{
    auto pBody = Take(yymsp[-1].minor.yy46);
    EmplaceAST<CWhileAst>(yygotominor.yy40, Take(yymsp[-3].minor.yy35), std::move(*pBody));
  yy_destructor(yypParser,20,&yymsp[-4].minor);
  yy_destructor(yypParser,8,&yymsp[-2].minor);
  yy_destructor(yypParser,13,&yymsp[0].minor);
}
        break;
      case 23: /* statement ::= DO NEWLINE WHILE expression END */
{
    EmplaceAST<CRepeatAst>(yygotominor.yy40, Take(yymsp[-1].minor.yy35));
  yy_destructor(yypParser,21,&yymsp[-4].minor);
  yy_destructor(yypParser,8,&yymsp[-3].minor);
  yy_destructor(yypParser,20,&yymsp[-2].minor);
  yy_destructor(yypParser,13,&yymsp[0].minor);
}
        break;
      case 24: /* statement ::= DO NEWLINE statement_list WHILE expression END */
{
    auto pBody = Take(yymsp[-3].minor.yy46);
    EmplaceAST<CRepeatAst>(yygotominor.yy40, Take(yymsp[-1].minor.yy35), std::move(*pBody));
  yy_destructor(yypParser,21,&yymsp[-5].minor);
  yy_destructor(yypParser,8,&yymsp[-4].minor);
  yy_destructor(yypParser,20,&yymsp[-2].minor);
  yy_destructor(yypParser,13,&yymsp[0].minor);
}
        break;
      case 25: /* expression_list ::= expression */
{
    CreateList(yygotominor.yy69, yymsp[0].minor.yy35);
}
        break;
      case 26: /* expression_list ::= expression_list COMMA expression */
{
    ConcatList(yygotominor.yy69, yymsp[-2].minor.yy69, yymsp[0].minor.yy35);
  yy_destructor(yypParser,14,&yymsp[-1].minor);
}
        break;
      case 27: /* expression ::= ID LPAREN RPAREN */
{
    EmplaceAST<CCallAST>(yygotominor.yy35, yymsp[-2].minor.yy0.stringId, ExpressionList());
  yy_destructor(yypParser,11,&yymsp[-1].minor);
  yy_destructor(yypParser,12,&yymsp[0].minor);
}
        break;
      case 28: /* expression ::= ID LPAREN expression_list RPAREN */
{
    auto pList = Take(yymsp[-1].minor.yy69);
    EmplaceAST<CCallAST>(yygotominor.yy35, yymsp[-3].minor.yy0.stringId, std::move(*pList));
  yy_destructor(yypParser,11,&yymsp[-2].minor);
  yy_destructor(yypParser,12,&yymsp[0].minor);
}
        break;
      case 29: /* expression ::= LPAREN expression RPAREN */
{
    MovePointer(yymsp[-1].minor.yy35, yygotominor.yy35);
  yy_destructor(yypParser,11,&yymsp[-2].minor);
  yy_destructor(yypParser,12,&yymsp[0].minor);
}
        break;
      case 30: /* expression ::= expression LESS expression */
{
    EmplaceAST<CBinaryExpressionAST>(yygotominor.yy35, Take(yymsp[-2].minor.yy35), BinaryOperation::Less, Take(yymsp[0].minor.yy35));
  yy_destructor(yypParser,1,&yymsp[-1].minor);
}
        break;
      case 31: /* expression ::= expression EQUALS expression */
{
    EmplaceAST<CBinaryExpressionAST>(yygotominor.yy35, Take(yymsp[-2].minor.yy35), BinaryOperation::Equals, Take(yymsp[0].minor.yy35));
  yy_destructor(yypParser,2,&yymsp[-1].minor);
}
        break;
      case 32: /* expression ::= expression PLUS expression */
{
    EmplaceAST<CBinaryExpressionAST>(yygotominor.yy35, Take(yymsp[-2].minor.yy35), BinaryOperation::Add, Take(yymsp[0].minor.yy35));
  yy_destructor(yypParser,3,&yymsp[-1].minor);
}
        break;
      case 33: /* expression ::= expression MINUS expression */
{
    EmplaceAST<CBinaryExpressionAST>(yygotominor.yy35, Take(yymsp[-2].minor.yy35), BinaryOperation::Substract, Take(yymsp[0].minor.yy35));
  yy_destructor(yypParser,4,&yymsp[-1].minor);
}
        break;
      case 34: /* expression ::= expression STAR expression */
{
    EmplaceAST<CBinaryExpressionAST>(yygotominor.yy35, Take(yymsp[-2].minor.yy35), BinaryOperation::Multiply, Take(yymsp[0].minor.yy35));
  yy_destructor(yypParser,5,&yymsp[-1].minor);
}
        break;
      case 35: /* expression ::= expression SLASH expression */
{
    EmplaceAST<CBinaryExpressionAST>(yygotominor.yy35, Take(yymsp[-2].minor.yy35), BinaryOperation::Divide, Take(yymsp[0].minor.yy35));
  yy_destructor(yypParser,6,&yymsp[-1].minor);
}
        break;
      case 36: /* expression ::= expression PERCENT expression */
{
    EmplaceAST<CBinaryExpressionAST>(yygotominor.yy35, Take(yymsp[-2].minor.yy35), BinaryOperation::Modulo, Take(yymsp[0].minor.yy35));
  yy_destructor(yypParser,7,&yymsp[-1].minor);
}
        break;
      case 37: /* expression ::= PLUS expression */
{
    EmplaceAST<CUnaryExpressionAST>(yygotominor.yy35, UnaryOperation::Plus, Take(yymsp[0].minor.yy35));
  yy_destructor(yypParser,3,&yymsp[-1].minor);
}
        break;
      case 38: /* expression ::= MINUS expression */
{
    EmplaceAST<CUnaryExpressionAST>(yygotominor.yy35, UnaryOperation::Minus, Take(yymsp[0].minor.yy35));
  yy_destructor(yypParser,4,&yymsp[-1].minor);
}
        break;
      case 39: /* expression ::= NUMBER */
{
    EmplaceAST<CLiteralAST>(yygotominor.yy35, CValue::FromDouble(yymsp[0].minor.yy0.value));
}
        break;
      case 40: /* expression ::= STRING */
{
    EmplaceAST<CLiteralAST>(yygotominor.yy35, pParse->GetStringLiteral(yymsp[0].minor.yy0.stringId));
}
        break;
      case 41: /* expression ::= BOOL */
{
    EmplaceAST<CLiteralAST>(yygotominor.yy35, CValue::FromBoolean(yymsp[0].minor.yy0.boolValue));
}
        break;
      case 42: /* expression ::= ID */
{
    EmplaceAST<CVariableRefAST>(yygotominor.yy35, yymsp[0].minor.yy0.stringId);
}
        break;
      default:
      /* (0) translation_unit ::= toplevel_list */ yytestcase(yyruleno==0);
      /* (1) toplevel_list ::= toplevel_line */ yytestcase(yyruleno==1);
      /* (2) toplevel_list ::= toplevel_list toplevel_line */ yytestcase(yyruleno==2);
        break;
  };
  yygoto = yyRuleInfo[yyruleno].lhs;
  yysize = yyRuleInfo[yyruleno].nrhs;
  yypParser->yyidx -= yysize;
  yyact = yy_find_reduce_action(yymsp[-yysize].stateno,(YYCODETYPE)yygoto);
  if( yyact < YYNSTATE ){
#ifdef NDEBUG
    /* If we are not debugging and the reduce action popped at least
    ** one element off the stack, then we can push the new element back
    ** onto the stack here, and skip the stack overflow test in yy_shift().
    ** That gives a significant speed improvement. */
    if( yysize ){
      yypParser->yyidx++;
      yymsp -= yysize-1;
      yymsp->stateno = (YYACTIONTYPE)yyact;
      yymsp->major = (YYCODETYPE)yygoto;
      yymsp->minor = yygotominor;
    }else
#endif
    {
      yy_shift(yypParser,yyact,yygoto,&yygotominor);
    }
  }else{
    assert( yyact == YYNSTATE + YYNRULE + 1 );
    yy_accept(yypParser);
  }
}

/*
** The following code executes when the parse fails
*/
#ifndef YYNOERRORRECOVERY
static void yy_parse_failed(
  yyParser *yypParser           /* The parser */
){
  ParseBatchGrammarARG_FETCH;
#ifndef NDEBUG
  if( yyTraceFILE ){
    fprintf(yyTraceFILE,"%sFail!\n",yyTracePrompt);
  }
#endif
  while( yypParser->yyidx>=0 ) yy_pop_parser_stack(yypParser);
  /* Here code is inserted which will be executed whenever the
  ** parser fails */

    pParse->OnFatalError();
  ParseBatchGrammarARG_STORE; /* Suppress warning about unused %extra_argument variable */
}
#endif /* YYNOERRORRECOVERY */

/*
** The following code executes when a syntax error first occurs.
*/
static void yy_syntax_error(
  yyParser *yypParser,           /* The parser */
  int yymajor,                   /* The major type of the error token */
  YYMINORTYPE yyminor            /* The minor type of the error token */
){
  ParseBatchGrammarARG_FETCH;
#define TOKEN (yyminor.yy0)

    (void)yymajor;  // Silence compiler warnings.
    pParse->OnError(TOKEN); // TOKEN has type defined in '%token_type'
  ParseBatchGrammarARG_STORE; /* Suppress warning about unused %extra_argument variable */
}

/*
** The following is executed when the parser accepts
*/
static void yy_accept(
  yyParser *yypParser           /* The parser */
){
  ParseBatchGrammarARG_FETCH;
#ifndef NDEBUG
  if( yyTraceFILE ){
    fprintf(yyTraceFILE,"%sAccept!\n",yyTracePrompt);
  }
#endif
  while( yypParser->yyidx>=0 ) yy_pop_parser_stack(yypParser);
  /* Here code is inserted which will be executed whenever the
  ** parser accepts */
  ParseBatchGrammarARG_STORE; /* Suppress warning about unused %extra_argument variable */
}

/* The main parser program.
** The first argument is a pointer to a structure obtained from
** "ParseBatchGrammarAlloc" which describes the current state of the parser.
** The second argument is the major token number.  The third is
** the minor token.  The fourth optional argument is whatever the
** user wants (and specified in the grammar) and is available for
** use by the action routines.
**
** Inputs:
** <ul>
** <li> A pointer to the parser (an opaque structure.)
** <li> The major token number.
** <li> The minor token number.
** <li> An option argument of a grammar-specified type.
** </ul>
**
** Outputs:
** None.
*/
void ParseBatchGrammar(
  void *yyp,                   /* The parser */
  int yymajor,                 /* The major token code number */
  ParseBatchGrammarTOKENTYPE yyminor       /* The value for the token */
  ParseBatchGrammarARG_PDECL               /* Optional %extra_argument parameter */
){
  YYMINORTYPE yyminorunion;
  int yyact;            /* The parser action. */
  int yyendofinput;     /* True if we are at the end of input */
#ifdef YYERRORSYMBOL
  int yyerrorhit = 0;   /* True if yymajor has invoked an error */
#endif
  yyParser *yypParser;  /* The parser */

  /* (re)initialize the parser, if necessary */
  yypParser = (yyParser*)yyp;
  if( yypParser->yyidx<0 ){
#if YYSTACKDEPTH<=0
    if( yypParser->yystksz <=0 ){
      /*memset(&yyminorunion, 0, sizeof(yyminorunion));*/
      yyminorunion = yyzerominor;
      yyStackOverflow(yypParser, &yyminorunion);
      return;
    }
#endif
    yypParser->yyidx = 0;
    yypParser->yyerrcnt = -1;
    yypParser->yystack[0].stateno = 0;
    yypParser->yystack[0].major = 0;
  }
  yyminorunion.yy0 = yyminor;
  yyendofinput = (yymajor==0);
  ParseBatchGrammarARG_STORE;

#ifndef NDEBUG
  if( yyTraceFILE ){
    fprintf(yyTraceFILE,"%sInput %s\n",yyTracePrompt,yyTokenName[yymajor]);
  }
#endif

  do{
    yyact = yy_find_shift_action(yypParser,(YYCODETYPE)yymajor);
    if( yyact<YYNSTATE ){
      assert( !yyendofinput );  /* Impossible to shift the $ token */
      yy_shift(yypParser,yyact,yymajor,&yyminorunion);
      yypParser->yyerrcnt--;
      yymajor = YYNOCODE;
    }else if( yyact < YYNSTATE + YYNRULE ){
      yy_reduce(yypParser,yyact-YYNSTATE);
    }else{
      assert( yyact == YY_ERROR_ACTION );
#ifdef YYERRORSYMBOL
      int yymx;
#endif
#ifndef NDEBUG
      if( yyTraceFILE ){
        fprintf(yyTraceFILE,"%sSyntax Error!\n",yyTracePrompt);
      }
#endif
#ifdef YYERRORSYMBOL
      /* A syntax error has occurred.
      ** The response to an error depends upon whether or not the
      ** grammar defines an error token "ERROR".  
      **
      ** This is what we do if the grammar does define ERROR:
      **
      **  * Call the %syntax_error function.
      **
      **  * Begin popping the stack until we enter a state where
      **    it is legal to shift the error symbol, then shift
      **    the error symbol.
      **
      **  * Set the error count to three.
      **
      **  * Begin accepting and shifting new tokens.  No new error
      **    processing will occur until three tokens have been
      **    shifted successfully.
      **
      */
      if( yypParser->yyerrcnt<0 ){
        yy_syntax_error(yypParser,yymajor,yyminorunion);
      }
      yymx = yypParser->yystack[yypParser->yyidx].major;
      if( yymx==YYERRORSYMBOL || yyerrorhit ){
#ifndef NDEBUG
        if( yyTraceFILE ){
          fprintf(yyTraceFILE,"%sDiscard input token %s\n",
             yyTracePrompt,yyTokenName[yymajor]);
        }
#endif
        yy_destructor(yypParser, (YYCODETYPE)yymajor,&yyminorunion);
        yymajor = YYNOCODE;
      }else{
         while(
          yypParser->yyidx >= 0 &&
          yymx != YYERRORSYMBOL &&
          (yyact = yy_find_reduce_action(
                        yypParser->yystack[yypParser->yyidx].stateno,
                        YYERRORSYMBOL)) >= YYNSTATE
        ){
          yy_pop_parser_stack(yypParser);
        }
        if( yypParser->yyidx < 0 || yymajor==0 ){
          yy_destructor(yypParser,(YYCODETYPE)yymajor,&yyminorunion);
          yy_parse_failed(yypParser);
          yymajor = YYNOCODE;
        }else if( yymx!=YYERRORSYMBOL ){
          YYMINORTYPE u2;
          u2.YYERRSYMDT = 0;
          yy_shift(yypParser,yyact,YYERRORSYMBOL,&u2);
        }
      }
      yypParser->yyerrcnt = 3;
      yyerrorhit = 1;
#elif defined(YYNOERRORRECOVERY)
      /* If the YYNOERRORRECOVERY macro is defined, then do not attempt to
      ** do any kind of error recovery.  Instead, simply invoke the syntax
      ** error routine and continue going as if nothing had happened.
      **
      ** Applications can set this macro (for example inside %include) if
      ** they intend to abandon the parse upon the first syntax error seen.
      */
      yy_syntax_error(yypParser,yymajor,yyminorunion);
      yy_destructor(yypParser,(YYCODETYPE)yymajor,&yyminorunion);
      yymajor = YYNOCODE;
      
#else  /* YYERRORSYMBOL is not defined */
      /* This is what we do if the grammar does not define ERROR:
      **
      **  * Report an error message, and throw away the input token.
      **
      **  * If the input token is $, then fail the parse.
      **
      ** As before, subsequent error messages are suppressed until
      ** three input tokens have been successfully shifted.
      */
      if( yypParser->yyerrcnt<=0 ){
        yy_syntax_error(yypParser,yymajor,yyminorunion);
      }
      yypParser->yyerrcnt = 3;
      yy_destructor(yypParser,(YYCODETYPE)yymajor,&yyminorunion);
      if( yyendofinput ){
        yy_parse_failed(yypParser);
      }
      yymajor = YYNOCODE;
#endif
    }
  }while( yymajor!=YYNOCODE && yypParser->yyidx>=0 );
  return;
}
