#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// qtd. barbeiros
#define BARBERS 1
// qtd. cadeiras
#define CHAIRS 2
// qtd. clientes
#define CLIENTS 5
#define T BARBERS+CLIENTS

pthread_t t[T];
sem_t mutex;
// Barbeiros disponíveis
sem_t barbers;
// Sinaliza que há clientes para o barbdeiro 
sem_t clients;
// Variável de controle para cadeiras disponíveis
int clients_waiting = 0;

void* barber(void* param);

void* client(void* param);

int main() {
    sem_init(&mutex, 0, 1);
    sem_init(&barbers, 0, 0);
    sem_init(&clients, 0, 0);

    int t_id[T];

    for (int i = 0; i < BARBERS; i++) {
        t_id[i] = i+1;
        pthread_create(&t[i], NULL, &barber, &t_id[i]);
    }

    for (int i = BARBERS; i < T; i++) {
        t_id[i] = i-BARBERS+1;
        pthread_create(&t[i], NULL, &client, &t_id[i]);
    }

    for (int i = 0; i < T; i++) {
        pthread_join(t[i], NULL);
    }

    sem_destroy(&clients);
    sem_destroy(&barbers);
    sem_destroy(&mutex);

    return 0;
}

void* barber(void* param) {
    int id = *(int*)param;
    while (1) {
        // dorme;
        // quando acordado por um cliente, o ajuda;
        // se houverem mais clientes na fila, os ajuda (um por vez);
        // se não, dorme;
        sem_wait(&clients);

        sem_wait(&mutex);

        // Sinaliza que um cliente pode vir cortar o cabelo
        clients_waiting--;
        
        // "Atende" um cliente
        sem_post(&barbers);

        sem_post(&mutex);

        printf("\nBarber %d: cutting hair...\n", id);
        
        // Simula tempo de cortar o cabelo
        sleep(2);

        printf("Barber %d: finished!\n", id);
    }

    pthread_exit(0);
}

void* client(void* param) {
    int id = *(int*)param;
    while (1) {
        // Faz alguma coisa
        sleep(5);

        sem_wait(&mutex);

        // Se houverem cadeiras disponíveis
        if (clients_waiting < CHAIRS) {
            clients_waiting++;

            // Acorda o barbeiro
            sem_post(&clients);

            // Libera mutex para que outras threads verifiquem se há cadeiras disponíveis
            sem_post(&mutex);

            sem_wait(&barbers);

            printf("Client %d: being satisfied...\n", id);
        }
        else {
            sem_post(&mutex);
            printf("Client %d: will come back later.\n", id);
        }
    }
    
    pthread_exit(0);
}
