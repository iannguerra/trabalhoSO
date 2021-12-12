#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"struct.h"

// Função para selecionar informações específicas

void selecionar_especifico(FILE *arquivo)
{
    registro reg;
    int encontrar = 0;

    if((arquivo = fopen("simpledb.dat", "r+b"))==NULL)
    {
        printf("Erro ao abrir o arquivo!");
    }

    printf("Digite o id: ");
    scanf("%d",&encontrar);

    while (fread(&reg,sizeof(registro),1,arquivo)==1)
    {
       if(reg.id == encontrar)
        {
            printf("\n\nDados:");
            printf("\n\nid: %d\nsort-key: %s\nattributes: %s",reg.id,reg.type,reg.attributes);
        }
    }

    fclose(arquivo);
    
    getchar(); getchar(); //Capturar um caractere (Enter)
    system("clear"); //Limpar tela da aplicação        
}