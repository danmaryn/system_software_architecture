# Practical Work 5

## Task 5.v12
### Explanation
This task demonstrates a memory corruption issue caused by ignoring struct padding. When `memcpy` is used to copy packed data into an aligned structure, the payload spills into the compiler-generated padding bytes. As a result, the actual variables receive garbage data (the integer `i` becomes `-256` instead of the expected `42`). 

The Valgrind analysis further illustrates that this type of stack-based logical memory corruption does not trigger heap errors, out-of-bounds alerts, or memory leaks (`0 errors from 0 contexts`). This highlights how padding overwrites can silently corrupt application state while passing standard memory debugging tools.

### Result
```text
ltsvtbcvtmcastr@ltsvtbcvtmcastr:~/SSA/pz5$ ./task5_v12
--- Padding Overwrite Demonstration ---
Expected: c = 'A', i = 42
Actual:   c = 'A', i = -256 (0xFFFFFF00)

Raw memory dump of dest (8 bytes):
[0]: 41  <- char 'c'
[1]: 2A  <- padding (overwritten!)
[2]: 00  <- padding (overwritten!)
[3]: 00  <- padding (overwritten!)
[4]: 00  <- int 'i' (start)
[5]: FF  <- int 'i'
[6]: FF  <- int 'i'
[7]: FF  <- int 'i'
ltsvtbcvtmcastr@ltsvtbcvtmcastr:~/SSA/pz5$ valgrind ./task5_v12
==91861== Memcheck, a memory error detector
==91861== Copyright (C) 2002-2022, and GNU GPL'd, by Julian Seward et al.
==91861== Using Valgrind-3.22.0 and LibVEX; rerun with -h for copyright info
==91861== Command: ./task5_v12
==91861== 
--- Padding Overwrite Demonstration ---
Expected: c = 'A', i = 42
Actual:   c = 'A', i = -256 (0xFFFFFF00)

Raw memory dump of dest (8 bytes):
[0]: 41  <- char 'c'
[1]: 2A  <- padding (overwritten!)
[2]: 00  <- padding (overwritten!)
[3]: 00  <- padding (overwritten!)
[4]: 00  <- int 'i' (start)
[5]: FF  <- int 'i'
[6]: FF  <- int 'i'
[7]: FF  <- int 'i'
==91861== 
==91861== HEAP SUMMARY:
==91861==     in use at exit: 0 bytes in 0 blocks
==91861==   total heap usage: 1 allocs, 1 frees, 1,024 bytes allocated
==91861== 
==91861== All heap blocks were freed -- no leaks are possible
==91861== 
==91861== For lists of detected and suppressed errors, rerun with: -s
==91861== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
```