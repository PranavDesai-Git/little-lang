# Little-Lang

Little-Lang is a minimal, fast, integer-only programming language. At its core, it is an expression evaluator built on a binary tree, with built-in support for variables, functions, and conditionals.

## Syntax Design

To keep lexing and parsing as simple and fast as possible, Little-Lang uses a unique single-character prefix/suffix syntax, removing the need for complex keyword tokenization:

- **Variable Declaration:** `&x 10`
- **Variable Usage:** `*x`
- **Function Declaration:** `$myFunc x y ...`
- **Function Call:** `myFunc!`

## Architecture & AST

The Abstract Syntax Tree (AST) is built using a C Tagged Union (Algebraic Data Type). 

Each node in the binary tree contains an "OR type" (Union) for its specific data, along with a type indicator and a `FLAG` property for compiler-level optimizations.

### Node Structure

- **Types:** `LITERAL`, `VARIABLE`, `FUNCTION`, `BINARY_OP`, etc.
- **Data:** All numerical values are strictly **integers**. No floating point logic or overhead is used.

### Optimization Flags (Constant Folding & Strength Reduction)

The parser applies intelligent flags to nodes during AST construction, allowing the evaluator to perform advanced optimizations at runtime:

- `ZERO`: Short-circuits operations. For example, `(heavy_func!) * 0` instantly returns `0` without evaluating the left branch.
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
