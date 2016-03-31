# Structured language interpreter with LALR Parser and AST

Interpreter takes input line-by-line, builds AST for each line and then interprets it.

Interpreter supports expressions, variables and structured programming.

##### Input
```
print 21 + 13 - (1.12 * 2 - 1.51 / 2)
x = 10
y = 12.2
pinrt x*y
print x*y
if x
  x = x + 1
end
print x
```

##### Output
```
result: 32.515
Syntax error at (1,7).
result: 122
result: 11
```

### Changes since lemon-4-ast example

New AST nodes:

- CProgramAST
- CIfAST

Changes in `CBatchParser` class:

- AST object pool removed since we use `%destructor` directive in LEMON and no longer need object pool idiom.
- new methods added: `EnterBlock`, `ExitBlock`.
- parser creates `CProgramAST` instance and just puts statements into the last block. `CProgramAST` executes statements, other blocks do not.

After introducing blocks, only top-level statements are executed automatically since only top-level `CProgramAST` block runs execution. In the following example `print 1` executed only when parent `if`-statement runs it:

```
if 0
  print 1
end
```

Changes in `BatchGrammar.lemon`

- rule `if_condition` added to create AST for `"if"` and enter if-block
- rule `if_condition_line` added to garantue that rule `if_condition` reduced before any nested statement
- new variant of `statement` added to reduce whole if statement and exit if-block.
- LEMON "destructors" feature now used to garantue correct AST deletion. See `%destructor` directives.
- New header file `BatchParser_private.h` added to move C++ code away from of LEMON file.

### Requirements

- Ubuntu: Install `lemon` package
- MS Windows: compile Lemon from [source code (hwaci.com)](http://www.hwaci.com/sw/lemon/). It's just two files, `lemon.c` and `lempar.c`.
- Use any modern C++ compiler.
