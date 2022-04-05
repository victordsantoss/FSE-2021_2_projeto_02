#ifndef SOCKET_H_
#define SOCKET_H_

int mainSocket();
void receberDados(char *json, int cliente_socket);
void enviarDados(char *json, int cliente_socket);
void receberRequisicao();

#endif 