// Exemplo de programa que processe um vetor para n threads localizarm um item
// Criado em 12/04/2026
// Autor: Gustavo Martins França

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>


// Estrutura para passar os parâmetros para os threads
struct data_chunk {
    int num_seq,
        val,
        start,
        end;
    int* vec;
};


// Preenche um vetor com valores inteiros semi-aleatórios
void fill_vector_random(int *vec, int n);

// Função para buscar um inteiro em um intervalo de um vetor; rotina dos threads
void* find_integer(void* param);


int main(int argc, char** argv) {
    int value = 4;
    int num_threads = 3;
    int size_vector = 10;
    
    // É possível redefinir os parâmetros acima via terminal
    if (argc > 1) value = atoi(argv[1]);

    if (argc > 2) num_threads = atoi(argv[2]);

    if (argc > 3) size_vector = atoi(argv[3]);

    // Cria-se um vetor qualquer
    int vector[size_vector];
    fill_vector_random(vector, size_vector);

    // Divide-se o vetor em n partes e as separa para cada thread
    int size_subvector = size_vector / num_threads;
    int index_start = 0;
    int index_end = size_subvector;
        
    struct data_chunk data[num_threads];

    for (int k = 0; k < num_threads; k++) {
        data[k].num_seq = k;
        data[k].val = value;
        data[k].start = index_start;
        data[k].end = index_end;
        data[k].vec = vector;

        index_start += size_subvector;
        index_end += size_subvector;
    }
    data[num_threads-1].end = size_vector;

    pthread_t threads[num_threads];
    
    // Cria e invoca um thread para cada parte
    for (int i = 0; i < num_threads; i++) {
        pthread_create(&threads[i], NULL, find_integer, &data[i]);
    }

    int found_any = 0;
    void* status;

    // Aguarda a finalização dos threads
    for (int j = 0; j < num_threads; j++) {
        pthread_join(threads[j], &status);

        if ((int)status == 1) {
            found_any = 1;
        }
    }

    if (!found_any) {
        printf("Valor %d não foi encontrado\n", value);
    } 

    return 0;
}


void fill_vector_random(int *v, int n) {
    for (int i = 0; i < n; i++) {
        v[i] = rand() % n;
        printf("%d ", v[i]);
    }

    printf("\n");
}


void* find_integer(void* param) {
    int found = 0;
    struct data_chunk* data = param;

    for (int i = data->start; i < data->end; i++) {
        if (data->vec[i] == data->val) {
            printf("Thread %d encontrou na posição %d\n", data->num_seq, i);
            found = 1;
        }
    }

    pthread_exit((void*)found); // https://stackoverflow.com/questions/11796909/how-to-resolve-cast-to-pointer-from-integer-of-different-size-warning-in-c-co
}
