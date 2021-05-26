# Task1
## nm and objdump

### nm:

```bash
0000000000003da8 d _DYNAMIC
0000000000003fa8 d _GLOBAL_OFFSET_TABLE_
0000000000002000 R _IO_stdin_used
                 w _ITM_deregisterTMCloneTable
                 w _ITM_registerTMCloneTable
0000000000002164 r __FRAME_END__
000000000000201c r __GNU_EH_FRAME_HDR
0000000000004010 D __TMC_END__
0000000000004010 B __bss_start
                 w __cxa_finalize@@GLIBC_2.2.5
0000000000004000 D __data_start
00000000000011a0 t __do_global_dtors_aux
0000000000003da0 d __do_global_dtors_aux_fini_array_entry
0000000000004008 D __dso_handle
0000000000003d98 d __frame_dummy_init_array_entry
                 w __gmon_start__
0000000000003da0 d __init_array_end
0000000000003d98 d __init_array_start
0000000000001260 T __libc_csu_fini
00000000000011f0 T __libc_csu_init
                 U __libc_start_main@@GLIBC_2.2.5
                 U __printf_chk@@GLIBC_2.3.4
0000000000004010 D _edata
0000000000004018 B _end
0000000000001268 T _fini
0000000000001000 t _init
0000000000001100 T _start
0000000000004010 b completed.8060
0000000000004000 W data_start
0000000000001130 t deregister_tm_clones
00000000000011e0 t frame_dummy
00000000000010a0 T main
0000000000001160 t register_tm_clones
                 U strtol@@GLIBC_2.2.5
                 U this_function_should_increment_a_number_by_ten
```

int the last line, the function 'this_function_should_increment_a_number_by_ten' is called from the main function of 'some_program'

### objdump

Here we can also see, that in the `main` function, the function 'this_function_should_increment_a_number_by_ten' is used

```bash
➜ objdump --disassemble some_program --section=.text

some_program:   file format elf64-x86-64


Disassembly of section .text:

00000000000010a0 <main>:
    10a0: f3 0f 1e fa                   endbr64
    10a4: 48 83 ec 08                   subq    $8, %rsp
    10a8: 83 ff 02                      cmpl    $2, %edi
    10ab: 74 20                         je      0x10cd <main+0x2d>
    10ad: 48 8b 16                      movq    (%rsi), %rdx
    10b0: bf 01 00 00 00                movl    $1, %edi
    10b5: 31 c0                         xorl    %eax, %eax
    10b7: 48 8d 35 46 0f 00 00          leaq    3910(%rip), %rsi  # 2004 <_IO_stdin_used+0x4>
    10be: e8 cd ff ff ff                callq   0x1090 <this_function_should_increment_a_number_by_ten+0x1090>
    10c3: b8 01 00 00 00                movl    $1, %eax
    10c8: 48 83 c4 08                   addq    $8, %rsp
    10cc: c3                            retq
    10cd: 48 8b 7e 08                   movq    8(%rsi), %rdi
    10d1: ba 0a 00 00 00                movl    $10, %edx
    10d6: 31 f6                         xorl    %esi, %esi
    10d8: e8 a3 ff ff ff                callq   0x1080 <this_function_should_increment_a_number_by_ten+0x1080>
    10dd: 48 89 c7                      movq    %rax, %rdi
    10e0: e8 8b ff ff ff                callq   0x1070 <this_function_should_increment_a_number_by_ten+0x1070>
    10e5: 48 8d 35 2c 0f 00 00          leaq    3884(%rip), %rsi  # 2018 <_IO_stdin_used+0x18>
    10ec: bf 01 00 00 00                movl    $1, %edi
    10f1: 89 c2                         movl    %eax, %edx
    10f3: 31 c0                         xorl    %eax, %eax
    10f5: e8 96 ff ff ff                callq   0x1090 <this_function_should_increment_a_number_by_ten+0x1090>
    10fa: 31 c0                         xorl    %eax, %eax
    10fc: eb ca                         jmp     0x10c8 <main+0x28>
    10fe: 66 90                         nop
```

## Questions:

 - Is some_library.so the only shared library used by some_program? Use the ldd utility to check.
   - No, it is not:
   ```bash
    -> ldd ./some_program 
        linux-vdso.so.1 =>  (0x00007ffe80317000)
        some_library.so => not found
        libc.so.6 => /lib64/libc.so.6 (0x00007f5be84e2000)
        /lib64/ld-linux-x86-64.so.2 (0x00007f5be88b0000)
   ```
 - Compare the sizes of some_program and my_program - what do you find?
   - my_program is slightly smaller than some_program:
   ```bash
    -> ls -la
        total 68
        drwxr-xr-x. 2 csaw4225 lxusers  4096 May 26 18:36 .
        drwxr-xr-x. 5 csaw4225 lxusers  4096 May 26 17:41 ..
        -rw-r--r--. 1 csaw4225 lxusers   209 May 26 18:36 Makefile
        -rw-r--r--. 1 csaw4225 lxusers  3946 May 26 18:30 README.md
        -rwxr-xr-x. 1 csaw4225 lxusers 12208 May 26 18:36 my_program
        -rw-r--r--. 1 csaw4225 lxusers   366 May 26 18:36 my_program.c
        -rw-r--r--. 1 csaw4225 lxusers    78 May 26 17:41 some_library.c
        -rwxr-xr-x. 1 csaw4225 lxusers  8936 May 26 18:36 some_library.so
        -rwxr-xr-x. 1 csaw4225 lxusers 16824 May 26 17:41 some_program
   ```
 - What is the difference between dynamic and static linking?
   - dynamic: linking occurs during runtime
   - static: linking occurs during compilation (last step of compilation)

 - When would you use dynamic linking?
   - when you want to use a library as a sort of 'plugin' in your code (you dont exactly know where your function is going to be but you know how it looks like)
 - When would you use static linking?
   - when you have all the recources you need to include your library as a static link
 - What is position independent code, and why is it useful?
   - Position indipendent code indicates that the storage location of a code piece does not affect it's functionality
   - Example:
   ```arm
   # ADRESS: COMMAND
   # this would not work if this command was at position 150
   120: JUMP TO 123
   ...
   123: SOME MORE COMMANDS

   # this would work if this command was at position 150 (PIC)
   120: JUMP TO CUR_POSITION + 3
   ...
   123: SOME MORE COMMANDS
   ```
 - What is the purpose of the LD_LIBRARY_PATH environment variable?
   - With this variable paths can be defined in which libraries are searched for.
   - These libraries then have a higher priority than the standard libraries