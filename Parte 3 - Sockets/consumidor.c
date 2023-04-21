#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

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

int main(int argc, char const *argv[]) {
    int producer_fd, new_socket; // descritores de arquivo para o socket do servidor e para um novo socket
    struct sockaddr_in address; // estrutura para o endereço do servidor
    int addrlen = sizeof(address); // tamanho da estrutura de endereço
    char buffer_out[1024] = {0}; // buffer para armazenar a mensagem enviada
    char buffer_in[1024] = {0}; // buffer para armazenar a mensagem recebida
    int num = 1; //número inicial a ser enviado para o consumidor

    int PORT; // número da porta do processo consumidor

    if(argc < 2){
        printf("ERRO - use a seguinte sintaxe: %s <PORTA>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    PORT = atoi(argv[1]);

    // cria um socket TCP para comunicação com o produtor
    if ((producer_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Erro na criação do socket");
        exit(EXIT_FAILURE);
    }

    // configura a estrutura de endereço alvo do socket
    address.sin_family = AF_INET; // seta o sistema de endereços de rede para IPv4
    address.sin_addr.s_addr = INADDR_ANY; // seta endereço alvo socket como o da máquina local, assim ele aceita comunicações de qualquer interface de rede
    address.sin_port = htons(PORT); // converte o número da porta para ordem de bytes de rede (big-endian)

    // vincula o socket à porta e endereço especificados
    if (bind(producer_fd, (struct sockaddr *)&address, sizeof(address))<0) {
        perror("Erro de vinculação");
        exit(EXIT_FAILURE);
    }

    // aguarda por conexões entrantes
    if (listen(producer_fd, 3) < 0) {
        perror("Erro ao aguardar por conexões");
        exit(EXIT_FAILURE);
    }

    // aceita uma nova conexão
    if ((new_socket = accept(producer_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) {
        perror("Erro ao aceitar conexão");
        exit(EXIT_FAILURE);
    }


    while(1) {
        // recebe número do produtor
        memset(buffer_in, 0, sizeof(buffer_in));
        if (read(new_socket, buffer_in, 1024) < 0) {
            perror("Erro ao receber número do processo produtor.");
            exit(EXIT_FAILURE);
        } 

        num = atoi(buffer_in);
        if (num == 0) {
            printf("Consumidor: Recebeu número 0, terminando.\n");
            
            // fecha os sockets e encerra o programa
            close(new_socket);
            close(producer_fd);
            return 0;
        }
        printf("Consumidor: Recebeu o número %d.\n", num);

        //verifica se o número enviado pelo produtor é primo
        if(eh_primo(num)) {
            sprintf(buffer_out, "O número %d é primo", num); // Conversão da resposta para string de tamanho fixo
        } else {
            sprintf(buffer_out, "O número %d não é primo", num);
        }

        //envia resposta ao produtor sobre se o número é primo ou não
        if (send(new_socket, buffer_out, strlen(buffer_out), 0) < 0) {
            perror("Erro ao enviar número ao procesos produtor");
            exit(EXIT_FAILURE);
        }

    }

    return 0;
}
