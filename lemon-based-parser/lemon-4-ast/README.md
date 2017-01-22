# Interpreter with LALR Parser and AST

Interpreter takes input line-by-line, builds AST for each line and then interprets it.

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

### Changes since lemon-3-error-recovery example

AST added, nothing else was changed.

- `BatchAST.h` contains declarations for classes that represent Abstract Syntax Tree nodes
- `BatchAST.cpp` contains matching definitions
- `CBatchParser` has new method `AddStatement` and new template method `MakeAST`
- `CBatchParser` tracks existing AST nodes on own object pool &mdash; it controls their lifetime.
- `CBatchParser::OnError` no longer sets `m_isFatalError`, but new method `CBatchParser::OnFatalError` does.
- inline actions in `BatchGrammar.lemon` changed to create AST nodes instead of direct evaluation.

### Learn More

- [Советы по кодированию AST на русском](https://gist.github.com/sergey-shambir/16be54fd487d4e9fd349)

### Requirements

- Ubuntu: Install `lemon` package
- MS Windows: compile Lemon from [source code (hwaci.com)](http://www.hwaci.com/sw/lemon/). It's just two files, `lemon.c` and `lempar.c`.
- Use any modern C++ compiler.
