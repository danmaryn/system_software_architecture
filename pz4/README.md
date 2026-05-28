# Practical Work 4

## Task 4.1
### Explanation
This test demonstrates the maximum memory size that `malloc(3)` can theoretically allocate. On a 64-bit system, while `size_t` allows values up to 16 exabytes ($2^{64}-1$), the actual limit is restricted by `ptrdiff_t` (a signed type required for pointer arithmetic), which caps the maximum object size at 8 exabytes ($2^{63}-1$). On a 32-bit system, it is capped at 2 GB. Expectedly, allocating this maximum size fails due to hardware constraints.

### Result
```text
ltsvtbcvtmcastr@ltsvtbcvtmcastr:~/SSA/pz4$ ./task4_1_64
Platform: 64-bit
Max size_t: 18446744073709551615
Max ptrdiff_t: 9223372036854775807
malloc(SIZE_MAX) failed.
ltsvtbcvtmcastr@ltsvtbcvtmcastr:~/SSA/pz4$ ./task4_1_32
Platform: 32-bit
Max size_t: 4294967295
Max ptrdiff_t: 2147483647
malloc(SIZE_MAX) failed.
```

---

## Task 4.2
### Explanation
This experiment highlights integer overflow when calculating memory allocation size. Multiplying two large signed integers results in a negative value. When this negative value is implicitly cast to the unsigned `size_t` type required by `malloc`, it transforms into a massive positive number (near 18 exabytes), causing the allocation to fail.

### Result
```text
ltsvtbcvtmcastr@ltsvtbcvtmcastr:~/SSA/pz4$ ./task4_2
xa = 1000000, xb = 3000
num (signed int) = -1294967296
num cast to size_t = 18446744072414584320
malloc failed: cannot allocate memory.
```

---

## Task 4.3
### Explanation
This test explores the behavior of `malloc(0)`. Under Linux (glibc), requesting 0 bytes does not return `NULL`. Instead, it allocates a minimum-sized memory block (for internal metadata) and returns a unique, valid pointer. Passing this pointer to `free()` is perfectly safe and valid.

### Result
```text
ltsvtbcvtmcastr@ltsvtbcvtmcastr:~/SSA/pz4$ ltrace ./task4_3
__libc_start_main(0xbfb2ea780858, 1, 0xfffff25296c8, 0 <unfinished ...>
malloc(0)                                                                                 = 0xbfb2faaa72a0
printf("malloc(0) returned a non-NULL po"..., 0xbfb2faaa72a0malloc(0) returned a non-NULL pointer: 0xbfb2faaa72a0
)                                       = 54
free(0xbfb2faaa72a0)                                                                      = <void>
puts("Memory freed successfully."Memory freed successfully.
)                                                          = 27
__cxa_finalize(0xbfb2ea7a0008, 0xbfb2ea780800, 1, 568)                                    = 1
+++ exited (status 0) +++
```

---

## Task 4.4
### Explanation
This task demonstrates a classic Use-After-Free and Double-Free vulnerability. In the buggy version, a freed pointer is not reset to `NULL` (becoming a dangling pointer), causing the program to skip allocation on the next loop iteration, attempt to reuse the freed memory, and crash upon a second `free()`. The fixed version safely resets the pointer to `NULL` immediately after freeing it.

### Result
```text
ltsvtbcvtmcastr@ltsvtbcvtmcastr:~/SSA/pz4$ ./task4_4 fix
--- Running FIXED version ---
Allocated memory at: 0xbe97b07076b0
Using ptr at: 0xbe97b07076b0
Calling free(0xbe97b07076b0)...
Allocated memory at: 0xbe97b07076b0
Using ptr at: 0xbe97b07076b0
Calling free(0xbe97b07076b0)...
ltsvtbcvtmcastr@ltsvtbcvtmcastr:~/SSA/pz4$ ./task4_4 bug
--- Running BUGGY version ---
Allocated memory at: 0xadc9e48176b0
Using ptr at: 0xadc9e48176b0
Calling free(0xadc9e48176b0)...
ptr is NOT NULL (holds 0xadc9e48176b0), skipping malloc!
Using ptr at: 0xadc9e48176b0
Calling free(0xadc9e48176b0)...
free(): double free detected in tcache 2
Aborted (core dumped)
```

---

## Task 4.5
### Explanation
This program illustrates the safe pattern for using `realloc`. If `realloc` fails (e.g., when requesting an impossibly large block), it returns `NULL`, but the original memory block remains intact. By assigning the result to a temporary pointer instead of overwriting the original one, we preserve access to the original memory block and prevent memory leaks.

### Result
```text
ltsvtbcvtmcastr@ltsvtbcvtmcastr:~/SSA/pz4$ ./task4_5
Initial memory allocated at: 0xaf7ebc7d82a0
Attempting to realloc to 18446744073709551615 bytes...
realloc failed and returned NULL.
Original pointer (ptr) is still valid: 0xaf7ebc7d82a0
Original memory freed successfully to prevent memory leak.
```

---

## Task 4.6
### Explanation
This test evaluates edge cases for `realloc`. When passed `NULL` as the original pointer (`realloc(NULL, size)`), it behaves exactly like `malloc`, allocating a new block. When passed `0` as the size (`realloc(ptr, 0)`), it behaves like `free`, successfully releasing the memory block and returning `NULL`.

### Result
```text
ltsvtbcvtmcastr@ltsvtbcvtmcastr:~/SSA/pz4$ ./task4_6
--- Test 1: realloc(NULL, size) ---
realloc(NULL, 64) successfully allocated memory at: 0xbe99b72ba6b0

--- Test 2: realloc(ptr, 0) ---
Initial allocation at: 0xbe99b72ba700
realloc(ptr, 0) returned NULL. Memory at 0xbe99b72ba700 was freed.
```

---

## Task 4.7
### Explanation
This task highlights the security benefits of `reallocarray` over standard `realloc`. While traditional `realloc` requires manual multiplication of elements and size (which is vulnerable to integer overflows), `reallocarray` handles the multiplication internally and safely fails if an overflow is detected. The `ltrace` output confirms how the sizes are passed differently under the hood.

### Result
```text
ltsvtbcvtmcastr@ltsvtbcvtmcastr:~/SSA/pz4$ ltrace ./task4_7 old
__libc_start_main(0xb4fe814609c4, 2, 0xffffe69fc298, 0 <unfinished ...>
strcmp("old", "old")                                                                      = 0
calloc(1000, 16)                                                                          = 0xb4feb4bf12a0
realloc(0xb4feb4bf12a0, 8000)                                                             = 0xb4feb4bf12a0
free(0xb4feb4bf12a0)                                                                      = <void>
__cxa_finalize(0xb4fe81480008, 0xb4fe814608c0, 1, 568)                                    = 1
+++ exited (status 0) +++
ltsvtbcvtmcastr@ltsvtbcvtmcastr:~/SSA/pz4$ ltrace ./task4_7 new
__libc_start_main(0xb37ebeb109c4, 2, 0xffffc2ebc2b8, 0 <unfinished ...>
strcmp("new", "old")                                                                      = -1
strcmp("new", "new")                                                                      = 0
calloc(1000, 16)                                                                          = 0xb37ebf6222a0
reallocarray(0xb37ebf6222a0, 500, 16, 0xb37ebf6260c0)                                     = 0xb37ebf6222a0
free(0xb37ebf6222a0)                                                                      = <void>
__cxa_finalize(0xb37ebeb30008, 0xb37ebeb108c0, 1, 568)                                    = 1
+++ exited (status 0) +++
```

---

## Task 4.v12
### Explanation
This routine investigates how `realloc()` handles block expansion. It demonstrates that when a block is expanded, `realloc` may copy it to a new location due to glibc optimizations. Furthermore, memory freed from `Block A` is efficiently reused for `Block B`. When `Block B` is expanded, the presence of an adjacent allocated `Guard block` physically prevents in-place expansion, forcing `realloc` to allocate a new memory region and copy the data.

### Result
```text
ltsvtbcvtmcastr@ltsvtbcvtmcastr:~/SSA/pz4$ ./task4_v12
Block A allocated at: 0xc70147b802a0
Block A expanded to 32 bytes at: 0xc70147b806d0

Block B allocated at: 0xc70147b802a0
Guard block allocated at: 0xc70147b80700
Block B expanded to 1024 bytes at: 0xc70147b80720
```