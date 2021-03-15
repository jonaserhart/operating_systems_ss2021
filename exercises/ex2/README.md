# Exercise 2 - Binary Search Tree documentation

## Implementation notes

### Debug makros in `dbg.h`

I use my own debug makros defined in `dbg.h` where i print debug and error messages in a predifined format.
This makes is easier for me to debug code when i run it remotely (without any visual IDE over uibk-ssh for example).

I know the specifications said not to print error messages or logging, but i implemented these makros to only log messages when a fatal problem occurs (for example a memory allocation error due to not having enough memory).  
These makros do not interfere with the output of the program in general execution. (all tests pass)

### Why I used `bstree` as node

I implemented all methods so that the `bstree` type is used as a node-like type, because a binary tree consists of sub-trees.

The only disatvantage is that the size of the tree is computed in O(n) instead of O(1).

All other function implementations are easier since it is all recursive.  
In general, I don't have to use an auxilary function for each method.

## Task 3

In this task no implementation is required. Its purpose is to ensure you understand the background behind the details of the previous task. You should be able to answer the following questions without the use of any additional notes. Presenting images and examples is fine, though not required.

### When would you use a sorted binary tree over a linked list?

I would use a tree if i needed the stored values in a sorted order.

For example if i seldom store a value and often retrieve a value;

### What would you change if the tree had to support int values instead of floats? Can you use this approach to support any value type, or are there limitations?

- `int` -> `float`  
if i just change the type in the header file, an int could be used instead of a float (we would also need to change the VALUE_FORMAT makro)

- other types  
sadly, this only works for types that can be compared directly using `<,>,==`.  
For example: when using a `string` or chararray we could not search for a value using the implemented comparations.  
We would also need to define a comparator function for `value_t`in the header file and change the memory allocation process for a node according to the type. 

### Why do we use multiple translation units instead of putting everything a single file?

- The code is more clear and can be structured better

- functions defined in header files do not have to be implemented before being used in another function

- incremential builds feature makes it so (if implemented that way in Makefile) that not everything has to be recompiled on every change of code

### Is the development cycle (i.e. changing a source file, recompiling and running tests) faster when using multiple translation units? Explain your answer.

- incremential builds (see previous question)

### What is a header guard and why is it needed?

#### Explanation

When including header files, all content of the header file is copied to the current file.

If we include multiple headers, it could occur that we eventually include functions with the same name from different files, leading to an error.

To prevent this, 'header guards' are used to ensure that one name is only defined once. 

#### Syntax

```c
#ifndef __dbg_h__
#define __dbg_h__
// only define dbg.h if it has not bee defined
// define dbg.h here
#endif
```

### Why is struct bstree not defined in the header file? What are the implications?



### Explain the const in the parameter list of bstree_print, is it required?

the `const` implicates that the value is not changed in the execution of the function.

it is not required, but if someone wanted to implement that function, they don't get tempted to change the value and produce unwanted bahaviour.

### Explain memory leaks. Why are memory leaks bad?

a memory leak occurs when a program reserves memory and does not release it when its not needed anymore.

this memory is then inaccessable to other processes

if memory is very limited on the system, this can quickly become a problem due to important processes not having enough memory 

### What is the reason behind writing everything in English?

all (most of all) programming languages are based on the english language, so it makes sense to write documentations in english.

### Why should you use static for non-exported functions?

### Why should we comment our source code? Is it always needed? What should the comment state? 

If some other programmer should understand your code, you should either write it so that it does not need to be commented or comment it so that everyone knows what your code does.

### What is self-documenting code?

If you name variables and functions right, a code can be self-documenting (not needing any comments, because it is clear from the code what it does) 

### Why should the module not output debug messages?

To not interfere with the tests

### Why and when should you use assert?

assert should be used in tests or when a program should be exited when a condition is not met.

### What are the benefits of using a Makefile over calling the compiler by hand?

Structured building, shortcuts

### Imagine it was your job to design the interface for the bstree module (bstree.h). What, if anything, would you have done differently, and why?

