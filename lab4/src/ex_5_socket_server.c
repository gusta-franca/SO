// Exemplo de programa que atua como servidor em um canal criado via socket
// Criado em 04/05/2026
// Autor: Gustavo Martins França

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define CONNECTIONS 10

char *socket_path = "socket_server";

char* lang_codes[] = {"en", "es", "pt"};
int num_langs = 3;

char* en_dict[] = {"receive", "door", "bottle", "computer", "mouse", "chair", "table", "phone", "thash", "keyboard"};
char* es_dict[] = {"ricibir", "puerta", "jarra", "computer", "mouse", "silla", "mesa", "teléfono", "basura", "teclado"};
char* pt_dict[] = {"receber", "porta", "garrafa", "computador", "mouse", "cadeira", "mesa", "telefone", "lixo", "teclado"};
char** dicts[] = {en_dict, es_dict, pt_dict};

char* lang_error = "Language not supported";
char* word_error = "Word not found";

int main() {
    int server_socket,       
        client_socket,       
        received_bytes;
    
    struct sockaddr_un addr; 
    char buf[256];
    char word[250];
    // Vetor para armazenar as linguagens recebidas
    char langs[3][3];

    if ((server_socket = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
        perror("socket: server");
        exit(-1);
    }

    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;

    strncpy(addr.sun_path, socket_path, sizeof(addr.sun_path)-1);
    // Desvincular path se já existe socket associado
    unlink(socket_path);

    if (bind(server_socket, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
        perror("bind: server");
        exit(-1);
    }

    if (listen(server_socket, CONNECTIONS) == -1) {
        perror("listen: server");
        exit(-1);
    }

    while (1) {
	    // Aguarda conexões dos clientes
        if ((client_socket = accept(server_socket, NULL, NULL)) == -1) {
            perror("accept: server");
            continue;
        }

        // Lê dados envidos pelos clientes
        while ((received_bytes = read(client_socket, buf, sizeof(buf))) > 0) {
            strncpy(langs[0], buf, 2);
            langs[0][2] = '\0';

            strncpy(langs[1], buf+3, 2);
            langs[1][2] = '\0';

            sscanf(buf+6, "%s", word);

            int src_idx = -1, tgt_idx = -1;
            for (int i = 0; i < 3; i++) {
                if (strcmp(langs[0], lang_codes[i]) == 0) {
                    src_idx = i;
                }
                if (strcmp(langs[1], lang_codes[i]) == 0) {
                    tgt_idx = i;
                }
            }

            if (src_idx == -1 || tgt_idx == -1) {
                write(client_socket, lang_error, strlen(lang_error));
                continue;
            }

            int found = 0;
            for (int i = 0; i < 10; i++) {
                if (strcmp(word, dicts[src_idx][i]) == 0) {
                    char* translation = dicts[tgt_idx][i];
                    write(client_socket, translation, strlen(translation));
                    found = 1;
                    break;
                }
            }

            if (!found) {
                write(client_socket, word_error, strlen(word_error));
            }
        }
        
        if (received_bytes == -1) {
            perror("read");
            exit(-1);
        } else if (received_bytes == 0) {
            printf("EOF - closing client socket\n");
            close(client_socket);
        }
    }

    close(server_socket);

    return 0;
}   
