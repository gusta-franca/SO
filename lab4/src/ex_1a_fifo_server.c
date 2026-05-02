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

// Estrutura para passar os parâmetros para as threads
struct data_chunk {
    char* texto;
};

// Thread para contar palavras e espaços de um trecho de texto em param
void* thread_conta_palavras_espacos(void* param);

// Thread para contar vogais e consoantes de um trecho de texto em param
void* thread_conta_vogais_consoantes(void* param);

int main() {
    int fd_server, num_bytes_read;
    char buffer[256];

    // cria um FIFO se inexistente com as permissões rwx rwx r--
    if (mkfifo(SERVER_FIFO, 0664) == -1 && (errno != EEXIST)) {
        perror("mkfifo");
        return 1;
    }

    // Abre o FIFO em modo de leitura
    if ((fd_server = open(SERVER_FIFO, O_RDONLY)) == -1) {
        perror("open");
        return 1;
    } 

    while (1) {   
        memset(buffer, '\0', sizeof(buffer));

        num_bytes_read = read(fd_server, buffer, sizeof(buffer)-1);
        switch (num_bytes_read) {
        case -1:
            perror("read error");
            break;
        case 0:
            printf("\n-- No data, reopening fifo --\n\n");

            close(fd_server);
            if ((fd_server = open(SERVER_FIFO, O_RDONLY)) == -1) {
                perror("open");
                return 1;
            }
            break;  
        default:
            printf("lido: %s\ntamanho: %zu\n", buffer, strlen(buffer)-1);

            struct data_chunk dados;
            dados.texto = buffer;
            
            pthread_t t1, t2;

            pthread_create(&t1, NULL, thread_conta_vogais_consoantes, &dados);
            pthread_create(&t2, NULL, thread_conta_palavras_espacos, &dados);

            pthread_join(t1, NULL);
            pthread_join(t2, NULL);
            break;
        }
    }

    return 0;
}

void* thread_conta_palavras_espacos(void* param) {
    struct data_chunk* dados = param;

    char* texto = dados->texto;
    int cont_palavras, num_letras, cont_espacos;
    cont_palavras = num_letras = cont_espacos = 0;

    // algoritmo extremamente simplificado para contagem de palavras e espaços
    for (int j = 0; texto[j] != '\0'; j++) {
        char letra = tolower(texto[j]);
        
        if (letra >= 'a' && letra <= 'z') num_letras++;
        else {
            if (letra == ' ') cont_espacos++;

            if (num_letras > 0) cont_palavras++;
            num_letras = 0;
        }
    } 

    printf("palavras: %d, espaços: %d\n", cont_palavras, cont_espacos);
    
    return NULL;
}

void* thread_conta_vogais_consoantes(void* param) {
    struct data_chunk* dados = param;

    char* texto = dados->texto;
    int cont_vogal, cont_consoante;
    cont_vogal = cont_consoante = 0;

    for (int j = 0; texto[j] != '\0'; j++) {
        char letra = tolower(texto[j]);

        if (letra == 'a' || letra == 'e' || letra == 'i' || letra == 'o' || letra == 'u') cont_vogal++;
        else if (letra >= 'a' && letra <= 'z') cont_consoante++;
    }

    printf("vogais: %d, consoantes: %d\n", cont_vogal, cont_consoante);
    
    return NULL;
}
