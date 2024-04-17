---
title: Clojure engine in C++20
---


`coloru` is a very limited Clojure engine written in C++20.

It is developed as an excercise to learn implementing tail recursion optimization.
Whether or not the optimization is applied can be chosen with an command line argument.

# Language specification

The executed source code is a subset of Clojure.

These are the limits:

* Numbers are only integers.
* Macro's are not allowed.
* Global values are not allowed, so `def` is rejected.
* Lists can be used.
* Vectors cannot be used as data type, only as parameter list.
* Quote must be done with `'`.
* Parameter destructuring for lists is implemented
    * The ampersand `&` is allowed.
* There are only 3 builtins: `nil?` and `empty?` and `cons`.
* `println` is implemented.
* Loops must be implemented with recursion.
* No `and`, `or` and `not` in `if`.
* Lambda functions are constructed with `fn`.
* Named functions are constructed with `defn`.

Despite the limitations it is possible to experiment with simple
recursive functions such as faculty and list counting.

After transformation the tail recursive code can be shown on the screen.
This helps to understand CPS.

# Command line usage

## Options

These are the command line options:

`-f <file>`
: read `<file>` and run the Clojure code

`-d`
: generate debug messages

`-t`
: do tail recursion optimization

`-l`
: print the optimized code on the screen

`-e`

: starts the program in the debugger

## Examples

Run an example:

~~~~
coloru -f vb1.clj
~~~~

Run an example with debug:

~~~~
coloru -d -f vb1.clj
~~~~

Run an example with optimization:

~~~~
coloru -t -f vb1.clj
~~~~

Do the optimization and show the optimzed code without running:

~~~~
coloru -t -l -f vb1.clj
~~~~

# Build

Just start `make`

~~~~
make
~~~~

The executable `coloru` can be found in the current directory.
Copy and use it. 

# Log

## 17/ 4/2024 version `0.6.1`

These are the corrections/additions.

* `cons` is added.
* `println` and `print` return `nil`.
* The extra scope for builtins is removed.
* Extra logging is added in `runner`.
* The use of a vector `[]` as data type returns an error.

# Links

* [https://eli.thegreenplace.net/2017/on-recursion-continuations-and-trampolines/](https://eli.thegreenplace.net/2017/on-recursion-continuations-and-trampolines/)

