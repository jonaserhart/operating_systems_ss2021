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

in der letzten Zeile kann ausgelesen werden, dass die gesuchte funktion den namen 'this_function_should_increment_a_number_by_ten' hat.

### objdump

Auch hier in der `main` funktion ist zu erkennen, dass dort die funktion 'this_function_should_increment_a_number_by_ten' verwendet wird

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