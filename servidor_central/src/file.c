#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *readFile(int path){

  char *fp;
  long size;

  if (path == 1) fp = "./config/configuracao_andar_terreo.json";
  else fp = "./config/configuracao_andar_1.json";

  FILE *file = fopen(fp, "r");

  if (file == NULL){
    printf("ERRO! O arquivo não foi aberto!\n");
    exit(0);
  }

  fseek(file, 0, SEEK_END);
  size = ftell(file);
  fseek(file, 0, SEEK_SET);
  char *buffer = malloc(size + 1);

  if (buffer) fread(buffer, 1, size, file);
  fclose(file);

  return buffer;
}