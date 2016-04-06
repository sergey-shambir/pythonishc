# Минимальная поддержка процедурного стиля в интерпретаторе

Интерпретатор по-прежнему читае ввод построчно, строит AST и с его помощью выполняет инструкции.

С предыдущей итерации поддерживаются переменные, присваивания, печать выражений, управляющие инструкции `if`, `if .. else`, `while`, `do .. while`. В данном примере добавлена минимальная поддержка процедурного подхода:

- пользовательские функции, принимающие (0..∞) аргументов
- встроенные функции `sin(x)` и `rand(min, max)`
- вызов функций
- инструкция `return`
- смена области видимости переменных внутри функции

##### Ввод
```
def sqr(x)
  result = x * x
  return result
end

result = 2016
x = 2016
y = sqr(10)
print y
print x
print result
```

##### Вывод
```
result: 100
result: 2016
result: 2016
```

### Изменения в сравнении с lemon-6-full-structured

Новые узлы AST:

- CReturnAST (подкласс IStatementAST)
- CCallAST (подкласс IExpressionAST)
- CFunctionAST (подкласс IFunctionAST)

Новые классы:

- CVariablesScope реализует область видимости переменных. Создаётся узлами `CProgramAST` и `CFunctionAST`.
- CRandFunction реализует встроенную функцию `rand(min, max)`
- CSinFunction реализует встроенную функцию `sin(x)`

Изменения в `BatchGrammar.lemon`

- **пока не описаны**

### Requirements

- Ubuntu: Install `lemon` package
- MS Windows: compile Lemon from [source code (hwaci.com)](http://www.hwaci.com/sw/lemon/). It's just two files, `lemon.c` and `lempar.c`.
- Use any modern C++ compiler.
