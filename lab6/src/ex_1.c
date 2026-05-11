#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define T 5
#define M 112
#define N 112

int matrix[M][N],
    current_row = 0, 
    total_sum = 0;
pthread_t t[T];
pthread_mutex_t mutex;

void fill_matrix_random(int m, int n, int matrix[m][n]);

void* sum_matrix_row(void* param);

int main() {
    pthread_mutex_init(&mutex, NULL);

    fill_matrix_random(M, N, matrix);

    int t_id[T];

    for (int i = 0; i < T; i++) {
        t_id[i] = i+1;
        pthread_create(&t[i], NULL, &sum_matrix_row, &t_id[i]);
    }

    for (int i = 0; i < T; i++) {
        pthread_join(t[i], NULL);
    }

    printf("Total sum: %d\n", total_sum);

    pthread_mutex_destroy(&mutex);

    return 0;
}

void fill_matrix_random(int m, int n, int matrix[m][n]) {
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            matrix[i][j] = 1; 

            // printf("%4d", matrix[i][j]);
        }
        // printf("\n");
    }
    // printf("\n");
}

void* sum_matrix_row(void* param) {
    while (1) {
        int row = -1;
        
        // Maybe one mutex to get rows and one to update total_sum?
        pthread_mutex_lock(&mutex);
        if (current_row < N) {
            row = current_row;
            current_row++;
    
            int sum = 0;
            for (int i = 0; i < N; i++) {
                sum += matrix[row][i];
            }
            
            total_sum += sum;
            printf("Thread %d - Row %d: %d\n", *(int*)param, row+1, sum);
        }
        pthread_mutex_unlock(&mutex);

        if (row == -1) break;
    }

    pthread_exit(0);
}
