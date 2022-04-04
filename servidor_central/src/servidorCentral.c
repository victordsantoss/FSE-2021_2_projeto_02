#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <pthread.h>
#include <signal.h>

#include "cJSON.h"
#include "home.h"
#include "arquivo.h"

#define IP "192.168.0.53"
#define PORTA 10137
#define MAX_SIZE 5000
#define true 1
#define false 0

pthread_t SOCKET_GET, SOCKET_SEND, MENU_ID;
int s_cliente, id;
char *arquivo, *json_string, gpio = false;

void receberDados(){
  char *dados = malloc(MAX_SIZE);
  int tam_buffer = recv(s_cliente, dados, 5000, 0);
  if (tam_buffer) strcpy(json_string, dados);
  free(dados);
  sleep(1);
}

void enviarDados(){
  int value = 0;
  int file_size = strlen(arquivo);
  if (file_size > 0){
    if (!gpio) send(s_cliente, arquivo, file_size, 0);
    else{
      char *pin[5];
      printf("\nQual pino você deseja mudar o valor: ");
      scanf("%s", pin);
      send(s_cliente, pin, sizeof(char) * 5, 0);
      gpio = false;
    }
  }
  sleep(1);
}

void mudarSinal(int signal){
  printf("\nAguarde...\n");
  if (signal == SIGQUIT) gpio = true;
  if (signal == SIGTSTP){
    if(id == 1) id = 2;
    else id = 2; 
    strcpy(arquivo, lerArquivo(id));
  }
}

void central(int id){
  signal(SIGQUIT, mudarSinal);
  signal(SIGTSTP, mudarSinal);

  int servidor_socket;
  int opt = 1;
  struct sockaddr_in endereco_servidor;
  struct sockaddr_in endereco_cliente;
  unsigned int tam_cliente;

  if ((servidor_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) perror("Socket");

  memset(&endereco_servidor, 0, sizeof(endereco_servidor));
  endereco_servidor.sin_family = AF_INET;
  endereco_servidor.sin_addr.s_addr = inet_addr(IP);
  endereco_servidor.sin_port = htons(PORTA);

  setsockopt(servidor_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
  if (bind(servidor_socket, (struct sockaddr *)&endereco_servidor, sizeof(endereco_servidor)) < 0) perror("Bind");
  if (listen(servidor_socket, 10) < 0) perror("Listen");

  arquivo = malloc(MAX_SIZE);
  json_string = malloc(MAX_SIZE);
  strcpy(arquivo, lerArquivo(id));
  tam_cliente = sizeof(endereco_cliente);
  
  if ((s_cliente = accept(servidor_socket,(struct sockaddr *)&endereco_cliente,&tam_cliente)) < 0) perror("Accept");

  while (1){
    pthread_create(&MENU_ID, NULL, printMenu, json_string);
    pthread_join(MENU_ID, NULL);
    pthread_create(&SOCKET_GET, NULL, receberDados, NULL);
    pthread_create(&SOCKET_SEND, NULL, enviarDados, NULL);
    pthread_join(SOCKET_GET, NULL);
    pthread_join(SOCKET_SEND, NULL);
    sleep(1);
  }
  close(servidor_socket);
}