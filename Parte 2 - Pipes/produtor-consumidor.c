#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define PIPE_READ_END 0
#define PIPE_WRITE_END 1
#define BUFFER_SIZE 1024


// Função para verificar se um número é primo
int eh_primo(int num) {
    if (num <= 1) {
        return 0;
    }
    for (int i = 2; i <= num/2; i++) {
        if (num % i == 0) {
            return 0;
        }
    }
    return 1;
}

int main(int argc, char *argv[]) {
    int qntd_numeros, i, num = 1, delta, status;
    int pipefd[2]; // Descritores de arquivo do pipe
    pid_t pid;
    char buffer[BUFFER_SIZE]; // String de tamanho fixo para armazenar cada número enviado

    if (argc != 2) {
        printf("Uso: %s <qntd_numeros>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    qntd_numeros = atoi(argv[1]);

    if (pipe(pipefd) == -1) {
        perror("Erro ao criar pipe");
        exit(EXIT_FAILURE);
    }

    pid = fork();

    if (pid == -1) {
        perror("Erro ao criar processo filho");
        exit(EXIT_FAILURE);
    }

    else if (pid == 0) { // Processo filho (Consumidor)
        close(pipefd[PIPE_WRITE_END]);

        do {
            if (read(pipefd[PIPE_READ_END], buffer, sizeof(buffer)) == -1) {
                perror("Erro na leitura do pipe");
                exit(EXIT_FAILURE);
            }

            num = atoi(buffer);

            if (num == 0) {
                printf("Consumidor: Recebeu número 0, terminando.\n");
                close(pipefd[PIPE_READ_END]);
                exit(EXIT_SUCCESS);
            }

            if (eh_primo(num)) {
                printf("Consumidor: Recebido número %d, é primo.\n", num);
            } else {
                printf("Consumidor: Recebido número %d, não é primo.\n", num);
            }

        } while (1);

    } 
    
    else { // Processo pai (Produtor)
        close(pipefd[PIPE_READ_END]);

        srand(time(NULL));

        for (i = 0; i < qntd_numeros; i++) {
            delta = rand() % 100 + 1; // Gera um número aleatório entre 1 e 100
            num += delta;
            sprintf(buffer, "%d", num); // Conversão para string de tamanho fixo

            if (write(pipefd[PIPE_WRITE_END], buffer, sizeof(buffer)) == -1) {
                perror("Erro ao escrever no pipe");
                exit(EXIT_FAILURE);
            }

            printf("Produtor: Enviou número %d.\n", num);

            sleep(1); // Espera para que o consumidor possa receber
        }

        sprintf(buffer, "%d", 0); // Envia número 0 para indicar fim da produção

        if (write(pipefd[PIPE_WRITE_END], buffer, sizeof(buffer)) == -1) {
            perror("Erro ao escrever no pipe");
            exit(EXIT_FAILURE);
        }

        printf("Produtor: Enviou número 0, terminando.\n");

        close(pipefd[PIPE_WRITE_END]);

        wait(&status);
    }

    return 0;
}
