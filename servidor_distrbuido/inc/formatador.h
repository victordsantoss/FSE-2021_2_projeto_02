#ifndef FORMATADOR_H_
#define FORMATADOR_H__H_

#include "cJSON.h";

typedef struct Servidor{
    int porta;
    char *ip, *nome;
    cJSON *saidas, *entradas;
} Servidor;

void stringJson(char *json_string, Servidor *servidor);
void jsonFinal(char **json_string, Servidor *servidor, int temperatura, int humidade, int *pessoas);

#endif 