# GraphLang TODOs

This file tracks the upcoming architectural milestones and features planned for the GraphLang Virtual Machine.

## 1. The Accumulator Pattern (Tail Recursion)
**Goal:** Rewrite `fib(n)` to compute in `O(N)` time instead of `O(2^N)`.
**Details:** Currently, `fib(n)` uses tree recursion, causing massive slowdowns for large numbers like `fib(40)`. By writing a `fib_helper(n, a, b)` that counts from the bottom up, we can compute `fib(40)` instantly using the accumulator pattern.

## 2. Tail Call Optimization (TCO)
**Goal:** Implement a Trampoline in `Evaluator.c` to achieve `O(1)` memory usage for recursive functions.
**Details:** Even with the accumulator pattern, deep recursion (e.g., `fib(50000)`) will cause the C stack to overflow and crash the VM. We need to implement a Trampoline pattern that intercepts tail calls, pops the current C stack frame, and reuses it for the next call. This will give the language infinite recursion capabilities.

## 3. IO Monads (Side Effects)
**Goal:** Introduce state and side effects without breaking functional purity.
**Details:** GraphLang is currently a pure graph reduction engine. We need to build a new plugin that handles IO (printing, reading files) by passing an "IO State" token through the graph, ensuring that side effects are evaluated in the correct order.

## 4. Frontend Lexer & Parser
**Goal:** Stop writing ASTs manually in C and allow users to write code in a text file.
**Details:** Build a Lexer and Parser (either in C or using a tool like Flex/Bison) that takes a text file containing GraphLang syntax (e.g., `let fib = \n -> ...`) and automatically generates the AST blueprint for the VM to execute.
