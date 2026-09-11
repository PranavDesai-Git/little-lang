# GraphLang Syntax Specification

GraphLang is a whitespace-agnostic functional programming language built on a pure Graph Reduction backend.

## 1. Top-Level Definitions
Every definition (variable or function) must start with the `def` keyword. This makes parsing completely immune to whitespace formatting issues.

### Variables
```text
def x = 10
def y = x + 5
```

### Functions
Functions use space-separated application (no parentheses or commas needed for parameters). The backend natively supports chaining an unlimited number of arguments.
```text
def add x y = x + y
```

## 2. Conditionals (Two Flavors)
Both of these syntax flavors compile down to the exact same `nativeIf` AST node in the backend!

### Guards (Haskell-Style)
For clean, multi-branch function definitions. The pipe `|` denotes a branch condition.
```text
def fib n
  | n < 2 = n
  | 1     = fib (n - 1) + fib (n - 2)
```
*(Note: `1` is used as a truthy fallback, equivalent to `otherwise`).*

### Inline If-Then-Else
For quick, inline conditional expressions anywhere in the code.
```text
def x = if y < 5 then 10 else 20
```

## 3. Function Application
Calling functions also uses space-separated arguments! Use parentheses to group mathematical precedence.
```text
def main = add (fib 10) 5
```

## 4. Lists (Functional Cons Cells)
Because GraphLang is pure functional, Lists are built exactly like Haskell or Lisp (using Linked Lists). 

You can build them manually using the native `cons` function:
```text
def my_list = cons 1 (cons 2 (cons 3 nil))
```

Or, the Parser can provide Syntactic Sugar that automatically compiles down to `cons` chains:
```text
def my_list = [1, 2, 3]
```

### List Comprehensions (Haskell Style)
GraphLang supports powerful mathematical list comprehensions to generate, filter, and map lists dynamically.
```text
def evens = [x | x <- [1..10], x % 2 == 0]
```
*Note: Under the hood, the parser will translate this into recursive `if` and `cons` AST structures.*

## 5. Local Variables (`let` bindings)
For defining local variables inside of a function, GraphLang uses the ML-style `let ... in` syntax.
```text
def math x = 
    let a = x * 2
        b = x * 3
    in a + b
```

## 6. I/O and Monads
Because GraphLang is functionally pure, it avoids dirty side-effects by using the **IO Monad** (just like Haskell). 
You interact with the outside world by chaining IO actions using the bind operator `>>=` or a `do` block.
```text
def main = print "Hello" >>= \_ -> print "World"
```
*(Detailed Monad syntax and `do` blocks will be expanded in a future update).*

## 7. Modules and Imports
You can import other `.gl` files to share code across multiple files.
```text
import Math
import System.IO
```

### C Plugin System (FFI)
GraphLang supports a blazing-fast Foreign Function Interface (FFI). Because the entire language evaluates down to the unified `Node*` struct, you can write Native functions in pure C, compile them into `.so` Shared Libraries, and hot-load them directly into GraphLang at runtime!
```text
import_c "my_fast_plugin.so"
```
*(Any C function matching `Node* my_func(Node* args)` exported in that library becomes instantly callable).*

## 8. Comments
GraphLang uses standard C-style comments. The Lexer will completely strip these out before the Parser sees them.
```text
// This is a single line comment
/* This is a 
   multi-line comment */
```

## 9. Lexer Requirements
The Lexer will need to chunk text into these specific tokens:

* **Keywords**: `def`, `if`, `then`, `else`, `and`, `or`, `not`, `cons`, `head`, `tail`, `nil`, `let`, `in`, `import`, `import_c`
* **Identifiers**: Variable and function names (e.g., `fib`, `x`, `Math`)
* **Numbers**: Integer literals
* **Symbols**: `=`, `|`, `(`, `)`, `[`, `]`, `,`, `<-`, `..`, `>>=`
* **Operators**: `+`, `-`, `*`, `/`, `%`, `<`, `>`, `==`
* **Strings**: `"..."` (Used for file paths in imports)
* **Comments**: Handled natively by the Lexer and ignored.
