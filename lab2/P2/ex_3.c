// Exemplo de programa que processe um vetor para n filhos localizarm um item
// Criado em 30/03/2026
// Autor: Gustavo Martins França

#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>

void fill_vector_random(int *v, int n) {
    for (int i = 0; i < n; i++) {
        v[i] = rand() % n;
        printf("%d ", v[i]);
    }

    printf("\n");
}

int main(int argc, char **argv) {
    int value = 4;
    int num_process = 3;
    int size_vector = 10;
    
    // É possível redefinir os parâmetros acima via terminal
    if (argc > 1) value = atoi(argv[1]);

    if (argc > 2) num_process = atoi(argv[2]);

    if (argc > 3) size_vector = atoi(argv[3]);

    // Cria-se um vetor qualquer
    int vector[size_vector];
    fill_vector_random(vector, size_vector);

    // Divide-se o vetor em n partes
    int size_subvector = size_vector / num_process;
    int index_start = 0;
    int index_end = size_subvector;

    pid_t pid_child;
    
    for (int i = 0; i < num_process; i++) {
        // Se ainda houverem elementos sobrando na última iteração, o último processo fica responsável por buscar também nessas posições
        index_start = i * size_subvector;
        index_end = (i == num_process-1) ? size_vector : (index_start + size_subvector);
        
        // Cria-se um processo para caad parte
        pid_child = fork();

        if (pid_child < 0) {
            printf("Não foi possível criar um novo processo\n");

            exit(EXIT_FAILURE);
        }

        if (pid_child == 0) {
            // Um mesmo filho pode encontrar o valor em mais de uma posição, portanto cada processo tem um vetor de índices
            int indexes_found[index_end - index_start];
            int found_count = 0;

            for (int j = index_start; j < index_end; j++) {
                if (vector[j] == value) {
                    indexes_found[found_count++] = j;
                }
            }

            for (int k = 0; k < found_count; k++) {
                printf("Filho %d encontrou na posição %d\n", getpid(), indexes_found[k]);
            }

            exit(found_count > 0 ? 1 : 0);
        }
    }

    int status;
    pid_t pid_child_finalized;

    int found_any = 0;

    // Espera todos os processos filho finalizarem
    for (int i = 0; i < num_process; i++) {
        pid_child_finalized = wait(&status);

        if (WIFEXITED(status)) {
            if (WEXITSTATUS(status) == 1) {
                found_any = 1;
            }
        }
    }

    if (!found_any) {
        printf("Pai %d: valor %d não foi encontrado\n", getpid(), value);
    } 

    return 0;
}
