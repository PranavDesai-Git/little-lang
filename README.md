# Little-Lang

Little-Lang is a minimal, fast, integer-only programming language. At its core, it is an expression evaluator built on a binary tree, with built-in support for variables, functions, and conditionals.

## Syntax Design

Little-Lang uses a clean, modern LL(1) syntax. The parser distinguishes between variables and functions dynamically by using a single token of lookahead:

- **Variable Definition:** `def x = 10`
- **Function Definition:** `def add(a, b) = a + b`
- **Variable Usage:** `x + 5` (Evaluates to 15)
- **Function Call:** `add(x, 5)`

Because function calls are always followed by parentheses or arguments, the parser can easily differentiate them from variables without needing special prefixes or symbols.

## Architecture & Environment

### Data-Driven Environment (Hashtable)
Little-Lang operates on a Data-Driven Design. All variables, user-defined functions, and built-in operators (`+`, `-`, `*`, `/`) are stored in an $O(1)$ **Hashtable Environment**. 
The core evaluator does not use hardcoded `switch` statements for math operations. Instead, operators map directly to C function pointers in the hashtable (Dynamic Dispatch), making the language incredibly extensible.

### AST Node Structure
The Abstract Syntax Tree (AST) is built using a C Tagged Union. Each node contains:
- **Types:** `LITERAL`, `IDENTIFIER`, `FUNCTION_CALL`, etc.
- **Data:** All numerical values are strictly **integers**. No floating point logic or overhead is used.
- **Function Pointers:** For operations, nodes store a C function pointer fetched from the Hashtable.

### Optimization Flags (Constant Folding & Strength Reduction)
The parser applies intelligent flags to nodes during AST construction, allowing the evaluator to perform advanced optimizations at runtime without evaluating branches:

- `ZERO`: Short-circuits operations. For example, `heavy_func() * 0` instantly returns `0` without evaluating the left branch.
- `ONE`: Identity operations. Evaluates `x * 1` or `x / 1` by simply returning `x`.
- `TWO`: Strength reduction. Multiplication and division by 2 are optimized down to extremely fast bitwise integer shifts (`<< 1` and `>> 1`).
- `LEFTVAR` / `RIGHTVAR`: Signals the evaluator to treat a branch as a memory location/identifier rather than evaluating its value (vital for assignments).

## Building & Running

A `Makefile` is provided for standard compilation.

```bash
# Build the project
make

# Build and execute
./run.sh
```
