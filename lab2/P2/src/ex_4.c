// Exemplo de programa que fornece uma interface de shell simples
// Criado em 03/04/2026
// Autor: Gustavo Martins França

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_ARGS 20

// Separa a string input em tokens e os atribui à args
void parse_input(char* input, char** args) {
    int i = 0;

    // Separa a entrada em tokens e retorna o primeiro token
    args[i] = strtok(input, " ");

    // Faz o mesmo para o restante da entrada
    while (args[i] != NULL && i < MAX_ARGS - 1) {
        // A função strtok() contém um variável estática 
        // que armazena a string passada na primeira chamada da função.
        // Assim, não é necessário passar a variável novamente.
        args[++i] = strtok(NULL, " \n");
    }
}

int main() {
    char buffer[255];
    char* args[MAX_ARGS];

    while (1) {
        printf("> ");
        // Lê uma string de entrada e armazena num buffer
        fgets(buffer, sizeof(buffer), stdin);

        parse_input(buffer, args);

        if (args[0] == NULL) continue;

        int background = 0;
        int args_count = 0;

        while (args[args_count] != NULL) args_count++;
        // Verifica se há um caractere '&' no final do comando
        if (args_count > 0 && strcmp(args[args_count - 1], "&") == 0) {
            background = 1;
            
            // Remove '&' do comando
            args[args_count - 1] = NULL;
        }

        pid_t pid = fork();

        // Se for o processo filho executando,
        // troque seu código pelo comando
        if (pid == 0) {
            execvp(args[0], args);
            printf("Comando não encontrado\n");
            
            // Inalcançável se execvp executar
            exit(1);
        } 
        // Se o comando for executado em segundo plano, o processo pai o aguarda.
        else if (pid > 0) {
            if (!background) {
                waitpid(pid, NULL, 0);
            }
        } 
        else {
            printf("Não foi possível criar um novo processo\n");
        }
    }

    return 0;
}
