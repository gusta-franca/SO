// Exemplo de programa que usa memória compartilhada entre processos em uma hierarquia de modo que seja realizada a soma de dois vetores
// Criado em 04/05/2026
// Autor: Gustavo Martins França

#include <ctype.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

// Índices para cada processo filho
struct indexes {
    int index_start;
    int index_end;
};

// Preenche um vetor com valores inteiros semi-aleatórios
void fill_vector_random(int *v, int n);

// Imprime todo o conteúdo do vetor
void print_vector(int* v, int n);

int main(int argc, char** argv) {
    int num_elements = 20;
    int num_processes = 5;

    if (argc > 1) {
        num_elements = atoi(argv[1]);
    }

    if (argc > 2) {
        num_processes = atoi(argv[2]);
    }

    // Memória compartilhada
    const char* name = "/shm_sumvec";
    int size_total = sizeof(int) * (3*num_elements + num_processes);
    int shm_fd;
    void* shm_ptr;

    shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
    ftruncate(shm_fd, size_total);
    
    shm_ptr = mmap(NULL, size_total, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (shm_ptr == MAP_FAILED) {
        printf("Map failed\n");
        return -1;
    }

    int* v_1 = (int*)shm_ptr;
    int* v_2 = v_1 + num_elements;
    int* v_res = v_2 + num_elements;
    int* v_sig = v_res + num_elements;
    
    fill_vector_random(v_2, num_elements);
    fill_vector_random(v_1, num_elements);
    for (int i = 0; i < num_elements; i++) {
        v_res[i] = 0;
    }
    for (int i = 0; i < num_processes; i++) {
        v_sig[i] = 0;
    }

    int size_subvector = num_elements / num_processes;
    int index_start = 0;
    int index_end = size_subvector;

    int pipes[num_processes][2];
    pid_t pid;
    
    // Pai cria os pipes para comunicação com os filhos
    for (int j = 0; j < num_processes; j++) {
        if (pipe(pipes[j])) {
            fprintf(stderr, "Falha ao criar o pipe.\n");
            return EXIT_FAILURE;
        }
        
        pid = fork();
        
        if (pid < 0) {
            fprintf(stderr, "fork");
        }
        else if (pid > 0) {
            struct indexes range;
            range.index_start = index_start;
            range.index_end = index_end;

            index_start += size_subvector;
            index_end += size_subvector;

            if (j == num_processes-1) {
                index_end = num_elements-1;
            }
            
            close(pipes[j][0]);
            write(pipes[j][1], &range, sizeof(struct indexes));
        }
        else {
            struct indexes range;

            int fd = pipes[j][0];
            close(pipes[j][1]);
            read(fd, &range, sizeof(struct indexes));
            
            for (int i = range.index_start; i < range.index_end; i++) {
                v_res[i] = v_1[i] + v_2[i];
            }

            v_sig[j] = 1;

            exit(0);
        }
    }

    for (int i = 0; i < num_processes; i++) {
        wait(0);
    }

    for (int i = 0; i < num_processes; i++) {
        if (v_sig[i] != 1) {
            perror("invalid result");
            return -1;
        }
    }

    printf("v_1 = ");
    print_vector(v_1, num_elements);
    printf("v_2 = ");
    print_vector(v_2, num_elements);
    printf("v_r = ");
    print_vector(v_res, num_elements);

    munmap(shm_ptr, size_total);
    unlink(name);

    return 0;
}

void fill_vector_random(int *v, int n) {
    for (int i = 0; i < n; i++) {
        v[i] = rand() % n;
    }
}

void print_vector(int* v, int n) {
    printf("[");
    for (int i = 0; i < n; i++) {
        printf("%3d ", v[i]);
    }
    printf("]\n");
}
