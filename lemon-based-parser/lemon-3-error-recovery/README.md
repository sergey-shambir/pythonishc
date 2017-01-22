# Interpreter with LALR Parser

Interpreter takes input line-by-line and interprets each line variable assignment or print.

##### Input
```
print 21 + 13 - (1.12 * 2 - 1.51 / 2)
x = 10
y = 12.2
pinrt x*y
print x*y
```

##### Output
```
result: 32.515
Syntax error at (1,7).
result: 122
```

### Changes since lemon-2-interpreter example

Error recovery added, nothing else changed.

- `BatchGrammar.lemon`: new rule `statement_line ::= error NEWLINE.`, added C++ code called on `%parse_failure`.
- `CBatchParser::OnError` no longer sets `m_isFatalError`, but new method `CBatchParser::OnFatalError` does.
- Macro `YYNOERRORRECOVERY` removed.

### Error Handling Startegies

Lemon follows Bison (Yacc) error recovery strategies. There are 3 ways:

- panic &mdash; stop compilation, output 1 fatal error. We used that approach in previous examples.
- error rules and stack unwinding &mdash; parser unwinds stack until it can shift non-terminal symbol `error`. Note that non-terminal rule `error` never defined, it's keyword for Lemon language. We used this approach here.
- error rules &mdash; special rules added for common error patterns, but C++ action reports error. We don't use this approach.

### What Lemon Documentation Says

When a Lemon-generated parser encounters a syntax error, it first invokes the code specified by the %syntax_error directive, if any. It then enters its error recovery strategy. The error recovery strategy is to begin popping the parsers stack until it enters a state where it is permitted to shift a special non-terminal symbol named ``error''. It then shifts this non-terminal and continues parsing. But the %syntax_error routine will not be called again until at least three new tokens have been successfully shifted.

If the parser pops its stack until the stack is empty, and it still is unable to shift the error symbol, then the %parse_failed routine is invoked and the parser resets itself to its start state, ready to begin parsing a new file. This is what will happen at the very first syntax error, of course, if there are no instances of the ``error'' non-terminal in your grammar.

### Requirements

- Ubuntu: Install `lemon` package
- MS Windows: compile Lemon from [source code (hwaci.com)](http://www.hwaci.com/sw/lemon/). It's just two files, `lemon.c` and `lempar.c`.
- Use any modern C++ compiler.
