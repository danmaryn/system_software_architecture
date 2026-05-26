#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <elf.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <executable>\n", argv[0]);
        return 1;
    }

    int fd = open(argv[1], O_RDONLY);
    if (fd < 0) {
        printf("Failed to open file\n");
        return 1;
    }

    struct stat st;
    if (fstat(fd, &st) < 0) {
        printf("Failed to get file stats\n");
        close(fd);
        return 1;
    }

    void *mem = mmap(NULL, (size_t)st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (mem == MAP_FAILED) {
        printf("mmap failed\n");
        close(fd);
        return 1;
    }

    Elf64_Ehdr *ehdr = (Elf64_Ehdr *)mem;

    if (memcmp(ehdr->e_ident, ELFMAG, SELFMAG) != 0) {
        printf("Not an ELF file\n");
        munmap(mem, (size_t)st.st_size);
        close(fd);
        return 1;
    }

    printf("ELF loader started\n");
    printf("Entry point: 0x%lx\n", (unsigned long)ehdr->e_entry);
    printf("Program headers count: %d\n\n", ehdr->e_phnum);

    Elf64_Phdr *phdr = (Elf64_Phdr *)((unsigned char *)mem + ehdr->e_phoff);

    for (int i = 0; i < ehdr->e_phnum; i++) {
        if (phdr[i].p_type == PT_LOAD) {
            printf("Mapping PT_LOAD segment\n");
            printf("Virtual address: 0x%lx\n", (unsigned long)phdr[i].p_vaddr);
            printf("Memory size: %lu bytes\n\n", (unsigned long)phdr[i].p_memsz);
        }
    }

    munmap(mem, (size_t)st.st_size);
    close(fd);
    return 0;
}