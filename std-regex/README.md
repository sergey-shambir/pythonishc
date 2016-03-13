# Regex Search Examples

There are 3 examples demonstrating how to use regular expressions from STL &mdash; `std::regex` (`<regex>` header file).

### Identifier matcher (`match-id.cpp`)

This example uses [`std::regex_match`](http://www.cplusplus.com/reference/regex/regex_match/).
- Scans input line-by-line
- Each line checked by regular expression
- If whole line is identifier, program prints `is identifier`
- Otherwise, it prints `is not identifier`

### Basic Identifier Scanner (`scan-ids.cpp`)

This example uses [`std::regex_seach`](http://www.cplusplus.com/reference/regex/regex_search/).

- Scans input line-by-line
- Each line parsed from left to right in `while` loop without scanning the same substring twice
- To achieve that, we use iterator-based function overload:

```cpp
std::regex_search(std::string::iterator begin,
                  std::string::iterator end,
                  std::smatch & match,
                  std::regex const& pattern)
```

### Improved Identifier Scanner (`scan-ids-not-comments.cpp`)

This example uses [`std::regex_seach`](http://www.cplusplus.com/reference/regex/regex_search/).

- Still scans line-by-line and parses each line from left to right.
- Does not scan comments in form `/* */`.
- Member variable `m_isInComment` and 2 new regular expressions added.

##### Input example
```cpp
do {
    x++;
    counter += 1;
} while (x != y);
/* comments are ignored */
y *= 2;
/*/ we are in comment
    in multiline comment
   */
```

##### Output
```
counter
do
while
x
y
```