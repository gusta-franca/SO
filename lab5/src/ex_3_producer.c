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

    shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
    ftruncate(shm_fd, SIZE);

    shm_ptr = mmap(NULL, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (shm_ptr == MAP_FAILED) {
        printf("Map failed\n");
        
        return -1;
    }

    char titulo[40] = "O Livro da Ciência";
    char autor[30] = "Vários Autores";
    int num_paginas = 966;

    struct Livro livro;
    strcpy(livro.titulo, titulo);
    strcpy(livro.autor, autor);
    livro.num_paginas = num_paginas;

    memcpy(shm_ptr, &livro, SIZE);
    
    return 0;
}
