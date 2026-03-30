// Exemplo de programa que executa um comando via terminalcomo processo filho
// Criado em 29/03/2026
// Autor: Gustavo Martins França

#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("Nenhum comando recebido.\n");

        return 0;
    }

    pid_t pid_child;
    int value = 0;

    pid_child = fork();

    if (pid_child == 0) {
        char *cmd[argc];
        for (int i = 0; i < argc-1; i++) {
            cmd[i] = argv[i+1];
        }
        cmd[argc-1] = NULL;

        char path_cmd[9 + strlen(cmd[0])];
        sprintf(path_cmd, "/usr/bin/%s", cmd[0]);
        
        if (execv(path_cmd, cmd) == -1) {
            printf("Comando não existente\n");
            exit(1);
        }
    }

    else if (pid_child > 0) {        
        int status;
        pid_t pid_child_finalized;

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