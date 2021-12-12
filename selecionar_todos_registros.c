#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"struct.h"

// Função para selecionar todas as informações presentes no arquivo

void selecionar(FILE *arquivo)
{
    registro reg;

    if((arquivo = fopen("simpledb.dat", "r+b"))==NULL)
    {
        printf("Erro ao abrir o arquivo!");
    }

    while (fread(&reg,sizeof(registro),1,arquivo)==1) // Lê informações enquanto não chegar ao fim do arquivo
    {
        printf("\n\nDados:");
        printf("\nid: %d\nsort-key: %s\nattributes: %s",reg.id,reg.type,reg.attributes);
    }
    
    fclose(arquivo);

    getchar(); getchar(); //Capturar um caractere (Enter) 
    system("clear"); //Limpar tela da aplicação        
}