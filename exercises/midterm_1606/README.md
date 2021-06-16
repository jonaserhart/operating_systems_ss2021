
# Midterm exam 16.06.2021

## Notes

### Usage

> `$ ./restaurant <enable notifications> <number of guests> <number of cooks>`

### Files

`dbg.h`:
  - contains debug macros
  - used in my other exercises as well
  - originated out of a book from 'Zed. A. Shaw' 'Learning C the hard way', but was modified by me, original: [dbg.h](https://github.com/zedshaw/learn-c-the-hard-way-lectures/blob/master/dbg.h)

`.clang-format`:
  - keeps format in check
  - source: [github uibk](https://github.com/uibk-dps-teaching/ps_os_2021/blob/master/.clang-format )

`Makefile`:
  - makefile i prepaired in advance (no source, wrote that all by myself) 
  - commands:
    - `all`: make all `.c` files into object files for production (first entry, so just typing `make` will suffice)
    - `dev`: compile all `.c` files into object files without the `NDEBUG` flag for debugging with `dbg.h`
    - `clean`: cleans all object files and `*.dSYM` files that apple likes to make
    - `cleansub`: clean up the submission folder
    - `cleanall` : `clean` and `cleansub` 
    - `check`: helper for discovering dangerous (string) functions
    - `zip`: create a zip file with a timestamp of all relevant files for submission
    - `leakdoc`: (for macOS users) runs 'leaks' and saves the output in a file called 'leakdoc.txt'
    - `valgrinddoc` (for linux users) runs 'valgrind' and saves the output in a file called 'leakdoc.txt'

