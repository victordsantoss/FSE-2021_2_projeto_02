#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"

int menuInicial(){
    int id;

    system("clear");
    printf("FUNDAMENTO DE SISTEMAS EMBARCADO - FSE | PROJETO 02");

    while (1){
        printf("\n\nMonitorar Térreo - 1\nMonitorar Primeiro Andar - 2\n\n");
        scanf("%d", &id);
        if (id < 1 || id > 2) printf("\nID inválido\n");
        else return id;
    }
}

void printMenu(char *arquivo){
    if (strlen(arquivo) > 0){

        cJSON *json = cJSON_Parse(arquivo);
        system("clear");
        int temperatura = cJSON_GetObjectItemCaseSensitive(json, "temperature")->valueint;
        int humidade = cJSON_GetObjectItemCaseSensitive(json, "humidity")->valueint;
        int pessoas = cJSON_GetObjectItemCaseSensitive(json, "total_people")->valueint;
        int *nome = cJSON_GetObjectItemCaseSensitive(json, "nome")->valuestring;

        cJSON *saidas = cJSON_GetObjectItemCaseSensitive(json, "output");
        cJSON *entradas = cJSON_GetObjectItemCaseSensitive(json, "input");
        cJSON *pin = NULL;


        printf("Piso monitorado: %s\n", nome);
        printf("Temperatura: %d⁰C\nÚmidade: %d%%\nTotal de pessoas no prédio: %d\n", temperatura, humidade, pessoas);
        printf("\n\n==================== Saída ====================\n\n");
        printf("  Pino\t  Valor\t  Tag\n");

        cJSON_ArrayForEach(pin, saidas){
            char *status;
            int gpio = cJSON_GetObjectItemCaseSensitive(pin, "gpio")->valueint;
            char *tag = cJSON_GetObjectItemCaseSensitive(pin, "tags")->valuestring;
            int value = cJSON_GetObjectItemCaseSensitive(pin, "value")->valueint;

            if(value) status = "ON";
            else status = "OFF";
            printf("|| %02d\t|| %s\t|| %s\t\n", gpio, status, tag);
        }

        printf("\n\n==================== Entrada ====================\n\n");
        printf("  Pino\t  Valor\t  Tag\n");  
            
        cJSON_ArrayForEach(pin, entradas){
            char *status;
            int gpio = cJSON_GetObjectItemCaseSensitive(pin, "gpio")->valueint;
            char *tag = cJSON_GetObjectItemCaseSensitive(pin, "tags")->valuestring;
            int value = cJSON_GetObjectItemCaseSensitive(pin, "value")->valueint;

            if(value) status = "ON";
            else status = "OFF";
            printf("|| %02d\t|| %s\t|| %s\t\n", gpio, status, tag);
        }
    }
}