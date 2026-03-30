#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>

// Cria uma hierarquia de processos com n níveis
void create_process_hierarchy(int n) {
    // Caso base
    if (n <= 1) {
        return;
    }

    int num_child = 2;
    pid_t pid_child;

    for (int i = 0; i < num_child; i++) {
        pid_child = fork();

        if (pid_child < 0) {
            printf("Não foi possível criar um novo processo\n");

            exit(-1);
        }

        if (pid_child == 0) {
            // printf("Sou o processo filho %d; Pai:   %d\n", getpid(), getppid());
            
            create_process_hierarchy(n-1);

            exit(n);
        }
    }
        
    int status;
    pid_t pid_child_finalized;

    // Espera todos os processos filho finalizarem
    for (int i = 0; i < num_child; i++) {
        pid_child_finalized = wait(&status);

        if (WIFEXITED(status)) {
            // printf("Filho %d retornou com código %d\n", pid_child_finalized, WEXITSTATUS(status));
            printf("Pai %d, Filho %d, n = %d\n", getpid(), pid_child_finalized, WEXITSTATUS(status));
        }
    }
}

int main(int argc, char **argv) {
    int n = 3;

    if (argc > 1) {
        n = atoi(argv[1]);
    }

    create_process_hierarchy(n);
    
    return 0;
}