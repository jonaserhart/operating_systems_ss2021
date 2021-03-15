# Exercise 2 - Binary Search Tree documentation
## Table of contents
- [Exercise 2 - Binary Search Tree documentation](#exercise-2---binary-search-tree-documentation)
  * [Implementation notes](#implementation-notes)
    + [Debug makros in `dbg.h`](#debug-makros-in--dbgh-)
    + [Why I used `bstree` as node](#why-i-used--bstree--as-node)
    + [Test output:](#test-output-)
  * [Task 3](#task-3)
    + [When would you use a sorted binary tree over a linked list?](#when-would-you-use-a-sorted-binary-tree-over-a-linked-list-)
    + [What would you change if the tree had to support int values instead of floats? Can you use this approach to support any value type, or are there limitations?](#what-would-you-change-if-the-tree-had-to-support-int-values-instead-of-floats--can-you-use-this-approach-to-support-any-value-type--or-are-there-limitations-)
    + [Why do we use multiple translation units instead of putting everything a single file?](#why-do-we-use-multiple-translation-units-instead-of-putting-everything-a-single-file-)
    + [Is the development cycle (i.e. changing a source file, recompiling and running tests) faster when using multiple translation units? Explain your answer.](#is-the-development-cycle--ie-changing-a-source-file--recompiling-and-running-tests--faster-when-using-multiple-translation-units--explain-your-answer)
    + [What is a header guard and why is it needed?](#what-is-a-header-guard-and-why-is-it-needed-)
      - [Explanation](#explanation)
      - [Syntax](#syntax)
    + [Why is struct bstree not defined in the header file? What are the implications?](#why-is-struct-bstree-not-defined-in-the-header-file--what-are-the-implications-)
    + [Explain the const in the parameter list of bstree_print, is it required?](#explain-the-const-in-the-parameter-list-of-bstree-print--is-it-required-)
    + [Explain memory leaks. Why are memory leaks bad?](#explain-memory-leaks-why-are-memory-leaks-bad-)
    + [What is the reason behind writing everything in English?](#what-is-the-reason-behind-writing-everything-in-english-)
    + [Why should you use static for non-exported functions?](#why-should-you-use-static-for-non-exported-functions-)
    + [Why should we comment our source code? Is it always needed? What should the comment state?](#why-should-we-comment-our-source-code--is-it-always-needed--what-should-the-comment-state-)
    + [What is self-documenting code?](#what-is-self-documenting-code-)
    + [Why should the module not output debug messages?](#why-should-the-module-not-output-debug-messages-)
    + [Why and when should you use assert?](#why-and-when-should-you-use-assert-)
    + [What are the benefits of using a Makefile over calling the compiler by hand?](#what-are-the-benefits-of-using-a-makefile-over-calling-the-compiler-by-hand-)
    + [Imagine it was your job to design the interface for the bstree module (bstree.h). What, if anything, would you have done differently, and why?](#imagine-it-was-your-job-to-design-the-interface-for-the-bstree-module--bstreeh--what--if-anything--would-you-have-done-differently--and-why-)

<small><i><a href='http://ecotrust-canada.github.io/markdown-toc/'>Table of contents generated with markdown-toc</a></i></small>

## Implementation notes

My implementation of the binary search tree is based on the chapters in 'Data Structures & Algorithms in Java' by Goodrich, Tamassia and Goldwasser ([Link](https://www.wiley.com/en-us/Data+Structures+and+Algorithms+in+Java%2C+6th+Edition-p-9781118771334))

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

### Test output:

```
➜  ex2 git:(main) ✗ make check_task1
cc -std=c11 -Wall -Werror -Wextra -O2 -g -c bstree.c -o bstree.o
cc -std=c11 -Wall -Werror -Wextra -O2 -g bstree.o task1_tests.c -o task1_tests
bash ./check_task.sh task1_tests
leaks(187,0x202e17e00) malloc: stack logs being written into /tmp/stack-logs.187.10aedd000.leaks.NuAraR.index
leaks(187,0x202e17e00) malloc: recording malloc and VM allocation stacks to disk using standard recorder
leaks(187,0x202e17e00) malloc: stack logging compaction turned off; size of log files on disk can increase rapidly
leaks(187,0x202e17e00) malloc: process 101 no longer exists, stack logs deleted from /tmp/stack-logs.101.10b0f1000.task2_tests.6Vyx5n.index
task1_tests(188,0x201476e00) malloc: stack logs being written into /tmp/stack-logs.188.1092c0000.task1_tests.3s0Wni.index
task1_tests(188,0x201476e00) malloc: recording malloc and VM allocation stacks to disk using standard recorder
task1_tests(188,0x201476e00) malloc: stack logging compaction turned off; size of log files on disk can increase rapidly
[ NIL ] : 0
[[3.12], 4.20, [77.70]] : 3
[[[[-3.14], 2.00], 3.12], 4.20, [[9.99], 77.70, [[123.45], 666.00, [999.99]]]] : 9
[[[[-3.14], 2.00], 3.12], 4.20, [[[5.50], 9.99], 77.70, [[123.45], 666.00, [999.99]]]] : 10
task1_tests(188,0x201476e00) malloc: stack logs deleted from /tmp/stack-logs.188.1092c0000.task1_tests.3s0Wni.index
leaks(189,0x200f56e00) malloc: stack logs being written into /tmp/stack-logs.189.109463000.leaks.UTRJna.index
leaks(189,0x200f56e00) malloc: recording malloc and VM allocation stacks to disk using standard recorder
leaks(189,0x200f56e00) malloc: stack logging compaction turned off; size of log files on disk can increase rapidly
Process:         task1_tests [188]
Path:            /Users/USER/*/task1_tests
Load Address:    0x100fb0000
Identifier:      task1_tests
Version:         0
Code Type:       X86-64 (translated)
Platform:        macOS
Parent Process:  leaks [187]

Date/Time:       2021-03-15 13:35:05.906 +0100
Launch Time:     2021-03-15 13:35:05.582 +0100
OS Version:      macOS 11.2.1 (20D74)
Report Version:  7
Analysis Tool:   /Applications/Xcode.app/Contents/Developer/usr/bin/leaks
Analysis Tool Version:  Xcode 12.4 (12D4e)

Physical footprint:         2061K
Physical footprint (peak):  2065K
----

leaks Report Version: 4.0, multi-line stacks
Process 188: 174 nodes malloced for 13 KB
Process 188: 0 leaks for 0 total leaked bytes.

leaks(189,0x200f56e00) malloc: stack logs deleted from /tmp/stack-logs.189.109463000.leaks.UTRJna.index
leaks(187,0x309b00000) malloc: stack logs deleted from /tmp/stack-logs.187.10aedd000.leaks.NuAraR.index

Results:
✔ task1_tests passed all tests.
✔ Output of task1_tests matches expected output.
✔ task1_tests does not have any memory-related bugs.


➜  ex2 git:(main) ✗ make check_task2
cc -std=c11 -Wall -Werror -Wextra -O2 -g bstree.o task2_tests.c -o task2_tests
bash ./check_task.sh task2_tests
leaks(228,0x20453fe00) malloc: stack logs being written into /tmp/stack-logs.228.10c619000.leaks.FmApZR.index
leaks(228,0x20453fe00) malloc: recording malloc and VM allocation stacks to disk using standard recorder
leaks(228,0x20453fe00) malloc: stack logging compaction turned off; size of log files on disk can increase rapidly
task2_tests(229,0x2007f3e00) malloc: stack logs being written into /tmp/stack-logs.229.108702000.task2_tests.vn7gl8.index
task2_tests(229,0x2007f3e00) malloc: recording malloc and VM allocation stacks to disk using standard recorder
task2_tests(229,0x2007f3e00) malloc: stack logging compaction turned off; size of log files on disk can increase rapidly
[ NIL ] : 0
[[3.12], 4.20, [77.70]] : 3
[3.12, [77.70]] : 2
leaks(228,0x20453fe00) malloc: stack logs deleted from /tmp/stack-logs.228.10c619000.leaks.FmApZR.index

Results:
✔ task2_tests passed all tests.
✔ Output of task2_tests matches expected output.
✔ task2_tests does not have any memory-related bugs.
➜  ex2 git:(main) ✗ 
```

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

I guess it's not defined so that the implementation is not restricted too much and is up to the student.

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

a static function is only visible to the file it's declared in.
If you don't want another programmer to use a function then you could declare it static so noone outside your module can use it.

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

