#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>

int main() {
    pid_t pid_child;
    int value = 0;

    // Cria um processo filho com uma cópia do programa atual,
    // executando a partir dessa linha de código.
    // Para o pai, retorna o PID; Para o filho, retorna 0.
    // Se não foi possível criar um novo processo, retorna -1.
    pid_child = fork();

    // Bloco de código executado no processo filho
    if (pid_child == 0) {
        printf("Sou o processo filho %d; Pai:   %d\n", getpid(), getppid());
        
        // Finaliza o processo com código 0
        exit(0);
    }

    // Bloco de código executado no processo pai
    else if (pid_child > 0) {
        printf("Sou o processo pai   %d; Filho: %d\n", getpid(), pid_child);
        
        int status;
        pid_t pid_child_finalized;

        // Espera o processo filho retornar alguma coisa
        pid_child_finalized = wait(&status);

        if (WIFEXITED(status)) {
            printf("Filho %d retornou com código %d\n", pid_child_finalized, WEXITSTATUS(status));
        }
    }

    else {
        printf("Não foi possível criar um novo processo\n");
    }

    return 0;
}