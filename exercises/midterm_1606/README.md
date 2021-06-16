
# Midterm exam 16.06.2021

## Notes

### Usage

> `$ ./file <arg1> <arg2>`

### Files

`dbg.h`:
  - contains debug macros
  - used in my other exercises as well
  - originated out of a book from 'Zed. A. Shaw' 'Learning C the hard way', but was modified by me

`.clang-format`:
  - keeps format in check

`Makefile`:
  - commands:
    - `all`: make all `.c` files into object files for production
    - `dev`: compile all `.c` files into object files without the `NDEBUG` flag for debugging with `dbg.h`
    - `clean`: cleans all object files and `*.dSYM` files that apple likes to make
    - `cleansub`: clean up the submission folder
    - `cleanall` : `clean` and `cleansub` 
    - `check`: helper for discovering dangerous (string) functions
    - `zip`: create a zip file with a timestamp of all relevant files for submission

## Known Bugs ordered by severity

### 1) Example bug:

- Description  
description

- Possible cause:  
possible cause
