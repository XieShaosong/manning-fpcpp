# Functional programming in C++

![Book cover](https://gitlab.com/manning-fpcpp-book/code-examples/raw/master/.resources/fpcpp-cover.png)

## Code examples

This repository contains runnable code examples from the book Functional Programming in C++.

### The layout of the examples

The examples are separated into different directories based on the chapter of the book they belong to.

- [`chapter-01`](https://gitlab.com/manning-fpcpp-book/code-examples/tree/master/chapter-01) - Introduction to functional programming
- [`chapter-02`](https://gitlab.com/manning-fpcpp-book/code-examples/tree/master/chapter-02) - Getting started with functional programming
- [`chapter-03`](https://gitlab.com/manning-fpcpp-book/code-examples/tree/master/chapter-03) - Functional objects
- [`chapter-04`](https://gitlab.com/manning-fpcpp-book/code-examples/tree/master/chapter-04) - Creating new functions from the old ones

The `common` directory contains files that are used in multiple different examples like the `person_t` type defined in the `<person.h>` header file.

The `3rd-party` directory contains free/libre open source 3rd party libraries used in the examples.

### Running the examples

Most examples are contained in a single `.cpp` file that can be easily executed in any IDE. For people who prefer working in a terminal, or an editor like Vim or Emacs, an example Makefile compatible with the GNU Make command has been provided that can be used to compile the example. The Makefile uses `GCC` as the compiler, but this can be changed by altering the value of the `CXX` variable.

Most of these examples use `C++11` and `C++14`, so you will need a modern compiler in order to compile them. The latest stable versions of GCC or Clang are recommended.

