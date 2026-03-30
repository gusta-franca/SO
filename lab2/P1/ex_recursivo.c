#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>

void create_process_recursive() {
    pid_t pid_child;
    int value = 0;

    // O processo atual é copiado, e chama a função recursivamente. 
    // O processo filho também chama a função recursivamente,
    // assim criando um novo processo, totalizando 2.
    // Os 2 novos processos criaram 2 novos processos,
    // e os 4 chamaram a função recursivamente, e assim por diante.
    pid_child = fork();
    create_process_recursive();
}

int main() {
    create_process_recursive();

    return 0;
}