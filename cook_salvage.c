#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>
#include <sys/types.h>
#include <unistd.h>

#define COOKS 1
#define EATERS 20
#define PORTIONS 10

// Caldeirão com N porções = buffer[N];
int cauldron[PORTIONS];
int available_portions = 0;

// Mutex para controlar acesso ao caldeirão (cook e eater exclusivos);
sem_t mutex;

// Semáforo para controlar porções no caldeirão (posições cheias);
sem_t servings;

void *cook(void *param);

void *eater(void *param);

int main() {   
    int t_total = COOKS + EATERS;
    pthread_t t[t_total];
    int t_id[t_total];

    sem_init(&mutex, 0, 1);
    sem_init(&servings, 0, 0);

    for (int i = 0; i < EATERS; i++) {
        t_id[i] = i;
        pthread_create(&t[i], NULL, &eater, (void *)&t_id[i]);
    }

    for (int j = EATERS; j < t_total; j++) {
        t_id[j] = j - EATERS;
        pthread_create(&t[j], NULL, &cook, (void *)&t_id[j]);
    }

    for (int i = 0; i < t_total; i++) {
        pthread_join(t[i], NULL);
    }

    sem_destroy(&servings);
    sem_destroy(&mutex);

    return 0;
}

void *cook(void *param) {
    while (1) {
        // Tenta acessar cauldron (mutex);
        sem_wait(&mutex);
        
        // Se cauldron vazio, enche-o. Libera semáforo;
        if (available_portions == 0) {
            printf("cook %d working...\n", *(int*)param);
            
            for (int i = 0; i < PORTIONS; i++) {
                available_portions++;
                sem_post(&servings);
            }
        }

        sem_post(&mutex);

        // Se não, dorme (sleep).
        sleep(1);
    }
}

void *eater(void *param) {
    while (1) {
        // Tenta acessar semáforo;
        sem_wait(&servings);

        // Tenta acessar cauldron (mutex);
        sem_wait(&mutex);

        printf("eater %d is getting his food\n", *(int*)param);
        available_portions--;

        sem_post(&mutex);

        // Come (sleep, FORA dos semáforos).
        printf("eater %d is eating...\n", *(int*)param);
        sleep(5);
    }
}
