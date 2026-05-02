// Exemplo de programa que computa a média simples de cada linha de uma matriz e média geométrica de cada coluna
// Criado em 12/04/2026
// Autor: Gustavo Martins França

#define _POSIX_C_SOURCE 199309L

#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


// Estrutura para passar os parâmetros para os threads
struct data_chunk {
    int start,
        end,
        m,
        n;
};


// Preenche uma matriz com valores inteiros semi-aleatórios
void fill_matrix_random(int **matrix, int m, int n);

// Função para computar média aritmética de uma linha ou média geométrica de uma coluna; rotina dos threads
void* compute_mean(void* param);

// Salva resultados em arquivo texto
void save_results(int m, int n);

double* means;
double* geometric_means;
int** matrix;

int main(int argc, char** argv) {
    // Cálculo de tempo considerando todo o programa
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    int num_threads = 3;
    int m = 6;
    int n = 8;

    // É possível redefinir os parâmetros acima via terminal
    if (argc > 1) num_threads = atoi(argv[1]);

    if (argc > 2) {
        m = atoi(argv[2]);
        n = m;
    }

    if (argc > 3) {
        n = atoi(argv[3]);
    }

    means = malloc(m * sizeof(double));
    geometric_means = malloc(n * sizeof(double));
    matrix = malloc(m * sizeof(int*));
    for (int i = 0; i < m; i++) {
        matrix[i] = malloc(n * sizeof(int));
    }

    fill_matrix_random(matrix, m, n);
    
    // Cada thread pega um workload de tasks para processar
    int total_tasks = m+n;
    int workload = total_tasks / num_threads;
    
    int task_start = 0;
    int task_end = workload;
    
    struct data_chunk data[num_threads];
    
    for (int k = 0; k < num_threads; k++) {
        data[k].start = task_start;
        data[k].end = task_end;
        data[k].m = m;
        data[k].n = n;
        
        task_start += workload;
        task_end += workload;
    }
    
    data[num_threads-1].end = total_tasks;

    pthread_t threads[num_threads];
    
    // Cria e invoca um thread para cada parte
    for (int i = 0; i < num_threads; i++) {
        pthread_create(&threads[i], NULL, compute_mean, &data[i]);
    }

    // Aguarda a finalização dos threads
    for (int j = 0; j < num_threads; j++) {
        pthread_join(threads[j], NULL);
    }

    save_results(m, n);

    free(means);
    free(geometric_means);
    for (int i = 0; i < m; i++) {
        free(matrix[i]);
    }
    free(matrix);

    clock_gettime(CLOCK_MONOTONIC, &end);
    double time_spent;
    time_spent = (end.tv_sec - start.tv_sec) * 1e9;
    time_spent = (time_spent + (end.tv_nsec - start.tv_nsec)) * 1e-9;
    printf("Execution time: %f\n", time_spent);

    return 0;
}


void fill_matrix_random(int **matrix, int m, int n) {
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            matrix[i][j] = 1 + rand() % (m+n); 

            printf("%4d", matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}


void* compute_mean(void* param) {
    struct data_chunk* data = param;

    for (int i = data->start; i < data->end; i++) {
        if (i < data->m) {
            double mean = 0.0;

            for (int j = 0; j < data->n; j++) {
                mean += matrix[i][j];
            }

            means[i] = mean / data->n;

        } else {
            double geometric_mean = 1.0;
            int column = i - data->m;

            for (int k = 0; k < data->m; k++) {
                geometric_mean *= matrix[k][column];
            }

            geometric_means[column] = pow(geometric_mean, 1.0 / data->m);
        }
    }

    pthread_exit(NULL);
}


void save_results(int m, int n) {
    char filepath[] = "build/results.txt";
    FILE* file = fopen(filepath, "w");

    if (file == NULL) {
        printf("Erro ao salvar os resultados\n");
        
        return;
    }

    // fprintf(file, "Matrix (%dx%d):\n", m, n);
    // for (int i = 0; i < m; i++) {
    //     for (int j = 0; j < n; j++) {
    //         fprintf(file, "%4d ", matrix[i][j]);
    //     }
    //     fprintf(file, "\n");
    // }
    // fprintf(file, "\n");

    fprintf(file, "Means:\n");
    for (int i = 0; i < m; i++) {
        fprintf(file, "Row %d: %f\n", i+1, means[i]);
    }

    fprintf(file, "\nGeometric Means:\n");
    for (int i = 0; i < n; i++) {
        fprintf(file, "Column %d: %f\n", i+1, geometric_means[i]);
    }

    // printf("Results saved in %s\n", filepath);

    fclose(file);
}
