// Exemplo de programa que atua como cliente em um canal criado via socket
// Criado em 05/05/2026
// Autor: Gustavo Martins França

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

char* socket_path = "socket_server";

int main() {
    int client_socket,       
        sent_bytes,
        received_bytes;

    struct sockaddr_un addr; 
    char buf[256];

    if ((client_socket = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
        perror("socket: client");
        exit(-1);
    }

    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;

    strncpy(addr.sun_path, socket_path, sizeof(addr.sun_path)-1);

    if (connect(client_socket, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        perror("connect: client");
        exit(-1);
    }

    printf("> ");
    fflush(stdout);

    // Lê dados do terminal e envia para o servidor via sockets
    while ((sent_bytes = read(STDIN_FILENO, buf, sizeof(buf))) > 0) {
        if (buf[2] != '-' || buf[5] != ':') {
            printf("use the format 'xx-yy:word\n");
        }
        else {
            // Caso sejam escritos menos bytes que o pedido (buffer cheio no servidor)
            if (write(client_socket, buf, sent_bytes) != sent_bytes) {
                if (sent_bytes > 0) {
                    fprintf(stderr, "partial write");
                }
                else {
                    perror("write: client");
                    exit(-1);
                }
            }

            if ((received_bytes = read(client_socket, buf, sizeof(buf))) > 0) {
                buf[received_bytes] = '\0';
                printf("Server: %s", buf);
            }
        }

        printf("\n> ");
        fflush(stdout);
    }

    close(client_socket);

    return 0;
}
