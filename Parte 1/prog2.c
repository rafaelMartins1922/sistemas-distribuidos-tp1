#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>

// Handlers para cada sinal
void handle_sighup(int sig) {
    printf("Recebi o sinal SIGHUP\n");
}

void handle_sigint(int sig) {
    printf("Recebi o sinal SIGINT\n");
}

void handle_sigquit(int sig) {
    printf("Recebi o sinal SIGQUIT\n");
}

void handle_sigill(int sig) {
    printf("Recebi o sinal SIGILL, terminando o programa\n");
    exit(0);
}

int main(int argc, char *argv[]) {
    // Configura os handlers para cada sinal
    signal(SIGHUP, handle_sighup);
    signal(SIGINT, handle_sigint);
    signal(SIGQUIT, handle_sigquit);
    signal(SIGILL, handle_sigill);

    // Verifica se foi passado um argumento indicando a forma de espera
    if (argc != 2) {
        printf("ERRO - use a seguinte sintaxe para rodar: %s [busy/blocking]\n", argv[0]);
        return 1;
    }

    printf("Aguardando sinais\n");

    // Espera por sinais com busy wait
    if (strcmp(argv[1], "busy") == 0) {
        while (1) {
            
        }
    }

    // Espera por sinais com blocking wait
    else if (strcmp(argv[1], "blocking") == 0) {
        while (1) {
            pause(); 
        }
    }

    // Forma de espera inválida
    else {
        printf("Forma de espera inválida. Use 'busy' ou 'blocking'\n");
        return 1;
    }

    return 0;
}