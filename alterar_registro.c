#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"struct.h"

// Função para alterar attributes de registro específico

void alterar(FILE *arquivo)
{
    int id = 0;
    registro reg;

    if((arquivo = fopen("simpledb.dat", "r+b"))==NULL)
    {
        printf("Erro ao abrir o arquivo!");
    }
    
    printf("\nDigite o ID do registro que deseja alterar: ");
    scanf("%d",&id);
    
    printf("\nValue a ser reescritos: ");
    fgets(reg.attributes, 300, stdin); //desconsidera primeira entrada
    fgets(reg.attributes, 300, stdin); //grava dados da entrada stdin

    fseek(arquivo, (id - 1)*sizeof(registro), SEEK_SET); //Posicionar cursor no início do arquivo onde o ID informado é -1
    fwrite(&reg, sizeof(registro),1, arquivo); //Gravar dados em arquivo, consigderando o tamanho definido na struct
    fflush(arquivo); // Limpar buffer

    fclose(arquivo);

    system("clear"); //Limpar tela da aplicação
}