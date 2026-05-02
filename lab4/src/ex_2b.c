#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

// Permite que seja feita uma finalização limpa com os sinais SIGINT e SIGTERM
void signal_handler(int signal);

volatile sig_atomic_t keep_going = 1;

int main() {
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);

    FILE* file = fopen("ovelhinhas.txt", "w");
    if (!file) {
        printf("fopen");
        return 1;
    }

    int ovelhinhas = 1;
    while (keep_going) {
        fprintf(file, "%d ovelhinhas,\n", ovelhinhas++);
        sleep(1);
    }

    fclose(file);
    return 0;
}

void signal_handler(int signal) {
    if (signal == SIGINT || signal == SIGTERM) {
        keep_going = 0;
    }
}
