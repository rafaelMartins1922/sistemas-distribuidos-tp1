#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> // para função time 
#include <unistd.h> // para funções read, write e close
#include <sys/socket.h> // para funções de socket
#include <netinet/in.h> // para a estrutura sockaddr_in
#include <arpa/inet.h> // para a função inet_pton
// número da porta a ser usada para a comunicação

int main(int argc, char *argv[]) {
    int sock = 0; // descritor de arquivo para o socket
    struct sockaddr_in consumer_addr; // estrutura para o endereço do consumidor
    char buffer_out[1024] = {0}; // buffer para armazenar a mensagem enviada
    char buffer_in[1024] = {0}; // buffer para armazenar a mensagem recebida
    int num = 1; //número inicial a ser enviado para o consumidor
    int delta;  //número de incremento para num
    char* HOST; // IP do consumidor
    int PORT; // número da porta do processo consumidor
    int qntd_numeros; // quantidade de números a ser gerada

    if(argc < 4){
        printf("ERRO - use a seguinte sintaxe: %s <HOST> <PORTA> <QNT_NUMEROS>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    HOST = argv[1];
    PORT = atoi(argv[2]);
    qntd_numeros = atoi(argv[3]);

    // cria um socket TCP
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\nErro na criação do socket\n");
        return -1;
    }

    // configura a estrutura de endereço alvo do socket
    consumer_addr.sin_family = AF_INET; // seta o sistema de endereços de rede para IPv4
    consumer_addr.sin_port = htons(PORT); // converte o número da porta para ordem de bytes de rede (big-endian)

    // converte o endereço IP do consumidor de string para forma binária
    if (inet_pton(AF_INET, HOST, &consumer_addr.sin_addr) <= 0) {
        printf("\nEndereço inválido/Endereço não suportado\n");
        return -1;
    }

    // conecta o socket ao consumidor
    if (connect(sock, (struct sockaddr *)&consumer_addr, sizeof(consumer_addr)) < 0) {
        printf("\nErro de conexão\n");
        return -1;
    }

    srand(time(NULL)); //seta o valor da seed para a função rand() com o valor do tempo unix atual

    for (int i = 0; i < qntd_numeros; i++) {
        sprintf(buffer_out, "%d", num); // Conversão do número a ser enviado para string de tamanho fixo

        if (send(sock, buffer_out, strlen(buffer_out), 0) < 0) {
            perror("Erro ao envia número ao procesos consumidor");
            exit(EXIT_FAILURE);
        }
        printf("Produtor: Enviou número %d.\n", num);
        
         // recebe a resposta do consumidor
        if (read(sock, buffer_in, 1024) < 0) {
            perror("Erro ao envia número ao procesos consumidor");
            exit(EXIT_FAILURE);
        }

        printf("Resposta recebida: %s\n", buffer_in);
        memset(buffer_in, 0, sizeof(buffer_in));
        
        delta = rand() % 100 + 1; // Gera um número aleatório entre 1 e 100
        num += delta;

    }

    //envia número 0 para finalizar comunicação com consumidor
    sprintf(buffer_out, "%d", 0);
    if (send(sock, buffer_out, strlen(buffer_out), 0) < 0) {
        perror("Erro ao envia número ao procesos consumidor");
        exit(EXIT_FAILURE);
    }

    printf("Terminando.\n");
    close(sock);
    return 0;
}
