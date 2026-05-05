#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <unistd.h>

struct Livro {
    char titulo[40];
    char autor[30];
    int num_paginas;
};

int main() {
    const int SIZE = sizeof(struct Livro);
    const char* name = "/shm_Livro";
    int shm_fd;
    void* shm_ptr;

    shm_fd = shm_open(name, O_RDWR, 0666);
    ftruncate(shm_fd, SIZE);

    shm_ptr = mmap(NULL, SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);
    if (shm_ptr == MAP_FAILED) {
        printf("mmap\n");
        
        return -1;
    }

    struct Livro* livro = (struct Livro*)shm_ptr;

    printf("Título: %s,\nAutor: %s,\nPáginas: %d\n", livro->titulo, livro->autor, livro->num_paginas);

    if (shm_unlink(name) == -1) {
        printf("shm_unlink\n");
        return -1;
    }
    
    return 0;
}
