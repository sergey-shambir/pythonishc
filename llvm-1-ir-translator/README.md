# Минимальный транслятор, основанный на LLVM

Интерпретатор из примера `lemon-8` был превращён в транслятор, использующий LLVM для генерации целевого кода. В процессе перехода появились регрессии:

- кодогенерация для типов Boolean, String временно не работает
- кодогенерация для циклов и ветвлений также не работает

Также изменилась грамматика:

- циклы, ветвления и типы Boolean/String сохранены на будущее
- теперь инструкции должны находится строго внутри функций
- точкой входа служит функция `main` без аргументов и без `return`

Транслятор читает входной поток целиком, и затем пишет в выходной поток LLVM-IR код в человекочитаемом виде. Этот код можно скомплировать в исполняемый файл командой `"clang my_code.ll"`.

##### Ввод
```
def main()
  x = 2 + 34 / 2
  print x - 4 * 0.5
end
```

##### Вывод
```
; ModuleID = 'main module'

@str = internal constant [5 x i8] c"%lf\0A\00"

declare i32 @printf(i8*, ...)

define i32 @main() {
entry:
  %0 = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([5 x i8]* @str, i32 0, i32 0), double 0x4030FFFFFF800000)
  ret i32 0
}
```

### Изменения в сравнении с lemon-8-types

- новый класс `CCompilerDriver` реализует драйвер компилятора, в том числе чтение ввода и связывание между собой парсера и кодогенератора.
- новые интерфейсы IExpressionVisitor, IStatementVisitor открывают путь шаблону проектирования "Посетитель" ("Visitor")
- новые классы `CCodeGenerator`, `CBlockCodeGenerator`, `CExpressionCodeGenerator` генерируют код для программы целиком, для тела функции и для выражения соответственно
- класс `CInterpreterContext` переименован в `CFrontendContext` и претерпел изменения в типах данных, с которыми он работает.
- класс `CValue` был удалён за ненадобностью
- методы `Evaluate` и `Execute` у выражений и инструкций были удалены, теперь узлы AST лишь дают возможность обхода посетителем, но сами не содержат логики генерации кода.

### Использованная литература

- [язык Kaleidoscope, глава 3](http://llvm.org/docs/tutorial/LangImpl3.html)
- [передача строковой константы как аргумента фукнции](http://stackoverflow.com/questions/16656855/llvm-ir-string-initialization)
- [как заставить Clang скомпилировать C++ в LLVM-IR](http://stackoverflow.com/questions/9148890/how-to-make-clang-compile-to-llvm-ir)

### Системные требования

- Рекомендуются LLVM версии 3.8, lemon произвольной версии
- Для ОС Ubuntu: установите пакеты `lemon`, `clang`, `llvm-dev`, `cmake`
- Для ОС Windows: установите CMake, [загрузите и установите Clang](http://llvm.org/releases/download.html), соберите Lemon из [исходных кодов (hwaci.com)](http://www.hwaci.com/sw/lemon/), соберите библиотеки LLVM из исходников с помощью CMake.
