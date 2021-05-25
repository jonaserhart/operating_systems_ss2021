## Notes

### Structure

Task1:
 - allocator1.h / allocator1.c

Task2:
 - allocator2.h / allocator1.c

Other:
 - allocator_test.c
   - Runs membench
 - dbg.h
   - debug macros
 - list.c / list.h
   - my implementation of a linked list

### Make and run

The commands

```bash
$ make
$ ./alloc_test
```
run membench_global for the allocators for task 1 & 2

### Memory leaks

i have a memory leak in 'allocator1.c' and i could not figure out where it came from

```c
/**
leaks Report Version: 4.0
Process 11595: 262364 nodes malloced for 137232 KB
Process 11595: 0 leaks for 0 total leaked bytes.

BUT i don't know how to get rid of this error:
alloc_test(15586,0x1027d3d40) malloc: *** error for object 0x154800010: pointer being freed was not
allocated

if i don't free the mempool pointer i get a leak of 128M, but if i do, i get this error
*/
void my_allocator_destroy() {
    ...
```

