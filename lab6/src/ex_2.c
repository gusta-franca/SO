#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

pthread_mutex_t mutex;
int ping_time = 1,
    pong_time = 0;

void* ping(void* param);

void* pong(void* param);

int main(int argc, char* argv[]) {
    int t_ping_count = 1;
    int t_pong_count = 1;

    if (argc > 1) {
        t_ping_count = atoi(argv[1]);
    }

    if (argc > 2) {
        t_pong_count = atoi(argv[2]);
    }

    int t_count = t_ping_count + t_pong_count;

    pthread_t t[t_count];
    int t_id[t_count];

    pthread_mutex_init(&mutex, NULL);

    for (int i = 0; i < t_ping_count; i++) {
        t_id[i] = i+1;
        pthread_create(&t[i], NULL, &ping, &t_id[i]);
    }

    for (int j = t_ping_count; j < t_count; j++) {
        t_id[j] = j+1;
        pthread_create(&t[j], NULL, &pong, &t_id[j]);
    }

    for (int k = 0; k < t_count; k++) {
        pthread_join(t[k], NULL);
    }

    pthread_mutex_destroy(&mutex);

    return 0;
}

void* ping(void* param) {
    while (1) {
        pthread_mutex_lock(&mutex);
        if (ping_time) {
            printf("Thread %d: ping\n", *(int*)param);

            ping_time = 0;
            pong_time = 1;
        }
        pthread_mutex_unlock(&mutex);
    }

    pthread_exit(0);
}

void* pong(void* param) {
    while (1) {
        pthread_mutex_lock(&mutex);
        if (pong_time) {
            printf("Thread %d: pong\n", *(int*)param);
            
            ping_time = 1;
            pong_time = 0;
        }
        pthread_mutex_unlock(&mutex);
    }
    
    pthread_exit(0);
}
