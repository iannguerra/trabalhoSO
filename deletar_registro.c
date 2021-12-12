#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"struct.h"


// Função para deletar registro específico

void deletar(FILE *arquivo)
{
    FILE *arquivo_aux;
    registro reg;
    int id_excluir = 0;

    if((arquivo = fopen("simpledb.dat", "rb+"))==NULL)
    {
        printf("Erro ao abrir o arquivo!");
    }

    if((arquivo_aux = fopen("arquivo_aux.dat", "w+b"))==NULL) //Reescrever arquivo auxiliar, caso já exista
    {
        printf("Erro ao abrir o arquivo!");
    }    

    printf("\nDigite o ID do registro que deseja excluir: ");
    scanf("%d",&id_excluir);
    
    while (fread(&reg,sizeof(registro),1,arquivo)==1)
    {
       if(reg.id != id_excluir) //Arquivo auxiliar preenchido por registros com ID's diferentes do selecionado
        {
            fwrite(&reg, sizeof(registro),1,arquivo_aux); //dados salvos em arquivo temporário
        }
    }

    fclose(arquivo_aux); //Fechar arquivo
    fclose(arquivo); //Fechar arquivo

    remove("simpledb.dat"); //Deletar arquivo binário com id a ser excluído
    rename("arquivo_aux.dat", "simpledb.dat"); //Renomear auxiliar com os dados corretos para o nome do antigo arquivo

    system("clear"); //Limpar tela da aplicação
}