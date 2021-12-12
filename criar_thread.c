#include<stdio.h>
#include<stdlib.h>
#include<string.h>

void *t1_rotina_erro(void *arg) //Função da thread
{
    printf("Erro ao abrir o arquivo");
    return NULL;
}