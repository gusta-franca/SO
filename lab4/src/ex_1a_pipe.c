#include <ctype.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

// Estrutura para passar os parâmetros para as threads
struct data_chunk {
    char* texto;
};

// Thread para contar palavras e espaços de um trecho de texto em param
void* thread_conta_palavras_espacos(void* param);

// Thread para contar vogais e consoantes de um trecho de texto em param
void* thread_conta_vogais_consoantes(void* param);

int main() {
    char buffer_parent[256];
    char buffer_child[256];
    
    while (1) {   
        memset(buffer_parent, '\0', sizeof(buffer_parent));
        memset(buffer_child, '\0', sizeof(buffer_child));
        
        int pipe_str[2];
        pid_t pid;
        
        if (pipe(pipe_str)) {
            fprintf(stderr, "Falha ao criar o pipe.\n");
            return EXIT_FAILURE;
        }

        // Lê string digitada via terminal
        printf("> ");
        fgets(buffer_parent, sizeof(buffer_parent), stdin);

        pid = fork();
        
        if (pid < 0) {
            fprintf(stderr, "Falha ao executar o fork.\n");
        }
        else if (pid == 0) {
            close(pipe_str[1]);

            int fd = pipe_str[0];
            
            read(fd, buffer_child, sizeof(buffer_child)-1);
            
            printf("lido: %s\ntamanho: %zu\n", buffer_child, strlen(buffer_child)-1);

            struct data_chunk dados;
            dados.texto = buffer_child;
            
            pthread_t t1, t2;

            pthread_create(&t1, NULL, thread_conta_vogais_consoantes, &dados);
            pthread_create(&t2, NULL, thread_conta_palavras_espacos, &dados);

            pthread_join(t1, NULL);
            pthread_join(t2, NULL);

            exit(0);
        }
        else {
            close(pipe_str[0]);

            write(pipe_str[1], buffer_parent, sizeof(buffer_parent));

            wait(0);
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
