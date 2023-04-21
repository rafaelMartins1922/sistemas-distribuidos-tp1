#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("ERRO - use a seguinte sintaxe: %s <PID> <SINAL>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    pid_t pid = atoi(argv[1]); // pid do consumidor
    char* signame = argv[2]; // nome do sinal escolhido pelo usuário
    int sigvalue; // valor do sinal

     if (strcmp(signame, "SIGHUP") == 0) {
        sigvalue = SIGHUP;
    } else if (strcmp(signame, "SIGINT") == 0) {
        sigvalue = SIGINT;
    } else if (strcmp(signame, "SIGQUIT") == 0) {
        sigvalue = SIGQUIT;
    } else if (strcmp(signame, "SIGILL") == 0) {
        sigvalue = SIGILL;
    } else {
        printf("Nome de sinal inválido\n");
        return 1;
    }

    // Checar se processo consumidor existe
    if (kill(pid, 0) == -1) {
        if (errno == ESRCH) {
            printf("Processo com PID %d não existe\n", pid);
        } else {
            perror("kill");
            exit(EXIT_FAILURE);
        }
    } else {
        // Enviar sinal ao consumidor
        if (kill(pid, sigvalue) == -1) {
            perror("kill");
            exit(EXIT_FAILURE);
        }

        printf("Sinal %s enviado ao processo %d\n", signame, pid);
    }

    return 0;
}