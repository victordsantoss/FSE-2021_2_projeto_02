#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *lerArquivo(int caminho){

  char *p_arq;
  long tam;

  if (caminho == 1) p_arq = "./config/configuracao_andar_terreo.json";
  else p_arq = "./config/configuracao_andar_1.json";

  FILE *arquivo = fopen(p_arq, "r");

  if (arquivo == NULL){
    printf("ERRO! O arquivo não foi aberto!\n");
    exit(0);
  }

  fseek(arquivo, 0, SEEK_END);
  tam = ftell(arquivo);
  fseek(arquivo, 0, SEEK_SET);
  char *buffer = malloc(tam + 1);

  if (buffer) fread(buffer, 1, tam, arquivo);
  fclose(arquivo);

  return buffer;
}