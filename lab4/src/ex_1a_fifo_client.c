// Exemplo de programa que atua como cliente de um canal FIFO
// Criado em 03/05/2026
// Autor: Gustavo Martins França

#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define SERVER_FIFO "/tmp/server_fifo"

int main() {
    int fd_server;
    char buffer_input[256];

    // Abre o FIFO em modo de escrita
    if ((fd_server = open(SERVER_FIFO, O_WRONLY)) == -1) {
        perror("open");
        return 1;
    }

    while (1) {   
        memset(buffer_input, '\0', sizeof(buffer_input));

        printf("client> ");
        fgets(buffer_input, sizeof(buffer_input), stdin);

        if ((write(fd_server, buffer_input, strlen(buffer_input)+1)) == -1) {
            printf("\nNo server, closing fifo\n\n");
            
            close(fd_server);
            break;
        }
    }

    return 0;
}
