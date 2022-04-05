#include "formatador.h";
#include "gpio.h";
#include <string.h>

void stringJson(char *json_string, Servidor *servidor){
    cJSON *json = cJSON_Parse(json_string);
    servidor->nome = malloc(sizeof(char)*20);
    strcpy(servidor->nome, cJSON_GetObjectItemCaseSensitive(json, "nome")->valuestring);
    servidor->saidas = cJSON_GetObjectItemCaseSensitive(json, "outputs");
    servidor->entradas = cJSON_GetObjectItemCaseSensitive(json, "inputs");
}

void jsonFinal(char **json_string, Servidor *servidor, int temperatura, int humidade, int *pessoas){
    cJSON *gpio_object = cJSON_CreateObject();
    int count = 2;
    char *gpio_type;
    cJSON *gpio;

    for (int i = 0; i < count; i++){
        if (i) gpio_type = "output";
        else gpio_type = "input";
        
        if (i) gpio = servidor->saidas;
        else gpio = servidor->entradas;

        cJSON *gpio_array = cJSON_CreateArray();
        int len = cJSON_GetArraySize(gpio);
        for (int j = 0; j < len; j++){
            cJSON *item = cJSON_GetArrayItem(gpio, j);
            char *type = cJSON_GetObjectItemCaseSensitive(item, "type")->valuestring;
            if (strcmp(type, "contagem") != 0){
                char *tag = cJSON_GetObjectItemCaseSensitive(item, "tag")->valuestring;
                int gpio = cJSON_GetObjectItemCaseSensitive(item, "gpio")->valueint;
                int value = read_gpio(gpio);
                cJSON *array_item = cJSON_CreateObject();
                cJSON_AddNumberToObject(array_item, "gpio", gpio);
                cJSON_AddStringToObject(array_item, "type", type);
                cJSON_AddStringToObject(array_item, "tags", tag);
                cJSON_AddNumberToObject(array_item, "value", value);
                cJSON_AddItemToArray(gpio_array, array_item);
            }
            else if (strcmp(type, "contagem") == 0){
                int gpio = cJSON_GetObjectItemCaseSensitive(item, "gpio")->valueint;
                if(gpio == 23) *pessoas += 1;
                else *pessoas -= 1;
            }
        }
        cJSON_AddItemToObject(gpio_object, gpio_type, gpio_array);
    }
    cJSON_AddStringToObject(gpio_object,"nome", servidor->nome);
    cJSON_AddNumberToObject(gpio_object, "temperature", temperatura);
    cJSON_AddNumberToObject(gpio_object, "humidity", humidade);
    cJSON_AddNumberToObject(gpio_object, "total_people", *pessoas);
    *json_string = malloc(5000);
    strcpy(*json_string, cJSON_Print(gpio_object));
}