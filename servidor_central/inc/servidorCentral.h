#ifndef SERVIDOR_H_
#define SERVIDOR_H_

void enviarDados(int sCliente, char *arquivo);
void receberDados(int sCliente);
void mudarSinal(int sinal);
void central(int id);

#endif