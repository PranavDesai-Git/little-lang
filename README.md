# GraphLang

An experimental, blazing fast purely functional Graph Reduction Virtual Machine written in C.

## Roadmap & Upcoming Features
Check out the [TODO.md](TODO.md) for a list of upcoming architectural milestones!

GraphLang is a minimal, fast, integer-only programming language. At its core, it is an expression evaluator built entirely on a **strict binary tree**, with built-in support for variables, functions, and conditionals.

## Syntax Design

GraphLang uses a clean, modern LL(1) syntax. The parser distinguishes between variables and functions dynamically by using a single token of lookahead:

- **Variable Definition:** `def x = 10`
- **Function Definition:** `def add(a, b) = a + b`
- **Variable Usage:** `x + 5` (Evaluates to 15)
- **Function Call:** `add(x, 5)`
- **Comments:** Any text following a single quote `'` is ignored by the lexer.

Because function calls are always followed by parentheses or arguments, the parser can easily differentiate them from variables.

## Operational Rules

- **Execution:** GraphLang evaluates source code by reading `.gl` files from top to bottom.
- **Truthiness:** There is no boolean type. `1` is True and `0` is False. Comparison operators naturally evaluate to `0` or `1`.
- **Variables:** Variables are completely mutable. Redefining a variable simply overwrites its previous entry in the hashtable.
- **Errors & Result Types:** Functions do not segfault on expected errors (like division by zero). Instead, they return the integer `0` tagged with a custom error flag. If a user tries to use an error-flagged node in a math operation without checking it first, the program safely aborts (Strict Error Propagation). Users handle these errors using a built-in `catch_flag` macro which clears the flag and executes a branch.

## Architecture & Evaluation

### Strict Binary Tree & Cons Lists
Everything in GraphLang is a strict binary tree. Function calls take exactly two arguments (mapped directly to the `left` and `right` AST node pointers). 

To support arbitrary amounts of data, GraphLang implements **Lists** using a Lisp-style Cons Cell approach. A list is simply a chain of binary nodes where `left` holds the integer value (the head) and `right` points to the next node in the list (the tail).

### Graph Reduction (Tree Rewriting)
Unlike traditional interpreters that maintain complex "Scope Stacks" or environments at runtime, GraphLang evaluates functions using **Graph Reduction**:
1. When a function is called, the evaluator grabs the function's AST template from the hashtable.
2. It clones the tree, physically swaps the placeholder variables for the evaluated `left` and `right` arguments, and **replaces** the function call node with this new tree.
3. The tree is then evaluated and collapses down into a single `LITERAL` node.

### Advanced Features (Lazy Evaluation & First-Class Functions)
Because of the Graph Reduction architecture, GraphLang inherently supports advanced functional programming paradigms:
- **First-Class Functions:** Functions are just AST trees in a hashtable. A function name can be passed as an argument to another function (e.g., `map(func_name, list)`), which then dynamically grafts that function tree onto the list at runtime.
- **Lazy/Infinite Lists:** Because GraphLang doesn't evaluate arguments until they are strictly needed, users can define infinite, self-referential lists. The tail of the list is simply an unevaluated branch of the AST.

### Memory Management (Mark-and-Sweep GC)
Because Graph Reduction physically overwrites and orphans tree nodes during execution, GraphLang manages memory using a custom **Mark-and-Sweep Garbage Collector**:
1. **Mark:** The GC pauses execution, walks the Hashtable environment, recursively traverses every active binary tree, and flips a `FLAG_GC_MARKED` bit on each reachable node.
2. **Sweep:** The GC iterates through a master pool of all allocated nodes. Any node without the mark bit is safely destroyed, while marked nodes have their bit cleared for the next cycle.

### Data-Driven Environment (Hashtable)
All variables, user-defined functions, and built-in operators (`+`, `-`, `*`, `/`) are stored in an $O(1)$ **Hashtable Environment**. The parser uses this to map operators directly to C function pointers (Dynamic Dispatch), removing the need for hardcoded `switch` statements.

### Optimization Flags (Compile-Time Evaluation)
The parser is aggressive. Pure math expressions without variables are instantly evaluated at parse-time (**Constant Folding**). For runtime execution, nodes use a 32-bit `unsigned int` bitmask for flags (leaving over 28 bits open for user-defined decorators, metadata, or explicit error checkers):
- `ZERO`: Short-circuits operations (e.g., `heavy_func() * 0` instantly returns `0` without walking the left branch).
- `ONE`: Identity operations. Evaluates `x * 1` by returning `x`.
- `TWO`: Strength reduction. Multiplication/division by 2 are optimized to native bitwise shifts (`<< 1` and `>> 1`).

## Building & Running

A `Makefile` is provided for standard compilation.

```bash
# Build the project
make

# Build and execute
./run.sh
```
