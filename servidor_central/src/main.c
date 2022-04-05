#include <stdio.h>
#include "servidorCentral.h";
#include "home.h";

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

int main(){
    int id = menuInicial();
    central(id);
    return 0;
}

