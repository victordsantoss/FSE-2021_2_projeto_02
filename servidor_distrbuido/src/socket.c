
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <pthread.h>

#include "cJSON.h"
#include "gpio.h"
#include "dht22.h"
#include "message_formater.h"

#define IP "192.168.0.53"
#define PORTA 10137
#define MAX_SIZE 5000
pthread_t SOCKET_GET, SOCKET_SEND;

char *mensagem, *dados;
int temperatura = 0, humidade = 0, pessoas = 0, toggle_value = 0, cliente_socket;

void receberRequisicao(){
    int i;
    char num[5];
    recv(cliente_socket, dados, MAX_SIZE, 0);

    while (dados[i] != '\0'){
        if(i > 3)
            break;
        i++;
    }

    if (i < 3){
        printf("porta %d recebida", atoi(num));
        toggle_value = 1;
        strcpy(num, dados);
        int value = read_gpio(atoi(num));
        if (value == 0){
            printf("Ligando %d\n", atoi(num));
            turn_on(atoi(num));
        }
        else{
            printf("Desligando %d\n", atoi(num));
            turn_off(atoi(num));
        }
    }
    else{
        toggle_value = 0;
        strcpy(mensagem, dados);
    }
    sleep(1);
}

void enviarDados(){
    if (strlen(mensagem) > 0 && !toggle_value){
        char *final = malloc(MAX_SIZE);
        Server *server_config = malloc(sizeof(Server));
        parse_json_string(mensagem, server_config);
        read_dht_data(&temperatura, &humidade, 0);
        final_json(&final, server_config, temperatura, humidade, &pessoas);
        send(cliente_socket, final, MAX_SIZE, 0);
        free(final);
        free(server_config);
    }
    sleep(1);
}

void mainSocket(){
    struct sockaddr_in endereco_servidor;
    unsigned short porta_servidor;
    char *ip_servidor;
    mensagem = calloc(MAX_SIZE, sizeof(char));
    dados = calloc(MAX_SIZE, sizeof(char));
    ip_servidor = inet_addr(IP);
    porta_servidor = PORTA;

    if ((cliente_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0){
        perror("Socket");
        exit(0);
    }
    
    endereco_servidor.sin_family = AF_INET;
    endereco_servidor.sin_addr.s_addr = ip_servidor;
    endereco_servidor.sin_port = htons(porta_servidor);

    while (connect(cliente_socket, (struct sockaddr *)&endereco_servidor,sizeof(endereco_servidor)) < 0);

    while (1){
        pthread_create(&SOCKET_GET, NULL, receberRequisicao, NULL);
        pthread_join(SOCKET_GET, NULL);
        pthread_create(&SOCKET_SEND, NULL, enviarDados, NULL);
        pthread_join(SOCKET_SEND, NULL);
    }
}