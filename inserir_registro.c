#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"struct.h"

// Função de inserir dados no arquivo binário

void inserir(FILE *arquivo, int cont)
{
    registro reg;
    reg.id = cont;

    if((arquivo = fopen("simpledb.dat", "r+b"))==NULL)
    {
        if((arquivo = fopen("simpledb.dat", "w+b"))==NULL)
        {
            printf("Erro ao abrir o arquivo!");
        }
    }

    printf("\nSORT-KEY: ");
    fgets(reg.type, 300, stdin); //desconsidera primeira entrada
    fgets(reg.type, 300, stdin); //desconsidera primeira entrada
    
    printf("\nVALUE: ");
    fgets(reg.attributes, 300, stdin); //grava dados da entrada stdin
    
    fseek(arquivo,0,SEEK_END); //Posicionar cursor no início do arquivo
    fwrite(&reg, sizeof(reg),1, arquivo); //Gravar dados em arquivo, consigderando o tamanho definido na struct
    fflush(arquivo);

    fclose(arquivo);

    system("clear"); //Limpar tela da aplicação
}