// Exemplo de programa que gera processos filho indefinidamente
// Criado em 29/03/2026
// Autor: Gustavo Martins França

#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>

void create_process_recursive() {
    pid_t pid_child;
    int value = 0;

    pid_child = fork();

    if (pid_child < 0) {
        printf("fork error\n");
    } 

    if (pid_child > 0) {
        printf("New child: %d\n", pid_child);
    }

    create_process_recursive();
}

int main() {
    create_process_recursive();

    return 0;
}
