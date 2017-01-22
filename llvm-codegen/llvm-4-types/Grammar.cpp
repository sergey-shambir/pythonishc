/* Driver template for the LEMON parser generator.
** The author disclaims copyright to this source code.
*/
/* First off, code is included that follows the "include" declaration
** in the input grammar file. */
#include <stdio.h>

// Generated function: void ParseGrammar(void*, int, SToken, CParser*);

#include "Parser_private.h"

using namespace parser_private;

// Indicate that ParseGrammarFree() will never be called with a nullptr.
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
**    ParseGrammarTOKENTYPE     is the data type used for minor tokens given 
**                       directly to the parser from the tokenizer.
**    YYMINORTYPE        is the data type used for all minor tokens.
**                       This is typically a union of many types, one of
**                       which is ParseGrammarTOKENTYPE.  The entry in the union
**                       for base tokens is called "yy0".
**    YYSTACKDEPTH       is the maximum depth of the parser's stack.  If
**                       zero the stack is dynamically sized using realloc()
**    ParseGrammarARG_SDECL     A static variable declaration for the %extra_argument
**    ParseGrammarARG_PDECL     A parameter declaration for the %extra_argument
**    ParseGrammarARG_STORE     Code to store %extra_argument into yypParser
**    ParseGrammarARG_FETCH     Code to extract %extra_argument from yypParser
**    YYNSTATE           the combined number of states.
**    YYNRULE            the number of rules in the grammar
**    YYERRORSYMBOL      is the code number of the error symbol.  If not
**                       defined, then do no error processing.
*/
#define YYCODETYPE unsigned char
#define YYNOCODE 44
#define YYACTIONTYPE unsigned char
#define ParseGrammarTOKENTYPE SToken
typedef union {
  int yyinit;
  ParseGrammarTOKENTYPE yy0;
  StatementListPtr yy2;
  ParameterDeclPtr yy6;
  ParameterDeclListPtr yy10;
  StatementPtr yy16;
  FunctionPtr yy55;
  ExpressionListPtr yy63;
  int yy64;
  ExpressionPtr yy65;
  int yy87;
} YYMINORTYPE;
#ifndef YYSTACKDEPTH
#define YYSTACKDEPTH 100
#endif
#define ParseGrammarARG_SDECL CParser *pParse;
#define ParseGrammarARG_PDECL ,CParser *pParse
#define ParseGrammarARG_FETCH CParser *pParse = yypParser->pParse
#define ParseGrammarARG_STORE yypParser->pParse = pParse
#define YYNSTATE 99
#define YYNRULE 48
#define YYERRORSYMBOL 28
#define YYERRSYMDT yy87
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
#define YY_ACTTAB_COUNT (222)
static const YYACTIONTYPE yy_action[] = {
 /*     0 */    30,   29,   27,   26,   25,   24,   23,    1,   27,   26,
 /*    10 */    25,   24,   23,   73,   97,   30,   29,   27,   26,   25,
 /*    20 */    24,   23,    1,   58,   96,   59,   77,   10,   72,    5,
 /*    30 */    30,   29,   27,   26,   25,   24,   23,   30,   29,   27,
 /*    40 */    26,   25,   24,   23,    2,   86,   30,   29,   27,   26,
 /*    50 */    25,   24,   23,    1,   22,   21,   58,   35,   59,   77,
 /*    60 */     9,   44,   57,   76,   56,   53,   28,   78,   19,   18,
 /*    70 */    17,   55,   16,   54,   52,   65,   82,   81,   80,   30,
 /*    80 */    29,   27,   26,   25,   24,   23,   22,   21,   58,   88,
 /*    90 */    59,   77,   11,   57,   90,   31,   56,   87,   28,   19,
 /*   100 */    18,   17,    4,   16,   54,   25,   24,   23,   82,   81,
 /*   110 */    80,   12,   57,   75,   58,   62,   59,   89,   19,   18,
 /*   120 */    17,   94,   16,   54,   57,   74,  148,    6,   64,   63,
 /*   130 */    19,   18,   17,   99,   16,   54,   57,   70,   93,   92,
 /*   140 */    91,   95,   19,   18,   17,   61,   16,   54,   57,   71,
 /*   150 */    79,   20,   68,   34,   19,   18,   17,   57,   16,   54,
 /*   160 */    62,    3,   33,   19,   18,   17,   94,   15,   54,   57,
 /*   170 */    13,   32,  149,   98,   63,   19,   18,   17,   57,   16,
 /*   180 */    54,   60,   67,   45,   19,   18,   17,   32,   14,   54,
 /*   190 */    69,   46,   58,  149,   59,   77,    7,   58,   95,   59,
 /*   200 */    77,    8,   61,  149,   66,   47,   40,   51,   50,   85,
 /*   210 */    84,   83,   49,   48,   43,   42,   41,   39,   38,  149,
 /*   220 */    37,   36,
};
static const YYCODETYPE yy_lookahead[] = {
 /*     0 */     1,    2,    3,    4,    5,    6,    7,    8,    3,    4,
 /*    10 */     5,    6,    7,   14,    8,    1,    2,    3,    4,    5,
 /*    20 */     6,    7,    8,   28,    8,   30,   31,   32,   14,    8,
 /*    30 */     1,    2,    3,    4,    5,    6,    7,    1,    2,    3,
 /*    40 */     4,    5,    6,    7,    8,   16,    1,    2,    3,    4,
 /*    50 */     5,    6,    7,    8,    3,    4,   28,   13,   30,   31,
 /*    60 */    32,   29,   13,   14,   13,   33,   15,   16,   19,   20,
 /*    70 */    21,   22,   23,   24,   36,   37,   25,   26,   27,    1,
 /*    80 */     2,    3,    4,    5,    6,    7,    3,    4,   28,    8,
 /*    90 */    30,   31,   32,   13,   14,   18,   13,    8,   15,   19,
 /*   100 */    20,   21,    8,   23,   24,    5,    6,    7,   25,   26,
 /*   110 */    27,   15,   13,   14,   28,   28,   30,   31,   19,   20,
 /*   120 */    21,   34,   23,   24,   13,   14,   39,   40,   41,   42,
 /*   130 */    19,   20,   21,    0,   23,   24,   13,   14,    9,   10,
 /*   140 */    11,    8,   19,   20,   21,   12,   23,   24,   13,   14,
 /*   150 */    16,   17,   16,   17,   19,   20,   21,   13,   23,   24,
 /*   160 */    28,    8,   35,   19,   20,   21,   34,   23,   24,   13,
 /*   170 */    15,   13,   43,   41,   42,   19,   20,   21,   13,   23,
 /*   180 */    24,   38,   37,   29,   19,   20,   21,   13,   23,   24,
 /*   190 */    16,   29,   28,   43,   30,   31,   32,   28,    8,   30,
 /*   200 */    31,   32,   12,   43,   38,   29,   29,   29,   29,   29,
 /*   210 */    29,   29,   29,   29,   29,   29,   29,   29,   29,   43,
 /*   220 */    29,   29,
};
#define YY_SHIFT_USE_DFLT (-2)
#define YY_SHIFT_COUNT (63)
#define YY_SHIFT_MIN   (-1)
#define YY_SHIFT_MAX   (190)
static const short yy_shift_ofst[] = {
 /*     0 */   190,  135,  123,  165,  156,  156,  133,   49,  111,   99,
 /*    10 */    80,  144,   51,  174,   83,   83,   83,   83,   83,   83,
 /*    20 */    83,   83,   83,   83,   83,   83,   83,   83,   83,   83,
 /*    30 */    83,   83,  129,  129,  158,  155,   14,   -1,   45,   36,
 /*    40 */    29,   78,   78,   78,   78,   78,    5,    5,  100,  100,
 /*    50 */   100,  100,  136,  134,  153,   94,   96,   77,   89,   81,
 /*    60 */    21,   44,   16,    6,
};
#define YY_REDUCE_USE_DFLT (-6)
#define YY_REDUCE_COUNT (35)
#define YY_REDUCE_MIN   (-5)
#define YY_REDUCE_MAX   (192)
static const short yy_reduce_ofst[] = {
 /*     0 */    87,  169,  164,   60,   28,   -5,  132,   86,   86,   86,
 /*    10 */    86,   86,   32,   38,  192,  191,  189,  188,  187,  186,
 /*    20 */   185,  184,  183,  182,  181,  180,  179,  178,  177,  176,
 /*    30 */   162,  154,  166,  143,  145,  127,
};
static const YYACTIONTYPE yy_default[] = {
 /*     0 */   147,  147,  147,  147,  147,  147,  147,  147,  147,  147,
 /*    10 */   147,  147,  147,  147,  147,  147,  147,  147,  147,  147,
 /*    20 */   147,  147,  147,  147,  147,  147,  147,  147,  147,  147,
 /*    30 */   147,  147,  147,  147,  147,  147,  147,  147,  147,  147,
 /*    40 */   147,  121,  120,  130,  129,  119,  134,  135,  142,  141,
 /*    50 */   137,  136,  147,  147,  147,  147,  146,  147,  147,  147,
 /*    60 */   147,  147,  147,  147,  100,  112,  114,  113,  111,  110,
 /*    70 */   122,  125,  127,  128,  126,  124,  123,  115,  131,  132,
 /*    80 */   145,  144,  143,  140,  139,  138,  133,  118,  117,  116,
 /*    90 */   109,  108,  107,  106,  105,  104,  103,  102,  101,
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
  ParseGrammarARG_SDECL                /* A place to hold %extra_argument */
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
void ParseGrammarTrace(FILE *TraceFILE, char *zTracePrompt){
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
  "NEWLINE",       "STRING_TYPE",   "NUMBER_TYPE",   "BOOLEAN_TYPE",
  "FUNCTION",      "ID",            "END",           "LPAREN",      
  "RPAREN",        "COMMA",         "ASSIGN",        "PRINT",       
  "RETURN",        "IF",            "ELSE",          "WHILE",       
  "DO",            "NUMBER_VALUE",  "STRING_VALUE",  "BOOLEAN_VALUE",
  "error",         "expression",    "statement",     "statement_line",
  "statement_list",  "expression_list",  "function_declaration",  "parenthesis_parameter_list",
  "parameter_list",  "parameter_decl",  "type_reference",  "translation_unit",
  "toplevel_list",  "toplevel_line",  "toplevel_statement",
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
 /*   5 */ "toplevel_line ::= NEWLINE",
 /*   6 */ "toplevel_statement ::= function_declaration",
 /*   7 */ "type_reference ::= STRING_TYPE",
 /*   8 */ "type_reference ::= NUMBER_TYPE",
 /*   9 */ "type_reference ::= BOOLEAN_TYPE",
 /*  10 */ "function_declaration ::= FUNCTION ID parenthesis_parameter_list type_reference NEWLINE statement_list END",
 /*  11 */ "parenthesis_parameter_list ::= LPAREN RPAREN",
 /*  12 */ "parenthesis_parameter_list ::= LPAREN parameter_list RPAREN",
 /*  13 */ "parameter_list ::= parameter_decl",
 /*  14 */ "parameter_list ::= parameter_list COMMA parameter_decl",
 /*  15 */ "parameter_decl ::= ID type_reference",
 /*  16 */ "statement_list ::= statement_line",
 /*  17 */ "statement_list ::= statement_list statement_line",
 /*  18 */ "statement_line ::= statement NEWLINE",
 /*  19 */ "statement_line ::= error NEWLINE",
 /*  20 */ "statement ::= ID ASSIGN expression",
 /*  21 */ "statement ::= PRINT expression",
 /*  22 */ "statement ::= RETURN expression",
 /*  23 */ "statement ::= IF expression NEWLINE END",
 /*  24 */ "statement ::= IF expression NEWLINE statement_list END",
 /*  25 */ "statement ::= IF expression NEWLINE statement_list ELSE NEWLINE statement_list END",
 /*  26 */ "statement ::= WHILE expression NEWLINE END",
 /*  27 */ "statement ::= WHILE expression NEWLINE statement_list END",
 /*  28 */ "statement ::= DO NEWLINE WHILE expression END",
 /*  29 */ "statement ::= DO NEWLINE statement_list WHILE expression END",
 /*  30 */ "expression_list ::= expression",
 /*  31 */ "expression_list ::= expression_list COMMA expression",
 /*  32 */ "expression ::= ID LPAREN RPAREN",
 /*  33 */ "expression ::= ID LPAREN expression_list RPAREN",
 /*  34 */ "expression ::= LPAREN expression RPAREN",
 /*  35 */ "expression ::= expression LESS expression",
 /*  36 */ "expression ::= expression EQUALS expression",
 /*  37 */ "expression ::= expression PLUS expression",
 /*  38 */ "expression ::= expression MINUS expression",
 /*  39 */ "expression ::= expression STAR expression",
 /*  40 */ "expression ::= expression SLASH expression",
 /*  41 */ "expression ::= expression PERCENT expression",
 /*  42 */ "expression ::= PLUS expression",
 /*  43 */ "expression ::= MINUS expression",
 /*  44 */ "expression ::= NUMBER_VALUE",
 /*  45 */ "expression ::= STRING_VALUE",
 /*  46 */ "expression ::= BOOLEAN_VALUE",
 /*  47 */ "expression ::= ID",
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
** to ParseGrammar and ParseGrammarFree.
*/
void *ParseGrammarAlloc(void *(*mallocProc)(size_t)){
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
  ParseGrammarARG_FETCH;
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
    case 9: /* STRING_TYPE */
    case 10: /* NUMBER_TYPE */
    case 11: /* BOOLEAN_TYPE */
    case 12: /* FUNCTION */
    case 13: /* ID */
    case 14: /* END */
    case 15: /* LPAREN */
    case 16: /* RPAREN */
    case 17: /* COMMA */
    case 18: /* ASSIGN */
    case 19: /* PRINT */
    case 20: /* RETURN */
    case 21: /* IF */
    case 22: /* ELSE */
    case 23: /* WHILE */
    case 24: /* DO */
    case 25: /* NUMBER_VALUE */
    case 26: /* STRING_VALUE */
    case 27: /* BOOLEAN_VALUE */
{

    (void)yypParser;
    (void)yypminor;
    (void)pParse;

}
      break;
    case 29: /* expression */
{
 Destroy((yypminor->yy65)); 
}
      break;
    case 30: /* statement */
    case 31: /* statement_line */
{
 Destroy((yypminor->yy16)); 
}
      break;
    case 32: /* statement_list */
{
 Destroy((yypminor->yy2)); 
}
      break;
    case 33: /* expression_list */
{
 Destroy((yypminor->yy63)); 
}
      break;
    case 34: /* function_declaration */
{
 Destroy((yypminor->yy55)); 
}
      break;
    case 35: /* parenthesis_parameter_list */
    case 36: /* parameter_list */
{
 Destroy((yypminor->yy10)); 
}
      break;
    case 37: /* parameter_decl */
{
 Destroy((yypminor->yy6)); 
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
**       obtained from ParseGrammarAlloc.
** <li>  A pointer to a function used to reclaim memory obtained
**       from malloc.
** </ul>
*/
void ParseGrammarFree(
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
int ParseGrammarStackPeak(void *p){
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
   ParseGrammarARG_FETCH;
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
   ParseGrammarARG_STORE; /* Suppress warning about unused %extra_argument var */
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
  { 39, 1 },
  { 40, 1 },
  { 40, 2 },
  { 41, 2 },
  { 41, 2 },
  { 41, 1 },
  { 42, 1 },
  { 38, 1 },
  { 38, 1 },
  { 38, 1 },
  { 34, 7 },
  { 35, 2 },
  { 35, 3 },
  { 36, 1 },
  { 36, 3 },
  { 37, 2 },
  { 32, 1 },
  { 32, 2 },
  { 31, 2 },
  { 31, 2 },
  { 30, 3 },
  { 30, 2 },
  { 30, 2 },
  { 30, 4 },
  { 30, 5 },
  { 30, 8 },
  { 30, 4 },
  { 30, 5 },
  { 30, 5 },
  { 30, 6 },
  { 33, 1 },
  { 33, 3 },
  { 29, 3 },
  { 29, 4 },
  { 29, 3 },
  { 29, 3 },
  { 29, 3 },
  { 29, 3 },
  { 29, 3 },
  { 29, 3 },
  { 29, 3 },
  { 29, 3 },
  { 29, 2 },
  { 29, 2 },
  { 29, 1 },
  { 29, 1 },
  { 29, 1 },
  { 29, 1 },
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
  ParseGrammarARG_FETCH;
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
      case 5: /* toplevel_line ::= NEWLINE */ yytestcase(yyruleno==5);
{
  yy_destructor(yypParser,8,&yymsp[0].minor);
}
        break;
      case 6: /* toplevel_statement ::= function_declaration */
{
    pParse->AddFunction(Take(yymsp[0].minor.yy55));
}
        break;
      case 7: /* type_reference ::= STRING_TYPE */
{
    yygotominor.yy64 = static_cast<int>(ExpressionType::String);
  yy_destructor(yypParser,9,&yymsp[0].minor);
}
        break;
      case 8: /* type_reference ::= NUMBER_TYPE */
{
    yygotominor.yy64 = static_cast<int>(ExpressionType::Number);
  yy_destructor(yypParser,10,&yymsp[0].minor);
}
        break;
      case 9: /* type_reference ::= BOOLEAN_TYPE */
{
    yygotominor.yy64 = static_cast<int>(ExpressionType::Boolean);
  yy_destructor(yypParser,11,&yymsp[0].minor);
}
        break;
      case 10: /* function_declaration ::= FUNCTION ID parenthesis_parameter_list type_reference NEWLINE statement_list END */
{
    auto pParameters = Take(yymsp[-4].minor.yy10);
    auto pBody = Take(yymsp[-1].minor.yy2);
    ExpressionType returnType = static_cast<ExpressionType>(yymsp[-3].minor.yy64);
    EmplaceAST<CFunctionAST>(yygotominor.yy55, yymsp[-5].minor.yy0.stringId, returnType, std::move(*pParameters), std::move(*pBody));
  yy_destructor(yypParser,12,&yymsp[-6].minor);
  yy_destructor(yypParser,8,&yymsp[-2].minor);
  yy_destructor(yypParser,14,&yymsp[0].minor);
}
        break;
      case 11: /* parenthesis_parameter_list ::= LPAREN RPAREN */
{
    yygotominor.yy10 = Make<ParameterDeclList>().release();
  yy_destructor(yypParser,15,&yymsp[-1].minor);
  yy_destructor(yypParser,16,&yymsp[0].minor);
}
        break;
      case 12: /* parenthesis_parameter_list ::= LPAREN parameter_list RPAREN */
{
    MovePointer(yymsp[-1].minor.yy10, yygotominor.yy10);
  yy_destructor(yypParser,15,&yymsp[-2].minor);
  yy_destructor(yypParser,16,&yymsp[0].minor);
}
        break;
      case 13: /* parameter_list ::= parameter_decl */
{
    CreateList(yygotominor.yy10, yymsp[0].minor.yy6);
}
        break;
      case 14: /* parameter_list ::= parameter_list COMMA parameter_decl */
{
    ConcatList(yygotominor.yy10, yymsp[-2].minor.yy10, yymsp[0].minor.yy6);
  yy_destructor(yypParser,17,&yymsp[-1].minor);
}
        break;
      case 15: /* parameter_decl ::= ID type_reference */
{
    EmplaceAST<CParameterDeclAST>(yygotominor.yy6, yymsp[-1].minor.yy0.stringId, static_cast<ExpressionType>(yymsp[0].minor.yy64));
}
        break;
      case 16: /* statement_list ::= statement_line */
{
    CreateList(yygotominor.yy2, yymsp[0].minor.yy16);
}
        break;
      case 17: /* statement_list ::= statement_list statement_line */
{
    ConcatList(yygotominor.yy2, yymsp[-1].minor.yy2, yymsp[0].minor.yy16);
}
        break;
      case 18: /* statement_line ::= statement NEWLINE */
{
    MovePointer(yymsp[-1].minor.yy16, yygotominor.yy16);
  yy_destructor(yypParser,8,&yymsp[0].minor);
}
        break;
      case 19: /* statement_line ::= error NEWLINE */
{
    yygotominor.yy16 = nullptr;
  yy_destructor(yypParser,8,&yymsp[0].minor);
}
        break;
      case 20: /* statement ::= ID ASSIGN expression */
{
    EmplaceAST<CAssignAST>(yygotominor.yy16, yymsp[-2].minor.yy0.stringId, Take(yymsp[0].minor.yy65));
  yy_destructor(yypParser,18,&yymsp[-1].minor);
}
        break;
      case 21: /* statement ::= PRINT expression */
{
    EmplaceAST<CPrintAST>(yygotominor.yy16, Take(yymsp[0].minor.yy65));
  yy_destructor(yypParser,19,&yymsp[-1].minor);
}
        break;
      case 22: /* statement ::= RETURN expression */
{
    EmplaceAST<CReturnAST>(yygotominor.yy16, Take(yymsp[0].minor.yy65));
  yy_destructor(yypParser,20,&yymsp[-1].minor);
}
        break;
      case 23: /* statement ::= IF expression NEWLINE END */
{
    EmplaceAST<CIfAst>(yygotominor.yy16, Take(yymsp[-2].minor.yy65));
  yy_destructor(yypParser,21,&yymsp[-3].minor);
  yy_destructor(yypParser,8,&yymsp[-1].minor);
  yy_destructor(yypParser,14,&yymsp[0].minor);
}
        break;
      case 24: /* statement ::= IF expression NEWLINE statement_list END */
{
    auto pThenBody = Take(yymsp[-1].minor.yy2);
    EmplaceAST<CIfAst>(yygotominor.yy16, Take(yymsp[-3].minor.yy65), std::move(*pThenBody));
  yy_destructor(yypParser,21,&yymsp[-4].minor);
  yy_destructor(yypParser,8,&yymsp[-2].minor);
  yy_destructor(yypParser,14,&yymsp[0].minor);
}
        break;
      case 25: /* statement ::= IF expression NEWLINE statement_list ELSE NEWLINE statement_list END */
{
    auto pThenBody = Take(yymsp[-4].minor.yy2);
    auto pElseBody = Take(yymsp[-1].minor.yy2);
    EmplaceAST<CIfAst>(yygotominor.yy16, Take(yymsp[-6].minor.yy65), std::move(*pThenBody), std::move(*pElseBody));
  yy_destructor(yypParser,21,&yymsp[-7].minor);
  yy_destructor(yypParser,8,&yymsp[-5].minor);
  yy_destructor(yypParser,22,&yymsp[-3].minor);
  yy_destructor(yypParser,8,&yymsp[-2].minor);
  yy_destructor(yypParser,14,&yymsp[0].minor);
}
        break;
      case 26: /* statement ::= WHILE expression NEWLINE END */
{
    EmplaceAST<CWhileAst>(yygotominor.yy16, Take(yymsp[-2].minor.yy65));
  yy_destructor(yypParser,23,&yymsp[-3].minor);
  yy_destructor(yypParser,8,&yymsp[-1].minor);
  yy_destructor(yypParser,14,&yymsp[0].minor);
}
        break;
      case 27: /* statement ::= WHILE expression NEWLINE statement_list END */
{
    auto pBody = Take(yymsp[-1].minor.yy2);
    EmplaceAST<CWhileAst>(yygotominor.yy16, Take(yymsp[-3].minor.yy65), std::move(*pBody));
  yy_destructor(yypParser,23,&yymsp[-4].minor);
  yy_destructor(yypParser,8,&yymsp[-2].minor);
  yy_destructor(yypParser,14,&yymsp[0].minor);
}
        break;
      case 28: /* statement ::= DO NEWLINE WHILE expression END */
{
    EmplaceAST<CRepeatAst>(yygotominor.yy16, Take(yymsp[-1].minor.yy65));
  yy_destructor(yypParser,24,&yymsp[-4].minor);
  yy_destructor(yypParser,8,&yymsp[-3].minor);
  yy_destructor(yypParser,23,&yymsp[-2].minor);
  yy_destructor(yypParser,14,&yymsp[0].minor);
}
        break;
      case 29: /* statement ::= DO NEWLINE statement_list WHILE expression END */
{
    auto pBody = Take(yymsp[-3].minor.yy2);
    EmplaceAST<CRepeatAst>(yygotominor.yy16, Take(yymsp[-1].minor.yy65), std::move(*pBody));
  yy_destructor(yypParser,24,&yymsp[-5].minor);
  yy_destructor(yypParser,8,&yymsp[-4].minor);
  yy_destructor(yypParser,23,&yymsp[-2].minor);
  yy_destructor(yypParser,14,&yymsp[0].minor);
}
        break;
      case 30: /* expression_list ::= expression */
{
    CreateList(yygotominor.yy63, yymsp[0].minor.yy65);
}
        break;
      case 31: /* expression_list ::= expression_list COMMA expression */
{
    ConcatList(yygotominor.yy63, yymsp[-2].minor.yy63, yymsp[0].minor.yy65);
  yy_destructor(yypParser,17,&yymsp[-1].minor);
}
        break;
      case 32: /* expression ::= ID LPAREN RPAREN */
{
    EmplaceAST<CCallAST>(yygotominor.yy65, yymsp[-2].minor.yy0.stringId, ExpressionList());
  yy_destructor(yypParser,15,&yymsp[-1].minor);
  yy_destructor(yypParser,16,&yymsp[0].minor);
}
        break;
      case 33: /* expression ::= ID LPAREN expression_list RPAREN */
{
    auto pList = Take(yymsp[-1].minor.yy63);
    EmplaceAST<CCallAST>(yygotominor.yy65, yymsp[-3].minor.yy0.stringId, std::move(*pList));
  yy_destructor(yypParser,15,&yymsp[-2].minor);
  yy_destructor(yypParser,16,&yymsp[0].minor);
}
        break;
      case 34: /* expression ::= LPAREN expression RPAREN */
{
    MovePointer(yymsp[-1].minor.yy65, yygotominor.yy65);
  yy_destructor(yypParser,15,&yymsp[-2].minor);
  yy_destructor(yypParser,16,&yymsp[0].minor);
}
        break;
      case 35: /* expression ::= expression LESS expression */
{
    EmplaceAST<CBinaryExpressionAST>(yygotominor.yy65, Take(yymsp[-2].minor.yy65), BinaryOperation::Less, Take(yymsp[0].minor.yy65));
  yy_destructor(yypParser,1,&yymsp[-1].minor);
}
        break;
      case 36: /* expression ::= expression EQUALS expression */
{
    EmplaceAST<CBinaryExpressionAST>(yygotominor.yy65, Take(yymsp[-2].minor.yy65), BinaryOperation::Equals, Take(yymsp[0].minor.yy65));
  yy_destructor(yypParser,2,&yymsp[-1].minor);
}
        break;
      case 37: /* expression ::= expression PLUS expression */
{
    EmplaceAST<CBinaryExpressionAST>(yygotominor.yy65, Take(yymsp[-2].minor.yy65), BinaryOperation::Add, Take(yymsp[0].minor.yy65));
  yy_destructor(yypParser,3,&yymsp[-1].minor);
}
        break;
      case 38: /* expression ::= expression MINUS expression */
{
    EmplaceAST<CBinaryExpressionAST>(yygotominor.yy65, Take(yymsp[-2].minor.yy65), BinaryOperation::Substract, Take(yymsp[0].minor.yy65));
  yy_destructor(yypParser,4,&yymsp[-1].minor);
}
        break;
      case 39: /* expression ::= expression STAR expression */
{
    EmplaceAST<CBinaryExpressionAST>(yygotominor.yy65, Take(yymsp[-2].minor.yy65), BinaryOperation::Multiply, Take(yymsp[0].minor.yy65));
  yy_destructor(yypParser,5,&yymsp[-1].minor);
}
        break;
      case 40: /* expression ::= expression SLASH expression */
{
    EmplaceAST<CBinaryExpressionAST>(yygotominor.yy65, Take(yymsp[-2].minor.yy65), BinaryOperation::Divide, Take(yymsp[0].minor.yy65));
  yy_destructor(yypParser,6,&yymsp[-1].minor);
}
        break;
      case 41: /* expression ::= expression PERCENT expression */
{
    EmplaceAST<CBinaryExpressionAST>(yygotominor.yy65, Take(yymsp[-2].minor.yy65), BinaryOperation::Modulo, Take(yymsp[0].minor.yy65));
  yy_destructor(yypParser,7,&yymsp[-1].minor);
}
        break;
      case 42: /* expression ::= PLUS expression */
{
    EmplaceAST<CUnaryExpressionAST>(yygotominor.yy65, UnaryOperation::Plus, Take(yymsp[0].minor.yy65));
  yy_destructor(yypParser,3,&yymsp[-1].minor);
}
        break;
      case 43: /* expression ::= MINUS expression */
{
    EmplaceAST<CUnaryExpressionAST>(yygotominor.yy65, UnaryOperation::Minus, Take(yymsp[0].minor.yy65));
  yy_destructor(yypParser,4,&yymsp[-1].minor);
}
        break;
      case 44: /* expression ::= NUMBER_VALUE */
{
    EmplaceAST<CLiteralAST>(yygotominor.yy65, CLiteralAST::Value(yymsp[0].minor.yy0.value));
}
        break;
      case 45: /* expression ::= STRING_VALUE */
{
    EmplaceAST<CLiteralAST>(yygotominor.yy65, pParse->GetStringLiteral(yymsp[0].minor.yy0.stringId));
}
        break;
      case 46: /* expression ::= BOOLEAN_VALUE */
{
    EmplaceAST<CLiteralAST>(yygotominor.yy65, CLiteralAST::Value(yymsp[0].minor.yy0.boolValue));
}
        break;
      case 47: /* expression ::= ID */
{
    EmplaceAST<CVariableRefAST>(yygotominor.yy65, yymsp[0].minor.yy0.stringId);
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
  ParseGrammarARG_FETCH;
#ifndef NDEBUG
  if( yyTraceFILE ){
    fprintf(yyTraceFILE,"%sFail!\n",yyTracePrompt);
  }
#endif
  while( yypParser->yyidx>=0 ) yy_pop_parser_stack(yypParser);
  /* Here code is inserted which will be executed whenever the
  ** parser fails */

    pParse->OnFatalError();
  ParseGrammarARG_STORE; /* Suppress warning about unused %extra_argument variable */
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
  ParseGrammarARG_FETCH;
#define TOKEN (yyminor.yy0)

    (void)yymajor;  // Silence compiler warnings.
    pParse->OnError(TOKEN); // TOKEN has type defined in '%token_type'
  ParseGrammarARG_STORE; /* Suppress warning about unused %extra_argument variable */
}

/*
** The following is executed when the parser accepts
*/
static void yy_accept(
  yyParser *yypParser           /* The parser */
){
  ParseGrammarARG_FETCH;
#ifndef NDEBUG
  if( yyTraceFILE ){
    fprintf(yyTraceFILE,"%sAccept!\n",yyTracePrompt);
  }
#endif
  while( yypParser->yyidx>=0 ) yy_pop_parser_stack(yypParser);
  /* Here code is inserted which will be executed whenever the
  ** parser accepts */
  ParseGrammarARG_STORE; /* Suppress warning about unused %extra_argument variable */
}

/* The main parser program.
** The first argument is a pointer to a structure obtained from
** "ParseGrammarAlloc" which describes the current state of the parser.
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
void ParseGrammar(
  void *yyp,                   /* The parser */
  int yymajor,                 /* The major token code number */
  ParseGrammarTOKENTYPE yyminor       /* The value for the token */
  ParseGrammarARG_PDECL               /* Optional %extra_argument parameter */
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
  ParseGrammarARG_STORE;

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
